/*
 * MEAM510 HTML510 example w/LED
 * March 2021
 * 
 * Mark Yim
 * University of Pennsylvania
 * copyright (c) 2021 All Rights Reserved
 */
 
#include <WiFi.h> 
#include "html510.h"

#define LEDPIN 21

#define LEDC_CHANNEL       0 // use first channel of 16  
#define LEDC_RESOLUTION_BITS  8
#define LEDC_FREQ_HZ     5000
#define LED_PIN           21
#define LED_ANALOG       A3
int duty=0;
int readin=0;
 //wifi password                         
const char* ssid     = "Fios-G8WhZ";
const char* password = "Jesuisungrenouille777";
//const char* ssid     = "Tester";
//const char* password = "12345678";

WiFiServer server(80);              
//html
const char body[] PROGMEM = R"===(
<!DOCTYPE html>
<html><body>

<H1>
<span id="somelabel"> Duty Cycle  </span> <br>
<span id="duty"> Duty Cycle  </span> <br>
<span id="somelabel"> Frequency  </span> <br>
<span id="Frequency"> Frequency </span> <br>
</H1>

<script>


setInterval(updateFreq,100);
//changes the url to get the frequency signal going to the pin
function updateFreq() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Frequency").innerHTML =   
             this.responseText;
    }
  };
  xhttp.open("GET", "Freqstate", true);
  xhttp.send();
}

setInterval(updateDuty,100);
//changes the url to get the pwm signal going to the pin
function updateDuty() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("duty").innerHTML =   
             this.responseText;
    }
  };
  xhttp.open("GET", "Dutystate", true);
  xhttp.send();
}

</script>

</html>
)===";

/*****************/
/* web handler   */
void handleRoot(){
  sendhtml(body);
}
                 
// sends frequency signal
void handleFreqstate(){
  String s = "Frequency is "+String(LEDC_FREQ_HZ);
  sendplain(s);
}
//sends pwm signal
void handleDutystate(){
  String s = "Duty cycle is "+String((float)duty/255.0);
  sendplain(s);
}                    
                    

void setup() {
  Serial.begin(115200);                                             
  pinMode(LEDPIN, OUTPUT);
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
  ledcSetup(LEDC_CHANNEL, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL);
 //states of the led
  attachHandler("/ ",handleRoot);
  attachHandler("/Freqstate",handleFreqstate);
  attachHandler("/Dutystate",handleDutystate);

}

void loop(){
  
  serve(server, body);
  //readin analog from the A3 pin
  readin=analogRead(A3);
  //linear maps duty to the ledc resolution
  duty=map(readin,0,1770,0,255);
  //performs ledc 
  ledcWrite(LEDC_CHANNEL, duty);  // write duty to LEDC 
 
}
