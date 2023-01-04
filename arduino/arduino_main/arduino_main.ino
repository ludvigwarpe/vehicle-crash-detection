#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include "arduino_secrets.h"
#include "IMU_handler.h"
#include <SoftwareSerial.h>
#include <TinyGPS.h>


/*---- credentials for wifi ----*/
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
// wifi and mqtt conection
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
int port = 1883;
const char broker[] = "test.mosquitto.org";
const char topic_latitude[] = "luwa9626/vehicle-crash-detection/sensors/gps/latitude";
const char topic_longitude[] = "luwa9626/vehicle-crash-detection/sensors/gps/longitude";
const char topic_speed[] = "luwa9626/vehicle-crash-detection/sensors/gps/speed";
const char topic_impact[] = "luwa9626/vehicle-crash-detection/sensors/impact";

//GPS
//Variables used for GPS connection and data
TinyGPS gps;
SoftwareSerial ss(4, 3);
bool new_gps_data = false;
float current_lat;
float current_long;
float current_speed;

float current_lat = 0.0;
float current_long = 0.0;
float current_speed = 0.0;

// sample rate
const uint8_t SAMPLE_RATE = 10;
// impact sensor
const int impact_pin = 8;
// used for delay
unsigned long current_time_millis;
unsigned long previous_time_millis;
// starting program
void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("Starting program!");
  pinMode(impact_pin, INPUT);
  connect_wifi();
  connect_mqtt();
  initialize_IMU();
  delay(1000);
}
// main program loop
void loop() {
  if (!mqttClient.connected()){
    Serial.println("Reconnecting MQTT broker!");
    connect_mqtt();
  }
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Reconnecting WiFi!");
    WiFi.disconnect();
    connect_wifi();
  }
  mqttClient.poll();
  current_time_millis = millis();

  if (current_time_millis - previous_time_millis >= SAMPLE_RATE) {

    scan_gps_data(); //Scans and encodes gps data from software serial
    scan_gps_data();

    if (has_accelerometer_collision()) {
      Serial.println("Collision detected");
      send_message(topic_impact, "collision");
    }
    if (has_flipped()) {
      Serial.println("Car has flipped!");
      send_message(topic_impact, "flipped");
      delay(1000);
    }
    previous_time_millis = current_time_millis;
  }
  if (has_impact_collsion()) {
    Serial.println("Impact detected!");
    send_message(topic_impact, "knock");
  }
  if(new_gps_data){
    get_location_data();
    get_speed_data();
    send_message(topic_latitude, current_lat);
    send_message(topic_longitude, current_long);
    send_message(topic_speed, current_speed);
  }
}
// initializing wifi connection
void connect_wifi() {
  uint8_t status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    Serial.println(status);
    delay(5000);
  }
  Serial.println("You are now connected to the network: ");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
@@ -156,6 +156,7 @@
  return digitalRead(impact_pin) == LOW;
}

//Scans and encodes gps data from software serial
void scan_gps_data(){
   while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c))
        new_gps_data = true;
    }
  }

//Parses latitude and longitude from NMEA sentences, if the age or angle is invalid the variables are assigned a value of 0.0
void get_location_data(){
  unsigned long age;
  gps.f_get_position(&current_lat, &current_long, &age);
  current_lat = current_lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : current_lat;
  current_long = current_long == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : current_long;

}

//Parses ground speed in km/h, if the angle is invalid the variable is assigned 0.0
void get_speed_data(){
  current_speed = gps.f_speed_kmph() == TinyGPS::GPS_INVALID_F_SPEED ? 0.0 : gps.f_speed_kmph();
}