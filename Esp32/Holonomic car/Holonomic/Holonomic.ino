#include <WiFi.h>
#include "html510.h"
#include "Gamepad.h"
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

const char* ssid     = "Fios-G8WhZ";
const char* password = "Jesuisungrenouille777";
WiFiServer server(80);



/* web handler   */
void handleRoot(){
  sendhtml(body);
}

int Sconstrain(int X,int a,int b){
  if(X<a){
    return a;
  }
  else if( X>b){
    return b;
  }
  else{
    return X;
  }
}

//Esp32 Status
 char AutomousState[20]="False";
 int auton=0;
 
 int reset=0;
 int forward=0;
 int right=0;
 int clockwiserotate=0;
 int motorspeedPercent=0;
 int SensorState=0;
 int GripperState=0;

//autonmous states
 void RecieveState(){
  reset=getVal();
  auton=getVal();
  forward=getVal();
  right=getVal();
  clockwiserotate=getVal();
  motorspeedPercent=motorspeedPercent+getVal();
  SensorState= SensorState+getVal();
  GripperState=GripperState+getVal();
  
  motorspeedPercent=Sconstrain(motorspeedPercent,0,100);
  SensorState=Sconstrain(SensorState,0,100);
  GripperState=Sconstrain(GripperState,0,100);
  
  String Direction= "[ X:"+String(right*motorspeedPercent)+"; Y:"+String(forward*motorspeedPercent)+"; Rotation:"+String(clockwiserotate*motorspeedPercent)+"]";
  String Speed=String( motorspeedPercent);
  String Sensor=String(SensorState);
  String Gripper=String(GripperState);
  String s=Direction+","+Speed+","+Sensor+","+Gripper+",";
  sendplain(s);
  Serial.println("New Values");
  Serial.println(Direction);
  Serial.println(Speed);
  Serial.println(Sensor);
  Serial.println(Gripper);
  
 }

void CheckState(){
  String s="test1,test2,test3,test4";
  sendplain(s);
  
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
  Serial.println("WiFi connected"); 
  server.begin();

  Serial.begin(115200);
 //states of holonomic car
  
  attachHandler("/Orders?val=",RecieveState);
  attachHandler("/Check?val=",CheckState);
  attachHandler("/ ",handleRoot);
  
  delay(2000);
}
int testauton=0;
void loop() {
  // put your main code here, to run repeatedly:
 
serve(server, body);
/*
if(auton==0){
 while(auton==0 && reset!=1){
  if(testauton>100){
    auton=0;
  }
  serve(server, body);
  strcpy(AutnomousState, "TestAuton"+String(testauton));  
 }
}
*/
 
}
