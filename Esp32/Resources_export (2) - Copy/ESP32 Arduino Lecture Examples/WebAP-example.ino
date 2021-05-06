#include <WiFi.h> 

#define LEDPIN 21
const char* ssid = "ESP32-AP";

const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html><body>
<a href="/H">Turn ON</a> LED.<br>
<a href="/L">Turn OFF</a> LED.<br>
</body></html>
)===";

WiFiServer server(80); 

void setup() {
  Serial.begin(115200);                                             
  pinMode(LEDPIN, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
 
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
}

void loop(){
  WiFiClient client = server.available(); // loop until we have a client

  if (client) {
    String currentLine = "";              // incoming data
    while (client.connected()) {
      if (client.available()) {           // if bytes to read
        char c = client.read();           // read a byte, 
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {     // if blank line
            client.print(body);
            break;    // exit while loop
          } else { 
            if (currentLine.startsWith("GET /H "))    
              digitalWrite(LEDPIN, HIGH);  // LED ON  
            if (currentLine.startsWith("GET /L ")) 
              digitalWrite(LEDPIN, LOW);   // LED OFF
            currentLine = "";   // if new line but not blank, clear data
          }
        } else if (c != '\r') {  // if anything but a CR
          currentLine += c;      // add it to the end
        }
      }
    }
    client.stop();  // close the connection
  }
}
