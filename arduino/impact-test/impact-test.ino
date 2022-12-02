const int impactPin = 3; //declares the impact sensor @pin 3
int value;

void setup() {
  Serial.begin(9600);
  pinMode(impactPin, INPUT); //sets the sensor as INPUT
}


void loop() {
  value = digitalRead(knockPin); //reads status of sensor

  if (value == HIGH) {
    Serial.print("KY-031: ");
    Serial.println(value);
  }
}
