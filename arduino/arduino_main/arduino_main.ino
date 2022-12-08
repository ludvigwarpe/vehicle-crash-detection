#include "accelerometer_data_handler.h"

constexpr std::uint8_t BOUND = 9600u;
constexpr std::uint8_t SAMPLE_RATE = 10u;


void setup() {

  Serial.println("Setting up!");
  Serial.begin(BOUND);
  initialize_accelerometer();

}

void loop() {


  if (detect_collision())
  {
    Serial.println("Collision has occured!");
    while(1);
  }
  delay(SAMPLE_RATE);

}
