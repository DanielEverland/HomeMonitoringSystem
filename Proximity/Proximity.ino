// https://diyi0t.com/hc-sr501-pir-motion-sensor-arduino-esp8266-esp32/
//int inputPin = 7; // for Arduino microcontroller
int inputPin = D7; // for ESP8266 microcontroller
//int inputPin = 4; // for ESP32 microcontroller
// HEJ
// HEJ
void setup() {
  pinMode(inputPin, INPUT);
  Serial.begin(9600);

  delay(1500);
}
 
void loop(){
  int val = digitalRead(inputPin);
  Serial.println(val);
  if (val == HIGH) {
    Serial.println("Motion detected!");
    }
  else {
    Serial.println("No Motion detected!");
    }
    
delay(10);
}
