#ifndef IMU_HANDLER_H
#define IMU_HANDLER_H

#include <math.h>
#include <stdbool.h>


struct INO_Data {
  double x, y, z;
};

extern const float THRESHOLD;
extern const uint8_t QUEUE_CAPACITY;

extern double x_acc_error, y_acc_error, z_acc_error;
//extern double x_gyr_error, y_gyr_error, z_gyr_error;

extern float x_acc, y_acc, z_acc;
//extern float x_gyr, y_gyr, z_gyr;

extern struct INO_Data queue[];
extern uint8_t queue_front;
extern uint8_t queue_rear;
extern uint8_t queue_size;

extern struct INO_Data current_calibrated_data;
extern struct INO_Data previous_calibrated_data;

extern uint8_t flipped_counter;

void initialize_IMU();
void calculate_IMU_error();

struct INO_Data get_accelerometer_data();
//struct INO_Data get_gyroscope_data();

void enqueue(struct INO_Data data);

void dequeue();

double calculate_vector_sum();

struct INO_Data calibrate_data();


bool has_accelerometer_collision();
bool has_flipped();


#endif