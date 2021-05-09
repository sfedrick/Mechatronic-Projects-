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



#define MotorDirection1 9
#define MotorDirection2 26
#define MotorDirection3 27
#define MotorDirection4 14

#define MotorFrequency 5000
#define MotorResolutionBits 14
#define MotorResolution ((1<<MotorResolutionBits)-1)  // this is 2^MotorResolutionBits -1
#define Touch1 33
#define Touch2 33

#define TOF_ready 12
#define TOF_on 13
#define XSHUT_TOF 10


#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int MotorSpeeds[4] = { MotorSpeed1, MotorSpeed2, MotorSpeed3, MotorSpeed4};
int MotorDirections[4] = {MotorDirection1, MotorDirection2, MotorDirection3, MotorDirection4};
int MotorChannels[4] = {MotorChannel1, MotorChannel2, MotorChannel3, MotorChannel4};

//tester 
/*
const char* ssid     = "Tester";
const char* password = "12345678";
*/
//home
const char* ssid     = "Fios-G8WhZ";
const char* password = "Jesuisungrenouille777";

//upenn

/*
const char* ssid     = "#Skyroam_1t9";
const char* password = "55687127";

*/

//update rates

int servoUpdateRate = 1000; //update rate of servo in microseconds
int tofUpdateRate = 100000; //update rate of tof in microseconds
int motorUpdateRate = 100; //update rate of motor in microseconds
int positionUpdateRate = 100; //update rate at which we calculate position in microseconds
int serverUpdateRate = 5000; //update the server


//Esp32 Status
unsigned long StartTime = micros();
unsigned long ServoUpdate = micros();
unsigned long TOFUpdate = micros();
unsigned long MotorUpdate = micros();
unsigned long PositionUpdate = micros();
unsigned long ServerUpdate = micros();
unsigned long recieveUpdate = micros();
char AutonomousState[30] = "False";

float positionX = -1;
float positionY = -1;
int orientationTheta = 90;
int orientationIteration=0;
int TOF = -1;
bool SensorFailure = true;

int ChangeSensor;
int ChangeGripper;
int w1 = 0;
int w2 = 0;
int w3 = 0;
int w4 = 0;

//scanning code from MarkYim
#define LEDC_CHANNEL       2 // use first channel of 16  
#define LEDC_RESOLUTION_BITS  14
#define LEDC_RESOLUTION  ((1<<LEDC_RESOLUTION_BITS)-1)
#define LEDC_FREQ_HZ     60
#define SERVO_PIN        4
#define SERVO_SCAN       10  // can increase this to have wider sweep

#define SERVO_PIN2   0
#define LEDC_CHANNEL2 1


#define FULLBACK  LEDC_RESOLUTION*(15-SERVO_SCAN)*60/10000
#define SERVOOFF  LEDC_RESOLUTION*15*60/10000   // center servo (1.5ms pulse)
#define FULLFRONT LEDC_RESOLUTION*(15+SERVO_SCAN)*60/10000
#define SERVOTODEG(x) ((x)*(int8_t)90*10000)/(12*LEDC_RESOLUTION*60)


void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  uint32_t duty =  LEDC_RESOLUTION * min(value, valueMax) / valueMax;
  ledcWrite(channel, duty);  // write duty to LEDC
}






//****************************
//********* Scanning stuff:
//****************************
#define SAMPLEFREQ 100   // TOF can use 30, Ultrasonic maybe 15
#define SCANSPEED 40
#define SCANSIZE 45
#define ARRAYMAX SCANSIZE*2 // needs to be bigger than scansize
int scanR[ARRAYMAX];
int scanA[ARRAYMAX];
int scanoffset = SCANSIZE; // start at SCANSIZE so we don't endup negative mod

int angle;
int dir = SCANSPEED;


void scanStep(int range) {


  if (angle + SERVOOFF > FULLFRONT) {
    dir = -SCANSPEED;
  }
  if (angle + SERVOOFF < FULLBACK) {
    dir = SCANSPEED;
    //    printScan();
  }
  scanR[scanoffset % ARRAYMAX] = range;
  scanA[scanoffset % ARRAYMAX] = -SERVOTODEG(angle);
  scanoffset++;
  angle += dir;
  ledcAnalogWrite(LEDC_CHANNEL, SERVOOFF + angle, LEDC_RESOLUTION);

}


//attach handler for scanning
void handleUpdate() {
  String s = "";

  s = s + SCANSIZE;                // first number is number of data pairs
  for (int i = 0; i < SCANSIZE; i++) { // add range values
    s = s + "," + scanR[(scanoffset - i) % ARRAYMAX]; // range sensor lags angle by 1 step
  }
  for (int i = 0; i < SCANSIZE; i++) { // add angle value
    s = s + "," + scanA[(scanoffset - i - 1) % ARRAYMAX];
  }

  sendplain(s);
}




