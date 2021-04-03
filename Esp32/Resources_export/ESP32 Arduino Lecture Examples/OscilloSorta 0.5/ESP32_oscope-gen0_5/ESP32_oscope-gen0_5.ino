
/*
 * Wireless 2 ch oscilloscope - on ESP32
 * Input pins CHANNEL1 is GPIO-36, CHANNEL2 is GPIO-39, 
 * Square wave and sine wave output on GPIO-32
 * 
 * Analog mode 12bit 6KHz max bandwidth (limited by ADC)
 * Digital mode (3.3V logic) max bandwidth ~300KHz (limited by TCP)
 * features: horiz, vert (gnd) adj, 10uSdiv to 1S/div, square wave generator up to 20K, autoset (buggy)
 * 
 * Uses javascript and AJAX to do all display work.
 * (c) all rights reserved Mark Yim
 * December 31,2020
 * 
 * Version 0.5 March 25, 2021
 * - cleaned up TCP interface, should not have drops.
 * - fixed some state bugs
 */

#include <WiFi.h>
#include <WiFiClient.h>
#include "index.h"  //Web page header file
#include "html510.h"

#include "cosineGenerator.h"
#include "driver/dac.h"

WiFiServer server(80);
const char* ssid  = "OscilloSorta";

// scope display variables

#define XRANGE 400
#define YRANGE 500
#define TIMEDIVPERSCREEN    10
#define USECPERSCREEN   (TIMEDIVPERSCREEN*usecsPerDiv)
#define mVREF     3300
#define ADCRES     4096  // ADC is 12 bit (4096 values)
#define ADCcountPerV   1000*ADCRES/mVREF  
#define ACCOUPLETAU     800
#define DATAPTS 1000
int16_t x1[DATAPTS];
int16_t x2[DATAPTS];
uint32_t y[DATAPTS];
short indx;
uint32_t lastsweep=0;
int resolution=250;
int currentCh=1;
uint32_t usecsPerDiv = 1000;   // 10 divs per screen -> usecsPerScreen/10
int mvPerDiv[] = {500,500};      // 8 divs per screen   -> mvoltsPerScreen/8
int ground[] = {XRANGE/2,XRANGE/2};  // relative to screen 0 is middle of screen 
int xoffset[2] ; 
bool chOn[2] = {1,0} ;
bool ACcouplingOn[2] ;
bool fallingEdge[2]; 
bool digitalmode[2];
int ADCpin[2] = {36, 39};
const uint16_t VDial[] = {10,20,50,100,200,500,1000,2000,5000};
int iVDial=5;
const uint32_t TDial[] = {2,5,10,20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000};
int iTDial=8;


/**************************************/ 
/* function generator specific rounties */

#define SQUARE 0
#define SINE 1
#define LEDC_CHANNEL       0 // use first channel of 16  
#define LEDC_RESOLUTION_BITS  8
#define LEDC_RESOLUTION  ((1<<LEDC_RESOLUTION_BITS)-1) 
#define FG_PIN           25           // use either 25 or 26
#define DAC_CHANNEL    DAC_CHANNEL_1  // DAC_CHANNEL_1 is for FG_PIN2=25, DAC_CHANNEL_2 for 26

int testfreq=100, Duty=50, waveform;
int sinePrecision=8;
int sineScale =1;
int sineOffset=0;

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {            
  uint32_t duty =  LEDC_RESOLUTION * min(value, valueMax) / valueMax;   
  ledcWrite(channel, duty);  // write duty to LEDC 
}

void handleTestFq() {
  testfreq = getVal();

  if (waveform == SQUARE) {
    ledcSetup(LEDC_CHANNEL, testfreq, LEDC_RESOLUTION_BITS); // channel, freq, bits
    ledcAttachPin(FG_PIN, LEDC_CHANNEL);       // also sets pinMode to output
    ledcAnalogWrite(LEDC_CHANNEL, (Duty*255)/100, 255); // 50% duty cycle
  }
  else if (waveform == SINE) {
     dac_CW_set( testfreq,sinePrecision);
  }
  sendplain(""); // acknowledge
}

void handleDuty() {
  Duty = getVal();
  if (waveform == SQUARE)
    ledcAnalogWrite(LEDC_CHANNEL, (Duty*255)/100, 255); 
  sendplain(""); // acknowledge
}

void handleSineOffset() {
  int8_t val = getVal();
  if (waveform == SINE) {
     dac_offset_set(DAC_CHANNEL, val);
     sineOffset = val;
  }
  sendplain(""); // acknowledge
}

