
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "arduino_secrets.h"
#include "IMU_handler.h"

// credentials for wifi
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// wifi and mqtt conection
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
int port = 1883;
const char broker[] = "test.mosquitto.org";
const char topic_location[] = "luwa9626/vehicle-crash-detection/sensors/gps/location";
const char topic_speed[] = "luwa9626/vehicle-crash-detection/sensors/gps/speed";
const char topic_impact[] = "luwa9626/vehicle-crash-detection/sensors/impact";
const char topic_flipped[] = "luwa9626/vehicle-crash-detection/sensors/flipped";

//temporary mock values
const char location_temp[] = "59.31626265649651, 18.091590409027457";
const char speed_temp[] = "120 km/h";


// sample rate
const uint8_t SAMPLE_RATE = 10;

// impact sensor
const int impact_pin = 8;

// used for delay
unsigned long current_time_millis;
unsigned long previous_time_millis;


// starting program
void setup() {


  Serial.begin(9600);
  Serial.println("Starting program!");
  pinMode(impact_pin, INPUT);
  connect_wifi();
  connect_mqtt();
  initialize_IMU();
  delay(1000);
}
// main program loop
void loop() {

  current_time_millis = millis();

  if (current_time_millis - previous_time_millis >= SAMPLE_RATE) {

    mqttClient.poll();  //keeping mqtt connection alive

    /* PUBLISH DATA TO BROKER WITH LOCATION AND SPEED*/
    send_message(topic_location, location_temp);
    send_message(topic_speed, speed_temp);

    if (has_accelerometer_collision()) {
      Serial.println("Collision detected");
      send_message(topic_impact, "collision");
      //delay(1000);
    }
    if (has_flipped()) {
      Serial.println("Car has flipped!");
      send_message(topic_impact, "flipped");
      //delay(1000);
    }
    previous_time_millis = current_time_millis;
  }

  if (has_impact_collsion()) {
    Serial.println("Impact detected!");
    send_message(topic_impact, "knock");
    //delay(1000);
  }

}


// initializing wifi connection
void connect_wifi() {
  uint8_t status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }
  Serial.println("You are now connected to the network: ");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// initializing mqtt connection
void connect_mqtt() {
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code: ");
    Serial.println(mqttClient.connectError());

    while (1)
      ;
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

// publishes message with specified topic on mqtt broker
void send_message(char topic[], char message[]) {
  //Serial.println("Sending message to topic: ");
  //Serial.println(topic);
  //Serial.println(message);

  mqttClient.beginMessage(topic);
  mqttClient.print(message);
  mqttClient.endMessage();
  //Serial.println();
}


// reads the impact sensor for collision
bool has_impact_collsion() {

  return digitalRead(impact_pin) == LOW;
}
