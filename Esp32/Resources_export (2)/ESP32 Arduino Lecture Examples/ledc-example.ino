#define LEDC_CHANNEL       0 // use first channel of 16  
#define LEDC_RESOLUTION_BITS  13
#define LEDC_RESOLUTION  ((1<<LEDC_RESOLUTION_BITS)-1) 
#define LEDC_FREQ_HZ     5000
#define LED_PIN           2


// can set syntax to be like analogWrite() with input[ 0 : valueMax ]         
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {            
  uint32_t duty =  LEDC_RESOLUTION * min(value, valueMax) / valueMax;   
  ledcWrite(channel, duty);  // write duty to LEDC 
}

void setup() {  // Setup timer and attach timer to a led pin      
  ledcSetup(LEDC_CHANNEL, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL);
}
void loop() { 
  static int brightness = 0;  // how bright the LED is
  static int fadeAmount = 5;  // how much to fade by

  ledcAnalogWrite(LEDC_CHANNEL, brightness);  
  brightness = brightness + fadeAmount;             
  if (brightness <= 0 || brightness >= 255)
    fadeAmount = -fadeAmount;  // reverse the direction 
  delay(30);
}

