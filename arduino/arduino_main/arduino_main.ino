#include "IMU_handler.h"
#include "GPS_handler.h"
#include "publisher.h"


// sample rate
const uint8_t SAMPLE_RATE = 10;

// impact sensor
const int IMPACT_PIN = 8;

// used for delay
unsigned long current_time_millis;
unsigned long previous_time_millis;

// starting program
void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  Serial.println("Starting program!");
  pinMode(IMPACT_PIN, INPUT);
  connect_wifi();
  connect_mqtt();
  initialize_IMU();
  delay(1000);
}
// main program loop
void loop() {

  check_connection();

  current_time_millis = millis();
  if (current_time_millis - previous_time_millis >= SAMPLE_RATE) {

    scan_gps_data();

    if (has_accelerometer_collision()) {
      Serial.println("Collision detected");
      send_message(topic_accident, "collision");
    }
    if (has_flipped()) {
      Serial.println("Car has flipped!");
      send_message(topic_accident, "flipped");
      delay(1000);
    }
    previous_time_millis = current_time_millis;
  }
  if (has_impact_collsion()) {
    Serial.println("Impact detected!");
    send_message(topic_accident, "knock");
  }
  if(new_gps_data){
    get_location_data();
    get_speed_data();
    send_message(topic_latitude, current_lat);
    send_message(topic_longitude, current_long);
    send_message(topic_speed, current_speed);
  }
}

// reads the impact sensor for collision
bool has_impact_collsion() {
  return digitalRead(IMPACT_PIN) == LOW;
}