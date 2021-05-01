/* The ESP32 has four SPi buses, however as of right now only two of
 * them are available to use, HSPI and VSPI. Simply using the SPI API 
 * as illustrated in Arduino examples will use HSPI, leaving VSPI unused.
 * 
 * created 30/04/2018 by Alistair Symonds
 * modified 26/11/2018 by Mark Yim
 */
#include <SPI.h>

static const int SPIClock = 1000000;  // in Hz
SPIClass * vspi = NULL;

void setup() {
  vspi = new SPIClass(VSPI);
  
  //default pins: SCLK = 18, MISO = 19, MOSI = 23, SS = 5
  vspi->begin();  //can set pins e.g. vspi->begin(0, 2, 4, 33); //SCLK, MISO, MOSI, SS

  pinMode(22, OUTPUT);  // use 22 for SS
}

void loop() {
  byte data = 42; // random data

  vspi->beginTransaction(SPISettings(SPIClock, MSBFIRST, SPI_MODE0));
  digitalWrite(22, LOW); //pull SS slow to prep other end for transfer
  vspi->transfer(data);  
  digitalWrite(22, HIGH); //pull ss high to signify end of data transfer
  vspi->endTransaction();
  delay(10);
}

