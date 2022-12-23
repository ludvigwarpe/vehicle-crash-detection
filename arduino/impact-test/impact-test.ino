const int impactPin = 8; 
int value;
int sample_rate = 1000;

unsigned long current_time_millis;
unsigned long previous_time_millis;

void setup() {
  Serial.begin(9600);
  pinMode(impactPin, INPUT); 
}


void loop() {

  current_time_millis = millis();
  if (digitalRead(impactPin) == LOW) {
    //Serial.print("KY-031: ");
    Serial.println("IMPACT!!!!!");
  }
  if(current_time_millis - previous_time_millis >= sample_rate){
    //Serial.println("Delay");
    previous_time_millis = current_time_millis;
  }
  
  /*else{
    Serial.print("KY-031: ");
    Serial.println(value);
  }*/

}
