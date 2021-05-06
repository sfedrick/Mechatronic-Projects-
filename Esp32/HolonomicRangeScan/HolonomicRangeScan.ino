#include <WiFi.h>
#include "html510.h"
#include "Gamepad.h"
#define MotorSpeed1 2
#define MotorSpeed2 15
#define MotorSpeed3 1
#define MotorSpeed4 3

#define MotorChannel1 0
#define MotorChannel2 3
#define MotorChannel3 7
#define MotorChannel4 6

#define SERVOPIN1    4
#define SERVOPIN2    0

#define SERVOCHANNEL1 2
#define SERVOCHANNEL2 1

#define MotorDirection1 9
#define MotorDirection2 26
#define MotorDirection3 27
#define MotorDirection4 14

#define MotorFrequency 5000
#define MotorResolutionBits 8
#define MotorResolution 255 // this is 2^MotorResolutionBits -1
#define Touch1 33
#define Touch2 33

#define TOF_ready 12
#define TOF_on 13


#define SERVOFREQ    50
#define ServoResolutionBits 8
#define ServoResolution 255 

#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int MotorSpeeds[4]={ MotorSpeed1, MotorSpeed2, MotorSpeed3,MotorSpeed4};
int MotorDirections[4]={MotorDirection1,MotorDirection2,MotorDirection3,MotorDirection4};
int MotorChannels[4]={MotorChannel1,MotorChannel2,MotorChannel3,MotorChannel4};

int ServoPins[2]={SERVOPIN1,SERVOPIN2};
int ServoChannels[2]={SERVOCHANNEL1,SERVOCHANNEL2};

const char* ssid     = "Fios-G8WhZ";
const char* password = "Jesuisungrenouille777";

//update rates

int servoUpdateRate=1000; //update rate of servo in microseconds 
int tofUpdateRate=50000; //update rate of tof in microseconds
int motorUpdateRate=100;//update rate of motor in microseconds
int positionUpdateRate=100;//update rate at which we calculate position in microseconds
int serverUpdateRate=5000; //update the server 
float servolimit=0.15; // duty cycle of servo to usable range

//Esp32 Status
 unsigned long StartTime=micros();
 unsigned long ServoUpdate=micros();
 unsigned long TOFUpdate=micros();
 unsigned long MotorUpdate=micros();
 unsigned long PositionUpdate=micros();
 unsigned long ServerUpdate=micros();
 unsigned long recieveUpdate=micros();
 char AutonomousState[30]="False";

 int positionX=-1;
 int positionY=-1;
 int orientationTheta=-1;
 int TOF=-1;
 bool SensorFailure=true;


 int w1=0;
 int w2=0;
 int w3=0; 
 int w4=0;
 

 
 
// this is filled by scan variable
int compass[5]={0};

WiFiServer server(80);


int rangeToF (){
  VL53L0X_RangingMeasurementData_t measure;
  
  lox.rangingTest(&measure, false); // pass 'true' for debugging info 
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
  //Serial.printf("Distance (mm): %d\n",measure.RangeMilliMeter);
    return measure.RangeMilliMeter;
  }
//Serial.println(" out of range ");
  return 0;  // error, real values are never 0
}





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

void UpdatePosition(int dt){
  int speedStart=30;
  //motorspeedPercent;
}


 //Esp32 recieved states in order of get val
  int auton=0;
 int reset=0;
 int forward=0;
 int right=0;
 int clockwiserotate=0;
 int motorspeedPercent=0;
 int SensorState=0;
 int GripperState=0;
 int OnwallState=0;
 int ScanState=0;



 void RecieveState(){
  
  
  reset=getVal();
  auton=getVal();
  
  forward=getVal();
  right=getVal();
  clockwiserotate=getVal();
  
  motorspeedPercent=motorspeedPercent+getVal();
  //sensor state is backwards and i didn't feel like fixing the javascript
  SensorState= SensorState+getVal();
  GripperState=GripperState+getVal();
  OnwallState=getVal();
  ScanState=getVal();


  motorspeedPercent=Sconstrain(motorspeedPercent,50,100);
  SensorState=Sconstrain(SensorState,-50,50);
  GripperState=Sconstrain(GripperState,0,100);
  
  String Direction= "[ X:"+String(right*motorspeedPercent)+"; Y:"+String(forward*motorspeedPercent)+"; Rotation:"+String(clockwiserotate*motorspeedPercent)+"]";
  String Speed=String( motorspeedPercent);
  String Sensor=String(-1*SensorState);
  String Gripper=String(GripperState);
  String s=Direction+","+Speed+","+Sensor+","+Gripper+",";
  sendplain(s);

  
 }

void CheckState(){
  String s=String(reset)+","+String(auton)+","+AutonomousState+","+String(positionX)+\
  ","+String(positionY)+","+String(orientationTheta)+","+String(TOF)+","; 
    
  sendplain(s);
  
}
// sets the spend of the motor
void SetMotorSpeed(int m, int motornumber){
  int duty=map(m,-100,100,-MotorResolution,MotorResolution);

   if(duty<0){
    duty=duty*-1;
    digitalWrite(MotorDirections[motornumber], LOW);
  }
  else if(duty>=0){
    digitalWrite(MotorDirections[motornumber], HIGH);
  }
   
  
  ledcWrite(MotorChannels[motornumber], duty);  // write duty to LEDC 
  
}




