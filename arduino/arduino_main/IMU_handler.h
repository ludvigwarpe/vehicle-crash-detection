#ifndef IMU_HANDLER_H
#define IMU_HANDLER_H

#include <Arduino_LSM6DS3.h>
#include <math.h>
#include <stdbool.h>


/*---- constants ----*/
extern const float THRESHOLD;
extern const uint8_t MAX_FLIPS_IN_ROW;
extern const uint8_t QUEUE_CAPACITY;

/*---- struct representing accelerometer data. 
    Contains the x, y and z values from the accelerometer ----*/
struct Data {
  double x, y, z;
};

/*---- variables used for error calculation ----*/
extern double x_acc_error, y_acc_error, z_acc_error;

/*---- variables to store raw data from accelerometer ----*/
extern float x_acc, y_acc, z_acc;

/*---- variables used for a simple queue implemented as a circular array. 
    used to store data from multiple timestamps. ----*/
extern struct Data queue[];
extern uint8_t queue_front;
extern uint8_t queue_rear;
extern uint8_t queue_size;

/*---- variables to calculate differences between data from different timestamps ----*/
extern struct Data current_calibrated_data;
extern struct Data previous_calibrated_data;

/*---- keeps count of the number of detected flipps in a row ----*/
extern uint8_t flipped_counter;

/*---- initializes the internal meassurement unit ----*/
void initialize_IMU();

/*---- calculates the error of the accelerometer and sets the error values ----*/
void calculate_accelerometer_error();

/*---- reads the accelerometer and provides raw data ----*/
struct Data get_accelerometer_data();

/*---- puts data into the queue ----*/
void enqueue(struct Data data);

/*---- dequeues the queue ----*/
void dequeue();

/*---- calculates the vector sum (magnitude) by comparing current data and previous data ----*/
double calculate_vector_sum();

/*---- calibrates data by calculating an avarage from multiple timestamps ----*/
struct Data calibrate_data();

/*---- detemines if a collision has occured based if the magnitude is greater than the threshold----*/
bool has_accelerometer_collision();

/*---- detemines if the sensor is upside down ----*/
bool has_flipped();


#endif