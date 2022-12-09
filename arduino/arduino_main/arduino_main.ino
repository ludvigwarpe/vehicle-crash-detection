#include "accelerometer_data_handler.h"
#include <stdint.h>

constexpr byte SAMPLE_RATE = 10;

const byte impact_pin = 3; //declares the impact sensor @pin 3
byte impact_value = 0;


void setup() {

  Serial.println("Setting up!");
  Serial.begin(9600);
  pinMode(impact_pin, INPUT); //sets impact sensor as INPUT
  initialize_accelerometer();

}

bool has_impact_collsion(){
  return impact_value == LOW;
}

void loop() {

  /*impact_value = digitalRead(impactPin); //reads impact sensor

  if (has_impact_collsion())
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