void handleSineScale() {
  uint8_t val = getVal();
  if (waveform == SINE && (val < 4)) {
     dac_scale_set(DAC_CHANNEL, val);
     sineScale = val;
  }
  sendplain(""); // acknowledge
}

void handleSinePrecision() {
  uint8_t val = getVal();
  if (waveform == SINE && (val < 9)) {
    dac_CW_set(testfreq,sinePrecision);
    sinePrecision = val;
  }
  sendplain(""); // acknowledge
}

void handleSine() { // turn on sine wave (off square)
  ledcAnalogWrite(LEDC_CHANNEL, 0, 255); // give 0 duty
  ledcDetachPin(FG_PIN);
  pinMode(FG_PIN,INPUT);
  dac_cosine_enable(DAC_CHANNEL);
  dac_output_enable(DAC_CHANNEL);
  dac_scale_set(DAC_CHANNEL, sineScale);
  dac_CW_set(testfreq,sinePrecision);
  dac_offset_set(DAC_CHANNEL, sineOffset);
  waveform = SINE;
  sendplain(""); // acknowledge
}

void handleSquare() { // turn on square wave (off sine)
  dac_output_disable(DAC_CHANNEL);
  ledcSetup(LEDC_CHANNEL, testfreq, LEDC_RESOLUTION_BITS); // channel, freq, bits
  ledcAttachPin(FG_PIN, LEDC_CHANNEL);  // sets pinmode OUTPUT)
  ledcAnalogWrite(LEDC_CHANNEL, (255*Duty)/100, 255); // 50% duty cycle
  waveform = SQUARE;
  sendplain(""); // acknowledge
}

/* end of function generator specific routines */
/***********************************************/ 


/****************************/ 
/* Oscope specific routines */

void handleChannel() {  
  currentCh = getVal();
  String s = String(ground[currentCh])+","+String(ACcouplingOn[0]+2*ACcouplingOn[1]);
  sendhtml(s);
  Serial.printf("select channel %d\n", currentCh);
}

void handleRun() {
  int val = getVal();
  switch(val) {
    case 1: chOn[0] = 1; chOn[1] = 0; break;
    case 2: chOn[0] = 0; chOn[1] = 1; break;
    case 3: chOn[0] = 1; chOn[1] = 1; break;
    default: case 0: chOn[0] = 0; chOn[1] = 0; break; 
  }
  sendplain(""); // acknowledge
  Serial.printf("channelOn = %d\n", val);
}

void handleSpeedup() {  
  String s;
  if (++iTDial>=sizeof(TDial)/sizeof(uint16_t)) iTDial = sizeof(TDial)/sizeof(uint16_t)-1;
  usecsPerDiv = TDial[iTDial];
  s=String(usecsPerDiv);
  sendplain(s);
  Serial.printf("speedup %d\n",usecsPerDiv);
}

void handleSpeeddown() {
  String s;
  iTDial--;
  if (digitalmode[currentCh]) {
    if (iTDial<1) iTDial = 0;
  }
  else if (TDial[iTDial]<100) iTDial++; // analog mode can't handle <100uS well
  
  usecsPerDiv = TDial[iTDial];
  s=String(usecsPerDiv);
  sendplain(s);
  Serial.printf("speeddown %d\n",usecsPerDiv);
}

void handleVoltup() {  
  if (++iVDial>=sizeof(VDial)/sizeof(uint16_t)) iVDial = sizeof(VDial)/sizeof(uint16_t)-1;
  mvPerDiv[currentCh] = VDial[iVDial];
  String s = String(mvPerDiv[currentCh]);
  sendplain(s);
  Serial.printf("voltup %d %d\n",mvPerDiv[currentCh], iVDial);
}

void handleVoltdown() {
  if (--iVDial< 1 ) iVDial = 0;
  mvPerDiv[currentCh] = VDial[iVDial];
  String s = String(mvPerDiv[currentCh]);
  sendplain(s);
  Serial.printf("voltdown %d %d\n",mvPerDiv[currentCh], iVDial);
}

void handleToggleAC() {
  sendplain(""); // acknowledge
  ACcouplingOn[currentCh] = !ACcouplingOn[currentCh];
  Serial.printf("toggle ACcouplingOn\n");
}

void handleToggleDigital() {
  sendplain(""); // acknowledge
  digitalmode[currentCh] = !digitalmode[currentCh];
  if (digitalmode[currentCh]) pinMode(ADCpin[currentCh],INPUT);
  Serial.printf("toggle Digital\n");
}

void handleResolution() {
  resolution = getVal();
  sendplain(""); // acknowledge
}

void handleGround() {
  ground[currentCh] = getVal();
  sendplain(""); // acknowledge
}

