#include "publisher.h"



char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
int port = 1883;
const char broker[] = "test.mosquitto.org";
const char topic_latitude[] = "luwa9626/vehicle-crash-detection/sensors/gps/latitude";
const char topic_longitude[] = "luwa9626/vehicle-crash-detection/sensors/gps/longitude";
const char topic_speed[] = "luwa9626/vehicle-crash-detection/sensors/gps/speed";
const char topic_accident[] = "luwa9626/vehicle-crash-detection/sensors/impact";


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
  Serial.println();
}

void connect_mqtt() {
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  while (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code: ");
    Serial.println(mqttClient.connectError());
    delay(5000);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void check_connection(){
  if (!mqttClient.connected()){
    Serial.println("Reconnecting MQTT broker!");
    connect_mqtt();
  }
  if (WiFi.status() != WL_CONNECTED){
    Serial.println("Reconnecting WiFi!");
    WiFi.disconnect();
    connect_wifi();
  }
  mqttClient.poll();
}


void send_message(char topic[], char message[]) {
  mqttClient.beginMessage(topic);
  mqttClient.print(message);
  mqttClient.endMessage();
 /* Serial.print("Sent to topic: ");
  Serial.print(topic);
  Serial.print(" MSG: ");
  Serial.println(message);*/

}


void send_message(char topic[], float message) {
  mqttClient.beginMessage(topic);
  mqttClient.print(message, 6);
  mqttClient.endMessage();
  /*Serial.print("Sent to topic: ");
  Serial.print(topic);
  Serial.print(" MSG: ");
  Serial.println(message, 6);*/
}