
#include <Arduino_LSM6DS3.h>
#include "accelerometer_data_handler.h"


const uint8_t THRESHOLD = 2;  // will have to be fine tuned
const uint8_t QUEUE_CAPACITY = 6;
const uint8_t ZERO = 0;
const uint8_t ONE = 1;
const uint8_t TWO = 2;

float x_out = 0.0f;
float y_out = 0.0f;
float z_out = 0.0f;

struct AcceleratorData queue[QUEUE_CAPACITY];
uint8_t queue_front = 0;
uint8_t queue_rear = 0;
uint8_t queue_size = 0;

struct AcceleratorData current_calibrated_data;
struct AcceleratorData previous_calibrated_data;

void initialize_IMU() {

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }
  Serial.println("IMU initialized!");
  delay(10);
}

struct AcceleratorData get_accelerometer_data() {


  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x_out, y_out, z_out);
  } else {
    Serial.println("Failed to read accelerometer!");
  }

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
  float delt_x = current_calibrated_data.x - previous_calibrated_data.x;
  float delt_y = current_calibrated_data.y - previous_calibrated_data.y;
  float delt_z = current_calibrated_data.z - previous_calibrated_data.z;

  return sqrt(pow(delt_x, 2) + pow(delt_y, 2) + pow(delt_z, 2));
}

struct AcceleratorData calibrate_data() {


  float x_sum = 0.0f;
  float y_sum = 0.0f;
  float z_sum = 0.0f;


  for (uint8_t i = ZERO; i < queue_size; i++) {
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


  for (uint8_t i = ZERO; i < queue_size; i++) {
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

  uint8_t half_queue_size = queue_size / TWO;

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

  struct AcceleratorData raw_data = get_accelerometer_data();


  if (queue_size == QUEUE_CAPACITY) {
    dequeue();
  }
  enqueue(raw_data);


  current_calibrated_data = calibrate_data();
  float vector_sum = calculate_vector_sum();

  previous_calibrated_data = current_calibrated_data;

  if (vector_sum > THRESHOLD) {
    //Serial.println("COLLISION!!!");
    //Serial.println(vector_sum);

    return true;
  }

  //Serial.println("YOU ARE SAFE!!!");
  //Serial.println(vector_sum);

  return false;
}
