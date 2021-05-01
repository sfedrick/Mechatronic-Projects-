#include <WiFi.h>
#include "html510.h"

#define MotorSpeed1 36
#define MotorSpeed2 39
#define MotorSpeed3 35
#define MotorSpeed4 34

#define SERVOPIN1    38
#define SERVOPIN2    37


#define MotorDirection1 25
#define MotorDirection2 26
#define MotorDirection3 27
#define MotorDirection4 14


#define Touch1 33
#define Touch2 33

#define TOF_ready 12
#define TOF_on 13


#define SERVOFREQ    60


WiFiServer server(80);

//html
const char* ssid     = "Fios-G8WhZ";
const char* password = "Jesuisungrenouille777";

const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html><body>

<H1>
Hello
</H1>


<script>

</script>


</html>
)===";


/* web handler   */
void handleRoot(){
  sendhtml(body);
}

void setup() {
  // put your setup code here, to run once:



  Serial.begin(115200);                                             
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  WiFi.config(IPAddress(192, 168, 1, 109),
              IPAddress(192, 168, 1, 1),
              IPAddress(255, 255, 255, 0));
              
  while(WiFi.status()!= WL_CONNECTED ) { 
    delay(500);
    Serial.print("."); 
  }
  Serial.println("WiFi connected"); server.begin();

  Serial.begin(115200);
 //states of the led
  attachHandler("/ ",handleRoot);

}

void loop() {
  // put your main code here, to run repeatedly:
 serve(server, body);
}
