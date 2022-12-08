#include "accelerometer_data_handler.h"

constexpr std::uint8_t BOUND = 9600u;
constexpr std::uint8_t SAMPLE_RATE = 10u;


void setup() {

  Serial.println("Setting up!");
  Serial.begin(BOUND);

}

void loop() {
  detect_collision();
  delay(SAMPLE_RATE);

}
