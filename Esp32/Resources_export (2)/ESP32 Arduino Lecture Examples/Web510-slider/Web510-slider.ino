
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
#include "sliderJS.h"

#define LEDPIN 21
                         
//const char* ssid     = "yourhomeSSID";
//const char* password = "yourhomesasword";

WiFiServer server(80);              


/*****************/
/* web handler   */
void handleRoot(){
  sendhtml(body);
}

void handleH(){
  digitalWrite(LEDPIN, HIGH);  // LED ON  
  sendhtml(body);
}                    
                     
void handleL(){
  digitalWrite(LEDPIN, LOW);  // LED ON  
  sendhtml(body);
}

void handleSlider(){
  String s = "LED ";
  if (getVal()> 50)  digitalWrite(LEDPIN,HIGH);
  else  digitalWrite(LEDPIN,LOW);
  if (digitalRead(LEDPIN)) s = s+ "on";
  else s = s+ "off";
  
  sendplain(s);
  Serial.printf("%s\n",s);
}


void setup() {
  Serial.begin(115200);                                             
  pinMode(LEDPIN, OUTPUT);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  WiFi.config(IPAddress(192, 168, 1, 6),
              IPAddress(192, 168, 1, 1),
              IPAddress(255, 255, 255, 0));
              
  while(WiFi.status()!= WL_CONNECTED ) { 
    delay(500);
    Serial.print("."); 
  }
  Serial.println("WiFi connected"); server.begin();
 
  attachHandler("/H",handleH);
  attachHandler("/L",handleL);
  attachHandler("/ ",handleRoot);
  attachHandler("/slider?val=",handleSlider);
}

void loop(){
  serve(server, body);
  delay(1);
}
