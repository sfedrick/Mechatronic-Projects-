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

#define MotorFrequency 20000
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


const char* ssid     = "Fios-G8WhZ";
const char* password = "Jesuisungrenouille777";

//update rates

int servoUpdateRate = 1000; //update rate of servo in microseconds
int tofUpdateRate = 100000; //update rate of tof in microseconds
int motorUpdateRate = 100; //update rate of motor in microseconds
int positionUpdateRate = 100; //update rate at which we calculate position in microseconds
int serverUpdateRate = 5000; //update the server
float servolimit = 0.15; // duty cycle of servo to usable range

//Esp32 Status
unsigned long StartTime = micros();
unsigned long ServoUpdate = micros();
unsigned long TOFUpdate = micros();
unsigned long MotorUpdate = micros();
unsigned long PositionUpdate = micros();
unsigned long ServerUpdate = micros();
unsigned long recieveUpdate = micros();
char AutonomousState[30] = "False";

int positionX = -1;
int positionY = -1;
int orientationTheta = -1;
int TOF = -1;
bool SensorFailure = true;
bool ResetServo = false;
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
#define SERVO_SCAN       12   // can increase this to have wider sweep

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
int forward = 0;
int right = 0;
int clockwiserotate = 0;
int motorspeedPercent = 0;
int SensorState = 0;
int GripperState = 0;
int OnwallState = 0;
int ScanState = 0;



void RecieveState() {


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


  motorspeedPercent = Sconstrain(motorspeedPercent, 0, 100);
  SensorState = Sconstrain(SensorState, -50, 50);
  GripperState = Sconstrain(GripperState, 0, 100);

  String Direction = "[ X:" + String(right * motorspeedPercent) + "; Y:" + String(forward * motorspeedPercent) + "; Rotation:" + String(clockwiserotate * motorspeedPercent) + "]";
  String Speed = String( motorspeedPercent);
  String Sensor = String(SensorState);
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




void setMotorDirection() {
  int vx = forward;
  int vy = right;
  int w = -clockwiserotate;

  w1 = (vx + vy - w);
  w2 = (vx - vy + w);
  w3 = (vx - vy - w);
  w4 = (vx + vy + w);

  w1 = Sconstrain(w1, -1, 1);
  w2 = Sconstrain(w2, -1, 1);
  w3 = Sconstrain(w3, -1, 1);
  w4 = Sconstrain(w4, -1, 1);

}

//recode servo
void SetServoDirection(int s, int servonumber) {


}




//autonomous functions
//define end condition variable
int onwall_init1 = 0;
bool OnwallendCondition = false;

bool onWallLoop() {
  if (onwall_init1 > 100) {

    return true;
    strcpy(AutonomousState, "finished");
  }
  else {
    strcpy(AutonomousState, "onwall state iteration");
    delay(100);
    onwall_init1 += 1;
    return false;
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
  WiFi.config(IPAddress(192, 168, 1, 109),
              IPAddress(192, 168, 1, 1),
              IPAddress(255, 255, 255, 0));
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


bool Endit;
void loop() {
  if(reset==1){
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
      reset = 1;
    }
  }
  if (OnwallState == 0) {
    //reset initial conditions
    onwall_init1 = 0;
    OnwallendCondition = false;
  }


  if (auton != 1) {
    //perform manual actions
    strcpy(AutonomousState, "Manual Mode");
    setMotorDirection();
    if (StartTime - MotorUpdate > motorUpdateRate) {
      MotorUpdate = StartTime;
      SetMotorSpeed(motorspeedPercent * w1, 0);
      SetMotorSpeed(motorspeedPercent * w2, 1);
      SetMotorSpeed(motorspeedPercent * w3, 2);
      SetMotorSpeed(motorspeedPercent * w4, 3);
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
