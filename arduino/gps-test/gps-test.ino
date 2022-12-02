#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const int RXPin = 4, TXPin = 3;
const uint32_t GPSBaud = 9600;

SoftwareSerial ss(RXPin,TXPin); //Serial connection to GPS module, assumes pin 4 as RX and pin 3 as TX
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop() {
  while(ss.available() > 0){
    gps.encode(ss.read());
    Serial.print("LATITUDE= ")
    Serial.print(gps.location.lat(), 6);
    Serial.print(" LONGITUDE= ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("TIME (HHMMSSCC): ");
    Serial.println(gps.time.value());
    Serial.println("-------------------");
  }
}
