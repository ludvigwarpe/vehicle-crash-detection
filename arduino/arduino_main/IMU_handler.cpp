
#include <Arduino_LSM6DS3.h>
#include "IMU_handler.h"


const float THRESHOLD = 1.0;  // will have to be fine tuned
const uint8_t QUEUE_CAPACITY = 6;

double x_acc_error = 0.0;
double y_acc_error = 0.0;
double z_acc_error = 0.0;

/*double x_gyr_error = 0.0;
double y_gyr_error = 0.0;
double z_gyr_error = 0.0;*/

float x_acc = 0.0f;
float y_acc = 0.0f;
float z_acc = 0.0f;

/*float x_gyr = 0.0f;
float y_gyr = 0.0f;
float z_gyr = 0.0f;*/

struct INO_Data queue[QUEUE_CAPACITY];
uint8_t queue_front = 0;
uint8_t queue_rear = 0;
uint8_t queue_size = 0;

struct INO_Data current_calibrated_data;
struct INO_Data previous_calibrated_data;

uint8_t flipped_counter = 0;

void read_accelerometer() {

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x_acc, y_acc, z_acc);
  } 
}

/*void read_gyroscope() {
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x_gyr, y_gyr, z_gyr);
  } 
}*/

void calculate_IMU_error() {
  for (uint8_t i = 0; i < 200; i++) {
    read_accelerometer();
    x_acc_error += x_acc;
    y_acc_error += y_acc;
    z_acc_error += z_acc;

    /*read_gyroscope();
    x_gyr_error += x_gyr;
    y_gyr_error += y_gyr;
    z_gyr_error += z_gyr;*/
    delay(10);
  }
  //Divide the sum by 200 to get the error value
  x_acc_error /= 200;
  y_acc_error /= 200;
  z_acc_error /= 200;

  /*x_gyr_error /= 200;
  y_gyr_error /= 200;
  z_gyr_error /= 200;*/
}

void initialize_IMU() {

  while (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    delay(5000);
  }
  calculate_IMU_error();
  Serial.println("IMU initialized!");
}

struct INO_Data get_accelerometer_data() {


  read_accelerometer();

  struct INO_Data data;
  data.x = x_acc - x_acc_error;
  data.y = y_acc - y_acc_error;
  data.z = z_acc - z_acc_error;

  return data;
}

/*struct INO_Data get_gyroscope_data() {


  read_gyroscope();

  struct INO_Data data;
  data.x = x_gyr - x_gyr_error;
  data.y = y_gyr - y_gyr_error;
  data.z = z_gyr - z_gyr_error;

  return data;
}*/

void enqueue(struct INO_Data data) {


  if (queue_size < QUEUE_CAPACITY) {
    queue_rear = (queue_rear + 1) % QUEUE_CAPACITY;
    queue[queue_rear] = data;
    queue_size++;
  }
}

void dequeue() {


  if (queue_size > 0) {
    queue_front = (queue_front + 1) % QUEUE_CAPACITY;
    queue_size--;
  }
}

double calculate_vector_sum() {
  double delt_x = current_calibrated_data.x - previous_calibrated_data.x;
  double delt_y = current_calibrated_data.y - previous_calibrated_data.y;
  double delt_z = current_calibrated_data.z - previous_calibrated_data.z;

  return sqrt(delt_x * delt_x + delt_y * delt_y + delt_z * delt_z);
}

struct INO_Data calibrate_data() {


  double x_sum = 0.0;
  double y_sum = 0.0;
  double z_sum = 0.0;


  for (uint8_t i = 0; i < queue_size; i++) {
    x_sum += queue[i].x;
    y_sum += queue[i].y;
    z_sum += queue[i].z;
  }

  double x1_avg = x_sum / queue_size;
  double y1_avg = y_sum / queue_size;
  double z1_avg = z_sum / queue_size;

  double x_even_sum = 0.0f;
  double x_odd_sum = 0.0f;

  double y_even_sum = 0.0f;
  double y_odd_sum = 0.0f;

  double z_even_sum = 0.0f;
  double z_odd_sum = 0.0f;


  for (uint8_t i = 0; i < queue_size; i++) {
    if (i % 2 == 0) {
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

  uint8_t half_queue_size = queue_size / 2;

  if (half_queue_size == 0) {
    half_queue_size = 1;
  }

  double x2_avg = (x_odd_sum / half_queue_size) + (x_even_sum / half_queue_size);
  double y2_avg = (y_odd_sum / half_queue_size) + (y_even_sum / half_queue_size);
  double z2_avg = (z_odd_sum / half_queue_size) + (z_even_sum / half_queue_size);

  double x = (x1_avg + x2_avg) / 2;
  double y = (y1_avg + y2_avg) / 2;
  double z = (z1_avg + z2_avg) / 2;

  struct INO_Data calibrated_data;

  calibrated_data.x = x;
  calibrated_data.y = y;
  calibrated_data.z = z;

  return calibrated_data;
}



bool has_accelerometer_collision() {

  struct INO_Data raw_data = get_accelerometer_data();


  if (queue_size == QUEUE_CAPACITY) {
    dequeue();
  }
  enqueue(raw_data);


  current_calibrated_data = calibrate_data();
  double vector_sum = calculate_vector_sum();

  previous_calibrated_data = current_calibrated_data;

  if (vector_sum > THRESHOLD) {
    Serial.print("COLLISION: ");
    Serial.println(vector_sum);

    return true;
  }
  

  return false;
}

bool has_flipped() {

  struct INO_Data data = get_accelerometer_data();

  double roll = atan2(data.y, data.z) * 180/M_PI;

  if ((roll > 160)||(roll < -160)){
    flipped_counter++;
    if (flipped_counter >= 15){
      Serial.println(roll);
      return true;
    }
  }
  else{
    flipped_counter = 0;
  }
  return false;


  /*struct INO_Data data = get_gyroscope_data();


  if (data.x > 550 || data.x < -550 || data.y > 450 || data.y < -450) {
    Serial.println("Gyroscope:  ");
    Serial.println(data.x);
    Serial.println(data.y);
    Serial.println(data.z);

    Serial.println();
    return true;
  }
  return false;*/
}
