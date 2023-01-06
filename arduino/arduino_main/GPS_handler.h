#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <SoftwareSerial.h>
#include <TinyGPS.h>


extern SoftwareSerial ss;
extern TinyGPS gps;

extern bool new_gps_data;
extern float current_lat;
extern float current_long;
extern float current_speed;

bool has_new_gps_data();

void scan_gps_data();

void get_location_data();

void get_speed_data();

#endif