void setMotorDirection(){
int vx=forward;
int vy=right;
int w=-clockwiserotate;

w1=(vx+vy-w);
w2=(vx-vy+w);
w3=(vx-vy-w);
w4=(vx+vy+w);

w1=Sconstrain(w1,-1,1);
w2=Sconstrain(w2,-1,1);
w3=Sconstrain(w3,-1,1);
w4=Sconstrain(w4,-1,1);

}


void SetServoDirection(int s, int servonumber){
  int duty=0;
  int limit=servolimit*float(ServoResolution);
  if(servonumber<1){
  duty=map(s,-50,50,0,limit);
  }
  else{
  duty=map(s,0,100,0,limit);
  }
  ledcWrite(ServoChannels[servonumber], duty);  // write duty to LEDC 
 
  
}




//autonomous functions 
//define end condition variable
int onwall_init1=0;
bool OnwallendCondition=false;

bool onWallLoop(){
  if(onwall_init1>100){
   
    return true;
    strcpy(AutonomousState,"finished");
  }
  else{
    strcpy(AutonomousState,"onwall state iteration");
     delay(100);
  onwall_init1+=1;
   return false;}
  
}




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  int failureCount=0;
  
  while (SensorFailure && !lox.begin(0x53)&&failureCount<3) { 
    failureCount+=1;
    if(lox.begin(0x53)){
      SensorFailure=false;
    }
    delay(2000);
    if(failureCount>3){
      //ESP.restart();
      Serial.println("Sensor not found restarting"); 
    }
 Serial.println("Sensor not found"); 
  }
                                   
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  WiFi.config(IPAddress(192, 168, 1, 109),
              IPAddress(192, 168, 1, 1),
              IPAddress(255, 255, 255, 0));
failureCount=0;
  while(WiFi.status()!= WL_CONNECTED ) { 
    delay(500);
    Serial.print(".");
    failureCount+=1;
    if(failureCount>25){
      ESP.restart();
    }
   
  }
  Serial.println("WiFi connected"); 
  server.begin();

  
 //set up motors
int mnum;

for(mnum=0;mnum<4;mnum=mnum+1){ 
  //motor pins
  pinMode(MotorSpeeds[mnum], OUTPUT);
  //sets direction pin
  pinMode(MotorDirections[mnum], OUTPUT); 
  ledcAttachPin(MotorSpeeds[mnum], MotorChannels[mnum]);
  ledcSetup(MotorChannels[mnum], MotorFrequency, MotorResolutionBits);
}
//set up servos
int ServoPins[2]={SERVOPIN1,SERVOPIN2};
int ServoChannels[2]={SERVOCHANNEL1,SERVOCHANNEL2};
int snum;
for(snum=0;snum<2;snum=snum+1){ 
  //motor pins
  pinMode(ServoPins[snum], OUTPUT);
  ledcAttachPin(ServoPins[snum], ServoChannels[snum]);
  ledcSetup(ServoChannels[snum], SERVOFREQ, ServoResolutionBits);
}

 //states of holonomic car
  
  attachHandler("/Orders?val=",RecieveState);
  attachHandler("/Check?val=",CheckState);
  attachHandler("/ ",handleRoot);

}


bool Endit;
void loop() {
  // put your main code here, to run repeatedly:
StartTime=micros();

    
if(StartTime-ServerUpdate>serverUpdateRate){
  ServerUpdate=StartTime;
  serve(server, body);

}

//autonomous state onwall  
  while(OnwallState==1 && reset!=1){
      serve(server, body);
    //perform on wall state autonomous loop  
      OnwallendCondition=onWallLoop();
    //end condition
       if(OnwallendCondition){
        reset=1;
       }
    }
    if(OnwallState==0){
      //reset initial conditions
       onwall_init1=0;
       OnwallendCondition=false;
    }


 if(auton!=1){
  //perform manual actions
      strcpy(AutonomousState,"Manual Mode");
      setMotorDirection();
    if(StartTime-MotorUpdate>motorUpdateRate){
        MotorUpdate=StartTime;
        SetMotorSpeed(motorspeedPercent*w1, 0);
        SetMotorSpeed(motorspeedPercent*w2, 1);
        SetMotorSpeed(motorspeedPercent*w3, 2);
        SetMotorSpeed(motorspeedPercent*w4, 3); }
    
    if(StartTime-ServoUpdate>servoUpdateRate){
      ServoUpdate=StartTime;
      SetServoDirection(SensorState,0);
       SetServoDirection(GripperState,1);
    }
    if(StartTime-TOFUpdate>tofUpdateRate){
      TOFUpdate=StartTime;
      TOF = rangeToF();
     
     }

   
}



}
