

// https://diyi0t.com/hc-sr501-pir-motion-sensor-arduino-esp8266-esp32/

#include <ESP8266WiFi.h> 

#define UPDATE_TIME 1000

String nom = "ProximityNode";
const char* ssid = "Galaxy A53";
const char* password = "expeditious";

int inputPin = D7; // for ESP8266 microcontroller

unsigned long previousRequest = 0;
//Objects
WiFiClient host;
IPAddress server(192, 168, 127, 85);

void setup() {
  pinMode(inputPin, INPUT);
  Serial.begin(9600);
  
  // WIFI SETIP
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.print(nom);
  Serial.print(F(" connected to Wifi! IP address : "));
  Serial.println(WiFi.localIP());  // Print the IP address
  // WIFI END

 // delay(1500);
}
 
void loop(){
  int val = digitalRead(inputPin);
  Serial.println(val);
  if (val == HIGH) {
    Serial.println("Motion detected!");
    requestHost("detected");
    }
  else {
    Serial.println("No Motion detected!");
    requestHost("not detected");
    }
    
delay(10);
}

void requestHost(String Message) { /* function requestMaster */
  ////Request to host
  if ((millis() - previousRequest) > UPDATE_TIME) {  // client connect to server every 500ms
    previousRequest = millis();
    if (host.connect(server, 81)) {  // Connection to the server
      host.println("c[" + nom + "]c" + " motion m[" + Message + "]m \r");
    }
  }
}