void handleUpdate() {
  static long lastmicros = micros();
  static long lastADC = micros();
  uint32_t us;
  String s = "";
  
  while (indx < resolution) {
    us = micros();
  
    if (us-lastADC > ((long)usecsPerDiv/50*YRANGE)/resolution){ 
      if (chOn[0]) getSample1(us);
      if (chOn[1]) getSample2(us);
      y[indx] = us;//-lastsweep;
      lastADC = us;
      indx++;
    }
  }
  
  for (int i=0; i<indx-1; i++) {
    s = s +y[i]+",";
    if (chOn[0]) s = s+x1[i]+",";
    if (chOn[1]) s = s+x2[i]+",";
  }
  s = s +y[indx-1];
  if (chOn[0]) s = s+x1[indx-1];
  if (chOn[1]) s = s+x2[indx-1];
  
  indx = 0;
  sendplain(s);
}
    
void getSample1(uint32_t timeus) {
  static int32_t avgX;
  if (digitalmode[0])  x1[indx] = ((long)XRANGE * 3300* digitalRead(ADCpin[0]))/(8*mvPerDiv[0]) + xoffset[0];
  else  x1[indx] = ((long)XRANGE * analogRead(ADCpin[0]))/(ADCcountPerV*(8*mvPerDiv[0])/1000) + xoffset[0];

  avgX = (avgX*ACCOUPLETAU+ ((int32_t)x1[indx]-xoffset[0])*ACCOUPLETAU) / (ACCOUPLETAU+1); 
  if (ACcouplingOn[0]) {
    xoffset[0] = ground[0] - avgX/ACCOUPLETAU; 
  } else xoffset[0] = ground[0]; 
}

void getSample2(uint32_t timeus) {
  static int32_t avgX;
  if (digitalmode[1])  x2[indx] = ((long)XRANGE * 3300* digitalRead(ADCpin[1]))/(8*mvPerDiv[1]) + xoffset[1];
  else  x2[indx] = ((long)XRANGE * analogRead(ADCpin[1]))/(ADCcountPerV*(8*mvPerDiv[1])/1000) + xoffset[1];

  avgX = (avgX*ACCOUPLETAU+ ((int32_t)x2[indx]-xoffset[1])*ACCOUPLETAU) / (ACCOUPLETAU+1); 
  if (ACcouplingOn[1]) {
    xoffset[1] = ground[1] - avgX/ACCOUPLETAU; 
  } else xoffset[1] = ground[1]; 
}


/* end of function generator specific routines */
/***********************************************/

/********************/
/* standard stuff */

void handleFavicon(){
  sendplain(""); // acknowledge
}

void handleRoot() {
  sendhtml(body);
}

void setup() 
{
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid );
  delay(100); // hack need to Wait 100 ms for AP_START...

  Serial.print("Set softAPConfig "); Serial.println(ssid);

  WiFi.softAPConfig(IPAddress(192, 168, 1, 2),
              IPAddress(192, 168, 1, 2),
              IPAddress(255, 255, 255, 0)); 
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  server.begin();                  //Start server
  Serial.println("HTTP server started");
  
  // function generator handlers
  attachHandler("/testfq?val=",handleTestFq);
  attachHandler("/duty?val=",handleDuty);
  attachHandler("/sinescale?val=",handleSineScale);
  attachHandler("/sineoffset?val=",handleSineOffset);
  attachHandler("/sineprecision?val=",handleSinePrecision);
  attachHandler("/sineOn",handleSine);
  attachHandler("/squareOn",handleSquare);

  // oscope handlers
  attachHandler("/ch?val=",handleChannel);
  attachHandler("/chOn?val=",handleRun);
  attachHandler("/speedup",handleSpeedup);
  attachHandler("/speeddown",handleSpeeddown);
  attachHandler("/voltup",handleVoltup);
  attachHandler("/voltdown",handleVoltdown);
  attachHandler("/toggleAC",handleToggleAC);
  attachHandler("/toggleDigital",handleToggleDigital);
  attachHandler("/resolution?val=",handleResolution);
  attachHandler("/ground?val=",handleGround);

  // webpage handlers
  attachHandler("/up",handleUpdate);
  attachHandler("/favicon.ico",handleFavicon);
  attachHandler("/ ",handleRoot);
}

void loop()
{ 
  static long lastmicros = micros();
  static int toggle;
  uint32_t us;

  us = micros();
  if (us-lastmicros > 5000){  
    lastmicros = us;
    serve(server,body);    
    if (us-lastsweep > USECPERSCREEN) {
      lastsweep - us;
    }
  }
}
