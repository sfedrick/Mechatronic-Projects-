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
 char AutonomousState[30]="False";
 int auton=0;
 
 int reset=0;
 int forward=0;
 int right=0;
 int clockwiserotate=0;
 int motorspeedPercent=0;
 int SensorState=0;
 int GripperState=0;
 int OnwallState=0;
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
  OnwallState=getVal();
  
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
  String s=String(reset)+","+String(auton)+","+AutonomousState+",";
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
  
  
}


//define end condition variable
int onwall_init1=0;
bool OnwallendCondition=false;
bool onWallLoop(){
  if(onwall_init1>100){
    return true;
    Serial.println("finished");
    strcpy(AutonomousState,"finished");
  }
  else{
  Serial.println("I will be in here for 5 secs");
    Serial.println("On wall State: "+String(OnwallState)+" "+ String(onwall_init1));
    strcpy(AutonomousState,"onwall state iteration");
  onwall_init1+=1;
   return false;}
  
}

void loop() {
  // put your main code here, to run repeatedly:

serve(server, body);

//autonomous state onwall  
  while(OnwallState==1 && reset!=1 && !OnwallendCondition){
      serve(server, body);
    //perform on wall state autonomous loop  
      bool Endit=onWallLoop();
    //end condition
       if(Endit){
        reset=1;
       }
    }
    if(OnwallState==0){
      //reset initial conditions
       onwall_init1=0;
    }

 if(auton!=1){
  //perform manual actions
  strcpy(AutonomousState,"Manual Mode");
 }

}
