#include "GPS_handler.h"


TinyGPS gps;
SoftwareSerial ss(4, 3);

bool new_gps_data = false;
float current_lat = 0.0;
float current_long = 0.0;
float current_speed = 0.0;


void scan_gps_data(){
   while (ss.available())
    {
      char c = ss.read();

      if (gps.encode(c))
        new_gps_data = true;
    }
  }

void get_location_data(){
  unsigned long age;
  gps.f_get_position(&current_lat, &current_long, &age);
  current_lat = current_lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : current_lat;
  current_long = current_long == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : current_long;
  
}

void get_speed_data(){
  current_speed = gps.f_speed_kmph() == TinyGPS::GPS_INVALID_F_SPEED ? 0.0 : gps.f_speed_kmph();
}