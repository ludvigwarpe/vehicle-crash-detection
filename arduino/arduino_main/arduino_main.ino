#include "accelerometer_data_handler.h"

constexpr std::uint16_t BOUND = 9600u;
constexpr std::uint8_t SAMPLE_RATE = 10u;

const std::uint8_t impact_pin = 3; //declares the impact sensor @pin 3
std::uint8_t impact_value;


void setup() {

  Serial.println("Setting up!");
  Serial.begin(BOUND);
  pinMode(impact_pin, INPUT); //sets impact sensor as INPUT
  initialize_accelerometer();

}

bool has_impact_collsion(){
  return (impact_value == LOW);
}

void loop() {

  /*impact_value = digitalRead(impactPin); //reads impact sensor

  if (impact_value == LOW)
  {
    Serial.println("Impact collision has occured!");
  }*/


  if (has_accelerometer_collision())
  {
    Serial.println("Acceleration collision has occured!");
    while(1);
  }
  delay(SAMPLE_RATE);

}
