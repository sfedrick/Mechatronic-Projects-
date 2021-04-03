#define LEDC_CHANNEL       0 // use first channel of 16  
#define LEDC_RESOLUTION_BITS  8
#define LEDC_FREQ_HZ     5000
#define LED_PIN           21
#define LED_ANALOG       A3
int duty=0;
int readin=0;
// can set syntax to be like analogWrite() with input[ 0 : valueMax ]         

void setup() {  // Setup timer and attach timer to a led pin      
  Serial.begin(115200);
  ledcSetup(LEDC_CHANNEL, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL);
  
}
void loop() { 
  readin=analogRead(A3);
  duty=map(readin,0,1770,0,255);
  Serial.println(duty);
  ledcWrite(LEDC_CHANNEL, duty);  // write duty to LEDC 
 
}
