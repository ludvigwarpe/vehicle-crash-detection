#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "arduino_secrets.h"

/*---- credentials for wifi ----*/
extern char ssid[];
extern char pass[];

// wifi and mqtt conection
extern WiFiClient wifiClient;
extern MqttClient mqttClient;

extern int port;
extern const char broker[];
extern const char topic_latitude[];
extern const char topic_longitude[];
extern const char topic_speed[];
extern const char topic_accident[];

// initializing wifi connection
void connect_wifi();

// initializing mqtt connection
void connect_mqtt();

void check_connection();

// publishes message with specified topic on mqtt broker
void send_message(char topic[], char message[]);

//publishes gps data
void send_message(char topic[], float message);



#endif