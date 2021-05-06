#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();


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

void setup() {
   Serial.begin(115200); 
  if (!lox.begin(0x53)) { // for debugging use lox.begin(0x29,true), if device
    Serial.println(F("Failed to boot VL53L0X"));
    delay(2000);
  }  
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
int TOF = rangeToF();
 
  Serial.println(TOF);
  delay(100);


  
}
