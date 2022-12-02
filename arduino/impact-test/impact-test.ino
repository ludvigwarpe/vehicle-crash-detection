const byte interruptPin = 2;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(interruptPin), Knock, RISING );
}
void Knock() {
  Serial.println("1");
  delay(2);
}

void loop() {
  Serial.println("0");}
