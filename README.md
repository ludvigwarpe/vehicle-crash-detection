# Automatic Vechicle Detection System

Hi and welcome to our project! During the past few weeks we have created a prototype of an automatic vehicle detection system, using an Arduino Uno and a remote-controlled car. To see which componets were used in building the prototype, and how they were wired please read the below sections. The project utilizes an edge computing architecture by detecting potential vehicle collisions using accelerometer values. Values from the accelerometer was also used to calculate if the car has flipped over, using the atan2 function. Aswell as collision data, GPS location and speed data is also processed by the Arduino. To send data to the client-side a pub/sub MQTT architecure is used. These are displayed to the user via a simple GUI created in Android Studio.

## Components Used
- Arduino Uno Wifi rev2
- NEO-6M GPS-module with antenna
- KY-031 Knock impact sensor
- 9v battery

## Libraries Used

Arduino:
- TinyGPS by Mikal Hart
- Wifinna by Arduino
- ArduinoMqttClient by Arduino

Android Studio:
- Eclipse Paho Android Service by The Eclipse Foundation

## Wiring schematic
![Project Wiring](https://user-images.githubusercontent.com/75011876/211012858-10099473-5341-4110-897a-6f88d19aa1aa.jpg)

## Prototype
![bil](https://user-images.githubusercontent.com/75011876/211159002-da13a09c-670e-4286-8a3a-f1aae2d3ea1b.jpg)

## Testing
Testing of the prototype included 18 different scenarios based on different factors such as: speed, direction of the impact and if the car rolled over. We had overall accurate results predicting if a collison had occured.





