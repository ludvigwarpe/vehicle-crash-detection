#ifndef IMU_HANDLER_H
#define IMU_HANDLER_H

#include <math.h>
#include <stdbool.h>


struct AcceleratorData {
  float x, y, z;
};

extern const uint8_t THRESHOLD;
extern const uint8_t QUEUE_CAPACITY;
extern const uint8_t ZERO;
extern const uint8_t ONE;
extern const uint8_t TWO;

extern float x_out, y_out, z_out;

extern struct AcceleratorData queue[];
extern uint8_t queue_front;
extern uint8_t queue_rear;
extern uint8_t queue_size;

extern struct AcceleratorData current_calibrated_data;
extern struct AcceleratorData previous_calibrated_data;

void initialize_IMU();

struct AcceleratorData get_accelerometer_data();

void enqueue(struct AcceleratorData data);

void dequeue();

float calculate_vector_sum();

struct AcceleratorData calibrate_data();


bool has_accelerometer_collision();


#endif