WiFiServer server(80);


int rangeToF () {
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
void handleRoot() {
  sendhtml(body);
}

int Sconstrain(int X, int a, int b) {
  if (X < a) {
    return a;
  }
  else if ( X > b) {
    return b;
  }
  else {
    return X;
  }
}

void UpdatePosition(int dt) {
  int speedStart = 30;
  //motorspeedPercent;
}


//Esp32 recieved states in order of get val
int auton = 0;
int reset = 0;
int hardreset=0;
int forward = 0;
int right = 0;
int clockwiserotate = 0;
int motorspeedPercent = 0;
int SensorState = 0;
int GripperState = 100;
int OnwallState = 0;
int ScanState = 0;
int CWRotate90=0;



void RecieveState() {

  hardreset=getVal();
  reset = getVal();
  auton = getVal();

  forward = getVal();
  right = getVal();
  clockwiserotate = getVal();

  motorspeedPercent = motorspeedPercent + getVal();
  //sensor state is backwards and i didn't feel like fixing the javascript
  SensorState = SensorState + getVal();
  GripperState = GripperState + getVal();
  OnwallState = getVal();
  ScanState = getVal();
  CWRotate90=getVal();


  motorspeedPercent = Sconstrain(motorspeedPercent, 50, 100);
  SensorState = Sconstrain(SensorState, -50, 50);
  GripperState = Sconstrain(GripperState, 0, 100);

  String Direction = "[ X:" + String(right * motorspeedPercent) + "; Y:" + String(forward * motorspeedPercent) + "; Rotation:" + String(clockwiserotate * motorspeedPercent) + "]";
  //align it at zero
  String Speed = String( motorspeedPercent);
  String Sensor = String((-1*SensorState));
  String Gripper = String(GripperState);
  String s = Direction + "," + Speed + "," + Sensor + "," + Gripper + ",";
  sendplain(s);


}

void CheckState() {
  String s = String(reset) + "," + String(auton) + "," + AutonomousState + "," + String(positionX) + \
             "," + String(positionY) + "," + String(orientationTheta) + "," + String(TOF) + ",";

  sendplain(s);

}
// sets the spend of the motor
void SetMotorSpeed(int m, int motornumber) {
  int duty = map(m, -100, 100, -MotorResolution, MotorResolution);

  if (duty < 0) {
    duty = duty * -1;
    digitalWrite(MotorDirections[motornumber], LOW);
  }
  else if (duty >= 0) {
    digitalWrite(MotorDirections[motornumber], HIGH);
  }


  // ledcWrite(MotorChannels[motornumber], duty);  // write duty to LEDC
  ledcAnalogWrite(MotorChannels[motornumber], duty, MotorResolution);
}



void setMotorDirection(int y, int x ,int cw) {
  int vx = y;
  int vy = x;
  int w = -cw;

  w1 = (vx + vy - w);
  w2 = (vx - vy + w);
  w3 = (vx - vy - w);
  w4 = (vx + vy + w);

  w1 = Sconstrain(w1, -1, 1);
  w2 = Sconstrain(w2, -1, 1);
  w3 = Sconstrain(w3, -1, 1);
  w4 = Sconstrain(w4, -1, 1);

}






//autonomous functions
//define end condition variable
int onwall_init1 = 0;
int onwall_init2 = 0;
bool OnwallendCondition = false;
int rightWall=0;
bool onWallLoop() {
  if(SensorState<0){
     rightWall=1;
  }
  else{
    rightWall=-1;
  }
 TOF=rangeToF();
 
  if ((TOF>83) && onwall_init1==0) {
      onwall_init2+=1;
      setMotorDirection(0,rightWall,0);
      SetMotorSpeed(100* w1, 0);
      SetMotorSpeed(100* w2, 1);
      SetMotorSpeed(100* w3, 2);
      SetMotorSpeed(100* w4, 3);
      delay(250);
      setMotorDirection(0,0,0);
      SetMotorSpeed(0, 0);
      SetMotorSpeed(0, 1);
      SetMotorSpeed(0, 2);
      SetMotorSpeed(0, 3);
      return false;   
      strcpy(AutonomousState, "Getting on Wall"); 
  }
  else if((TOF==7912 || TOF==96)|| onwall_init2>15){
    strcpy(AutonomousState, "Sensor Broke"); 
     return true;
  }
  else if(TOF>2000 || TOF==0){
    strcpy(AutonomousState, "TOO far from wall"); 
     return true;
  }
  else{ 
      strcpy(AutonomousState, "Getting on Wall"); 
    //prevents double entering the loop
    strcpy(AutonomousState, "On Wall"); 
    delay(500);
    return true;
  }

}


//
int rotate_init = 0;
bool RotateEnd90 = false;


//rotate car 90 degrees
bool RotateLoop(int rotateDir) {
  if(rotate_init==0){
      strcpy(AutonomousState, "running");
      delay(100);
      setMotorDirection(0,0,CWRotate90);
      SetMotorSpeed(100* w1, 0);
      SetMotorSpeed(100* w2, 1);
      SetMotorSpeed(100* w3, 2);
      SetMotorSpeed(100* w4, 3);
      rotate_init += 1;
      delay(250);
      setMotorDirection(0,0,0);
      SetMotorSpeed(0, 0);
      SetMotorSpeed(0, 1);
      SetMotorSpeed(0, 2);
      SetMotorSpeed(0, 3);
      rotate_init=1;
      //record the rotation
      orientationTheta =(orientationTheta-rotateDir*90)%360;
      return true;
  }
  else{
      return true;
      strcpy(AutonomousState, "Prevented Double Tap");
  }
  }




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  int failureCount = 0;

   pinMode(XSHUT_TOF, OUTPUT);
  digitalWrite(XSHUT_TOF,HIGH);
  while (SensorFailure && !lox.begin(0x53) && failureCount < 3) {
    failureCount += 1;
    if (lox.begin(0x53)) {
      SensorFailure = false;
    }
    delay(2000);
    if (failureCount > 3) {
      //ESP.restart();
      Serial.println("Sensor not found restarting");
    }
    Serial.println("Sensor not found");
  }
    WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);

