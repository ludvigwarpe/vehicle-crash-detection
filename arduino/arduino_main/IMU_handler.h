#ifndef IMU_HANDLER_H
#define IMU_HANDLER_H

#include <Arduino_LSM6DS3.h>
#include <math.h>
#include <stdbool.h>


struct Data {
  double x, y, z;
};

extern const float THRESHOLD;
extern const uint8_t QUEUE_CAPACITY;

extern double x_acc_error, y_acc_error, z_acc_error;

extern float x_acc, y_acc, z_acc;

extern struct Data queue[];
extern uint8_t queue_front;
extern uint8_t queue_rear;
extern uint8_t queue_size;

extern struct Data current_calibrated_data;
extern struct Data previous_calibrated_data;

extern uint8_t flipped_counter;

void initialize_IMU();
void calculate_IMU_error();

struct Data get_accelerometer_data();

void enqueue(struct Data data);

void dequeue();

double calculate_vector_sum();

struct Data calibrate_data();


bool has_accelerometer_collision();
bool has_flipped();


#endif