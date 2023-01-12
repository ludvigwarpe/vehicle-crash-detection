#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "arduino_secrets.h"

/*---- credentials for wifi ----*/
extern char ssid[];
extern char pass[];

/*---- connection variables ----*/
extern WiFiClient wifiClient;
extern MqttClient mqttClient;
extern int port;
extern const char broker[];

/*---- mqtt topics ----*/
extern const char topic_latitude[];
extern const char topic_longitude[];
extern const char topic_speed[];
extern const char topic_accident[];


/*---- mqtt parameters for publishing ----*/
extern bool retained;
extern int qos;
extern bool duplicates;

/*---- connects to wifi ----*/
void connect_wifi();

/*---- connects mqtt ----*/
void connect_mqtt();

/*---- checks the connections and reconnects if needed ----*/
void check_connection();


/*---- publishes message with specified topic on mqtt broker ----*/
void send_message(char topic[], char message[]);
void send_message(char topic[], float message);



#endif