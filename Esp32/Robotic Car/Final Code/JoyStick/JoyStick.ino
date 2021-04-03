

/*Built off of:
 * MEAM510 Lab 4 demo
 * March 2021
 * 
 * Mark Yim
 * University of Pennsylvania
 * copyDirection (c) 2021 All Directions Reserved
 */
#include <WiFi.h>
#include "html510.h"
#include "joyJS.h"
#include "tankJS.h"

WiFiServer server(80);
//upenn wifi
/*
const char* ssid     = "#Skyroam_1t9";
const char* password = "55687127";
*/
//home wifi

const char* ssid     = "Fios-G8WhZ";
const char* password = "Jesuisungrenouille777";

const char *body;

/********************/
/* HTML510  web   */
void handleFavicon(){
  sendplain(""); // acknowledge
}

void handleRoot() {
  sendhtml(body);
}

void handleSwitch() { // Switch between JOYSTICK and TANK mode
  String s="";
  static int toggle=0;
  if (toggle) body = joybody;
  else body = tankbody;
  toggle = !toggle;
  sendplain(s); //acknowledge
}

#define Direction_CHANNEL0      0 // use first channel of 16  
#define Speed_CHANNEL1       3
#define MotorPIN    21
#define MotorPIN_Direction 22
#define ServoPIN    33
#define MotorFREQ    5000
#define ServoFREQ    60
#define LEDC_RESOLUTION_BITS  12
#define LEDC_RESOLUTION  ((1<<LEDC_RESOLUTION_BITS)-1) 
#define servoMin 0.045
#define servoMax 0.08
#define motorMin 0
#define motorMax 1


//initialize neccesary params
int SpeedMotor, DirectionServo,DirectionMotor;
// finds the duty cycle given the maximum and minimum allowed duty cycle
float FindDuty(int Value, float Min,float Max){
  int duty=Value;
  int Minimum=duty*Min;
  int Maximum=duty*Max;
  if(duty>=Maximum){
    duty=Maximum;
  }
  else if(duty<=Minimum){
     duty=Minimum;
  }
  return duty;
  }

void updateMotors() {
 ledcWrite(Speed_CHANNEL1,FindDuty(SpeedMotor, motorMin,motorMax));  // write duty to LEDC 
 ledcWrite(Direction_CHANNEL0,FindDuty(DirectionServo,servoMin,servoMax));  // write duty to LEDC 

  if(DirectionMotor<0){
    digitalWrite(MotorPIN_Direction, LOW);
  }
  else if(DirectionMotor>=0){
    digitalWrite(MotorPIN_Direction, HIGH);
  }
  

}

/************************/
/* joystick mode  code  */

int x,y;

void handleJoy() {
  int Speed, Direction;
  x = getVal(); // from -50 to +50
  y = getVal();
  String s = String(x) + "," + String(y);

  Speed = y;
  Direction = x;
 //determines the speed of the motor
  SpeedMotor = map(Speed, -50, 50, -1*LEDC_RESOLUTION, LEDC_RESOLUTION);
   if(SpeedMotor>=0){
  DirectionMotor=-1;
  }
  else{
    DirectionMotor=1;
    SpeedMotor=SpeedMotor*-1;
  }
  //determines direction of servor
  DirectionServo = map(Direction, -50, 50, 0, LEDC_RESOLUTION); 

  sendplain(s);
  Serial.printf("received X,Y:=%d,%d\n",x,y); 
}



void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  //Upenn wifi
  /*
  WiFi.config(IPAddress(192, 168, 43, 109), // change the last number to your assigned number
              IPAddress(192, 168, 43, 1),
              IPAddress(255, 255, 255, 0));
*/
  
  //home Wifi
  
 WiFi.config(IPAddress(192, 168, 1, 109),
              IPAddress(192, 168, 1, 1),
              IPAddress(255, 255, 255, 0));

            
  while(WiFi.status()!= WL_CONNECTED ) { 
    delay(500); Serial.print("."); 
  }
  Serial.println("WiFi connected"); 
  Serial.printf("Use this URL http://%s/\n",WiFi.localIP().toString().c_str());
  server.begin();                  //Start server

 // Motor initialization
  pinMode(MotorPIN_Direction , OUTPUT);
  pinMode(MotorPIN,OUTPUT);
  pinMode(ServoPIN,OUTPUT);
  ledcSetup(Direction_CHANNEL0, ServoFREQ, LEDC_RESOLUTION_BITS); // channel, freq, bits
  ledcAttachPin(ServoPIN, Direction_CHANNEL0);
  ledcSetup(Speed_CHANNEL1, MotorFREQ, LEDC_RESOLUTION_BITS); // channel, freq, bits
  ledcAttachPin(MotorPIN, Speed_CHANNEL1);
  
  

 // HTML510 initialization
  attachHandler("/joy?val=",handleJoy);
  body = joybody;
  attachHandler("/favicon.ico",handleFavicon);
  attachHandler("/ ",handleRoot);
}

void loop()
{ 
  static long lastWebCheck = millis();
  static long lastMotorUpdate = millis();
  uint32_t ms;

  ms = millis();
  if (ms-lastWebCheck > 2){ 
    serve(server,body);    
    lastWebCheck = ms;
  }
  if (ms-lastMotorUpdate > 1000/MotorFREQ) {
    updateMotors();
    lastMotorUpdate = ms;
  }
}
