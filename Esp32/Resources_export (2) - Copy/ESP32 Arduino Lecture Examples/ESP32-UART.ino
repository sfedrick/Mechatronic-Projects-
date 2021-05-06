/*
 * ESP32-UART
 *  test routine for SERIAL2 (using pin 16 and 17)
 * output should look something like
 *  ESP32 write 1
 *  teensy: 1teensy teensy
 *  ESP32 write 2
 *  teensy: 2teensy teensy
 *  ...
 */
#define RXD2 9
#define TXD2 10
 
void setup() {
  Serial.begin(115200);
  //  Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
}

int i;
 
void loop() { 
  while (Serial2.available()) { // read any incoming and write it to the monitor
    Serial.print(char(Serial2.read()));
  }

  // to help debug write an increasing number to serial2 once per second
  if (millis()%1000==1){   
    Serial2.println(i++);
    Serial.printf("ESP32 write %d\n",i);
    delay(10);
  }
}
