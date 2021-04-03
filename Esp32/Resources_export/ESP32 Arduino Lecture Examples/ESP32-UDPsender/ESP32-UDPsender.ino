/*
 * sends a packet to esp32-udpreceiver 
 * 
 */
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "modlab1";
const char* password = "ESAP2017";

WiFiUDP UDPTestServer;
unsigned int udpTargetPort=2808;
const int UDP_PACKET_SIZE = 48;
char udpBuffer[UDP_PACKET_SIZE];
IPAddress ipTarget(192, 168, 1, 98); // change to your target esp32 IP ADDRESS
IPAddress ipLocal(192, 168, 1, 99);  // change to your local esp32 IP ADDRESS

void setup() {
  Serial.begin(115200);  
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.print("Connecting to ");  Serial.println(ssid);
  
  WiFi.config(ipLocal, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);

  UDPTestServer.begin(udpTargetPort); // strange bug needs to come after WiFi.begin but before connect
  
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected as "); Serial.print(WiFi.localIP());
}

// send udp packets periodically                                                         
void loop() {
  delay (2000);
  digitalWrite(LED_BUILTIN, LOW); // debugging LED pulses                                     
  delay (1000);
  digitalWrite(LED_BUILTIN, HIGH);
  fncUdpSend();
}

void fncUdpSend()
{
  strcpy(udpBuffer, "hello testing message"); // send anything upto 48 char     
                                              
  UDPTestServer.beginPacket(ipTarget, udpTargetPort);
  UDPTestServer.printf("%s",udpBuffer);
  UDPTestServer.endPacket();
  // Serial.println(udpBuffer);
}


