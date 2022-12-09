#ifndef ACCELEROMETER_DATA_HANDLER_H
#define ACCELEROMETER_DATA_HANDLER_H

#include <Wire.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>


int ADXL345 = 0x53;

constexpr byte THRESHOLD = 2;  // will have to be fine tuned
constexpr byte QUEUE_CAPACITY = 6;
constexpr byte ZERO = 0;
constexpr byte ONE = 1;
constexpr byte TWO = 2;

float x_out;
float y_out;
float z_out;

struct AcceleratorData {
  float x;
  float y;
  float z;
};

struct AcceleratorData queue[QUEUE_CAPACITY];
byte queue_front = 0;
byte queue_rear = 0;
byte queue_size = 0;

struct AcceleratorData current_calibrated_data;
struct AcceleratorData previous_calibrated_data;

void initialize_accelerometer() {
  Wire.begin();
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
  delay(10);
}

struct AcceleratorData get_accelerator_data() {

  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true);
  x_out = (Wire.read() | Wire.read() << 8);
  x_out = x_out / 256;
  y_out = (Wire.read() | Wire.read() << 8);
  y_out = y_out / 256;
  z_out = (Wire.read() | Wire.read() << 8);
  z_out = z_out / 256;

  struct AcceleratorData raw_data;
  raw_data.x = x_out;
  raw_data.y = y_out;
  raw_data.z = z_out;

  return raw_data;
}

void enqueue(struct AcceleratorData data) {


  if (queue_size < QUEUE_CAPACITY) {
    queue_rear = (queue_rear + ONE) % QUEUE_CAPACITY;
    queue[queue_rear] = data;
    queue_size++;
  }
}

void dequeue() {


  if (queue_size > ZERO) {
    queue_front = (queue_front + ONE) % QUEUE_CAPACITY;
    queue_size--;
  }
}

float calculate_vector_sum() {
  float x = current_calibrated_data.x - previous_calibrated_data.x;
  float y = current_calibrated_data.y - previous_calibrated_data.y;
  float z = current_calibrated_data.z - previous_calibrated_data.z;

  return sqrt(x - y - z);
}

struct AcceleratorData calibrate_data() {


  float x_sum = 0.0f;
  float y_sum = 0.0f;
  float z_sum = 0.0f;


  for (byte i = ZERO; i < queue_size; i++) {
    x_sum += queue[i].x;
    y_sum += queue[i].y;
    z_sum += queue[i].z;
  }

  float x1_avg = x_sum / queue_size;
  float y1_avg = y_sum / queue_size;
  float z1_avg = z_sum / queue_size;

  float x_even_sum = 0.0f;
  float x_odd_sum = 0.0f;

  float y_even_sum = 0.0f;
  float y_odd_sum = 0.0f;

  float z_even_sum = 0.0f;
  float z_odd_sum = 0.0f;


  for (byte i = ZERO; i < queue_size; i++) {
    if (i % TWO == ZERO) {
      x_even_sum += queue[i].x;
      y_even_sum += queue[i].y;
      z_even_sum += queue[i].z;
    }

    else {
      x_odd_sum += queue[i].x;
      y_odd_sum += queue[i].y;
      z_odd_sum += queue[i].z;
    }
  }

  byte half_queue_size = queue_size / TWO;

  if (half_queue_size == ZERO) {
    half_queue_size = ONE;
  }

  float x2_avg = (x_odd_sum / half_queue_size) + (x_even_sum / half_queue_size);
  float y2_avg = (y_odd_sum / half_queue_size) + (y_even_sum / half_queue_size);
  float z2_avg = (z_odd_sum / half_queue_size) + (z_even_sum / half_queue_size);

  float x = (x1_avg + x2_avg) / TWO;
  float y = (y1_avg + y2_avg) / TWO;
  float z = (z1_avg + z2_avg) / TWO;

  struct AcceleratorData calibrated_data;

  calibrated_data.x = x;
  calibrated_data.y = y;
  calibrated_data.z = z;

  return calibrated_data;
}



bool has_accelerometer_collision() {

  struct AcceleratorData raw_data = get_accelerator_data();


  if (queue_size == QUEUE_CAPACITY) {
    dequeue();
  }
  enqueue(raw_data);


  current_calibrated_data = calibrate_data();
  float vector_sum = calculate_vector_sum();

  previous_calibrated_data = current_calibrated_data;

  if (vector_sum > THRESHOLD) {

    return true;
  }

  return false;
}


#endif