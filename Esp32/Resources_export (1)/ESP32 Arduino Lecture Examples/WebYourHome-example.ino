#include <WiFi.h> 

#define LEDPIN 21

const char* ssid     = "youhomessid";
const char* password = "yourhomepass";

const char body[] PROGMEM = R"===(HTTP/1.1 200 OK
Content-type:text/html

<!DOCTYPE html>
<html><body>
<h1>
<a href="/H">Turn ON</a> LED.<br>
<a href="/L">Turn OFF</a> LED.<br>
</h1>
</body></html>
)===";

WiFiServer server(80); 

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status()!= WL_CONNECTED ) { 
    delay(500);
    Serial.print("."); 
  }
  Serial.println("WiFi connected"); server.begin();
  
  Serial.print("Use this URL to connect: http://");  
  Serial.print(WiFi.localIP()); Serial.println("/");

}

void loop(){
  WiFiClient client = server.available(); // loop until we have a client

  if (client) {
    String currentLine = "";              // incoming data
    while (client.connected()) {
      if (client.available()) {           // if bytes to read
        char c = client.read();           // read a byte, 
        Serial.write(c); //** just for debugging comment this out.
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