//Mobile hotspot 
/*
 WiFi.config(IPAddress(192, 168, 43, 109), // change the last number to your assigned number
              IPAddress(192, 168, 43, 1),
              IPAddress(255, 255, 255, 0));

*/

  //home wifi 
  

 WiFi.config(IPAddress(192, 168, 1, 109), // default gateway change the last number to your assigned number
              IPAddress(192, 168, 1, 1), //default gateway
              IPAddress(255, 255, 255, 0));//subnet mask

              



//upenn wifi
/*

 WiFi.config(IPAddress(192, 168, 43, 109),
              IPAddress(192, 168, 43, 1),
              IPAddress(255, 255, 255, 0));
*/
             
  failureCount = 0;
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
    failureCount += 1;
    if (failureCount > 25) {
      ESP.restart();
    }

  }
  Serial.println("WiFi connected");
  server.begin();


  //set up motors
  int mnum;

  for (mnum = 0; mnum < 4; mnum = mnum + 1) {
    //motor pins
    pinMode(MotorSpeeds[mnum], OUTPUT);
    //sets direction pin
    pinMode(MotorDirections[mnum], OUTPUT);
    ledcAttachPin(MotorSpeeds[mnum], MotorChannels[mnum]);
    ledcSetup(MotorChannels[mnum], MotorFrequency, MotorResolutionBits);
  }
  //set up servos

  //set up sensor servor
  ledcSetup(LEDC_CHANNEL, 60, LEDC_RESOLUTION_BITS); // channel, freq, bits
  ledcAttachPin(SERVO_PIN, LEDC_CHANNEL);
  ledcAnalogWrite(LEDC_CHANNEL, SERVOOFF, LEDC_RESOLUTION);

  //set up gripper servo
  ledcSetup(LEDC_CHANNEL2, 60, LEDC_RESOLUTION_BITS); // channel, freq, bits
  ledcAttachPin(SERVO_PIN2, LEDC_CHANNEL2);
  ledcAnalogWrite(LEDC_CHANNEL2, SERVOOFF, LEDC_RESOLUTION);




  //states of holonomic car

  attachHandler("/Orders?val=", RecieveState);
  attachHandler("/Check?val=", CheckState);
  attachHandler("/up", handleUpdate);
  attachHandler("/ ", handleRoot);


}

