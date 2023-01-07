#ifndef GPS_HANDLER_H
#define GPS_HANDLER_H

#include <SoftwareSerial.h>
#include <TinyGPS.h>

/*----- Variables used for GPS connection and data -----*/
extern SoftwareSerial ss;
extern TinyGPS gps;
extern bool new_gps_data;
extern float current_lat;
extern float current_long;
extern float current_speed;


/*----- Scans and encodes gps data from software serial -----*/
void scan_gps_data();

/*----- Parses latitude and longitude from NMEA sentences, 
    if the age or angle is invalid the variables are assigned a value of 0.0 -----*/
void get_location_data();


/*----- Parses ground speed in km/h, 
    if the angle is invalid the variable is assigned 0.0 -----*/
void get_speed_data();

#endif