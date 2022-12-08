#include <Wire.h>

int ADXL345 = 0x53;
float x_out, y_out, z_out;

void setup(){
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
  delay(10);
}

void loop(){
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);
  x_out = (Wire.read()|Wire.read() << 8);
  x_out = x_out/256;
  y_out = (Wire.read()|Wire.read() << 8);
  y_out = y_out/256;
  z_out = (Wire.read()|Wire.read() << 8);
  z_out = z_out/256;
  Serial.print("Xa= ");
  Serial.print(x_out);
  delay(500);
  Serial.print("  Ya= ");
  Serial.print(y_out);
  delay(500);
  Serial.print("  Za= ");
  Serial.println(z_out);
  delay(500);

}

/*#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Wire.h>

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();

void setup() {
  Serial.begin(9600);
  if(!accel.begin()){
    Serial.println("No valid sensor found")
    while(1);
  }

}

void loop() {
  sensors_event_t event;
  accel.getEvent(&event);
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("");
  Serial.println("m/s^2 ");
  delay(500);
}*/