//reinitialize antonomous functions
bool Endit;
bool ResetServo = false;
void loop() {
  if(hardreset){
    digitalWrite(XSHUT_TOF,LOW);
    delay(10);
     digitalWrite(XSHUT_TOF,HIGH);
     ESP.restart();
  }

  // put your main code here, to run repeatedly:
  static uint32_t lastScan = micros();
  StartTime = micros();


  if (StartTime - ServerUpdate > serverUpdateRate) {
    ServerUpdate = StartTime;
    serve(server, body);

  }

  //autonomous state onwall
  while (OnwallState == 1 && reset != 1) {
    serve(server, body);
    //perform on wall state autonomous loop
    OnwallendCondition = onWallLoop();
    //end condition
   
    if (OnwallendCondition) {
      if(onwall_init1==0){
      ledcAnalogWrite(LEDC_CHANNEL, SERVOOFF, LEDC_RESOLUTION);
      delay(2);
      setMotorDirection(0,-rightWall,0);
      SetMotorSpeed(100* w1, 0);
      SetMotorSpeed(100* w2, 1);
      SetMotorSpeed(100* w3, 2);
      SetMotorSpeed(100* w4, 3);
      delay(150);
      setMotorDirection(0,0,0);
      SetMotorSpeed(0, 0);
      SetMotorSpeed(0, 1);
      SetMotorSpeed(0, 2);
      SetMotorSpeed(0, 3);
      }
       onwall_init1=1;
      
       angle = 0;
       reset = 1;
       int orientationConeP=(orientationTheta+45)%360;
       int orientationConeN=(orientationTheta-45)%360;
       if( orientationConeN<0){
         orientationConeN= orientationConeN+360;
       }
      if(orientationConeP<=135 && orientationConeN>=45  ){
      orientationTheta=90;
       }
       else if (orientationConeP<=315 && orientationConeN>=225){
        orientationTheta=270;
       }
       else if(orientationConeP<=225 &&  orientationConeN>=135){
        orientationTheta=180;
       }
       else{
        orientationTheta=0;
       }
        
       }
  }
  if (OnwallState == 0) {
  
    //reset initial conditions
    onwall_init1 = 0;
    onwall_init2 = 0;
    OnwallendCondition = false;
   
  
  }

  while((CWRotate90== 1 || CWRotate90== -1) && reset != 1) {
    serve(server, body);
    //perform rotate 
    //end condition
    RotateEnd90=RotateLoop(CWRotate90);

    if (RotateEnd90) {
      reset = 1;
      serve(server, body);
    }
  }
  if (CWRotate90 == 0) {
    //reset initial conditions
    rotate_init = 0;
    RotateEnd90 = false;
  }


  if (auton != 1) {
    //perform manual actions
    
    setMotorDirection(forward,right,clockwiserotate);
    if (StartTime - MotorUpdate > motorUpdateRate) {
      MotorUpdate = StartTime;
      SetMotorSpeed(motorspeedPercent * w1, 0);
      SetMotorSpeed(motorspeedPercent * w2, 1);
      SetMotorSpeed(motorspeedPercent * w3, 2);
      SetMotorSpeed(motorspeedPercent * w4, 3);
      orientationIteration=orientationIteration-clockwiserotate*motorspeedPercent;
      if(orientationIteration>12500){
        //add 5 degrees of orientation
        orientationTheta = (orientationTheta+5)%360;
        orientationIteration=0;
      }
      else if(orientationIteration<-12500){
        orientationTheta = (orientationTheta-5)%360; 
        orientationIteration=0;
         if(orientationTheta<0){
          orientationTheta=orientationTheta+360;
        }
        
      }
      

    }

    if (StartTime - ServoUpdate > servoUpdateRate && ScanState == 0) {
      ServoUpdate = StartTime;
      //remake servos
      ChangeSensor = map(SensorState, -50, 50, FULLBACK, FULLFRONT);
      ledcAnalogWrite(LEDC_CHANNEL, ChangeSensor, LEDC_RESOLUTION);
      ChangeGripper=map(GripperState, 0, 100, FULLBACK, FULLFRONT);
      ledcAnalogWrite(LEDC_CHANNEL2, ChangeGripper, LEDC_RESOLUTION);
    }
    if (StartTime - TOFUpdate > tofUpdateRate && ScanState == 0) {
      TOFUpdate = StartTime;
      TOF = rangeToF();

    }

    if (StartTime - lastScan > 1000000 / SAMPLEFREQ && ScanState == 1) { // update the servo position
      lastScan = StartTime;
      if (!ResetServo) {
        angle = 0;
        ledcAnalogWrite(LEDC_CHANNEL, SERVOOFF, LEDC_RESOLUTION);
        ResetServo = true;
      }
      else {
        TOF = rangeToF();     // uncomment if using ToF sensor
        scanStep(TOF);
      }
    }

    if (ResetServo && ScanState == 0) {
      ResetServo = false;
      angle = 0;
      ledcAnalogWrite(LEDC_CHANNEL, SERVOOFF, LEDC_RESOLUTION);
      SensorState = 0;
    }
  }



}
