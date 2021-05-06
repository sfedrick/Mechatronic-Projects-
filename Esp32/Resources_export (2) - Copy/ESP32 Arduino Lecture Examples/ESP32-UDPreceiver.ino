/*
 * receives packets from UDP-TCPsender.c (plus any other broadcasters)
 * 
 */
#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "modlab1";
const char* password = "ESAP2017";

WiFiUDP UDPTestServer;
unsigned int UDPPort = 2808;
IPAddress myIPaddress(192, 168, 1, 98);

const int UDP_PACKET_SIZE = 48; // can be up to 65535
byte packetBuffer[UDP_PACKET_SIZE+1];

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");  Serial.println(ssid);
 
  WiFi.config(myIPaddress, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid, password);

  UDPTestServer.begin(UDPPort); // strange bug needs to come after WiFi.begin but before connect
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");  
  packetBuffer[UDP_PACKET_SIZE] = 0; // null terminate buffer
}

void loop() {
   handleUDPServer();
   delay(1);
}

void handleUDPServer() {
  int cb = UDPTestServer.parsePacket();
  if (cb) {
    UDPTestServer.read(packetBuffer, UDP_PACKET_SIZE);
    Serial.printf("%s\n",packetBuffer);
    
  }
}
