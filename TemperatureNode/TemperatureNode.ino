//Libraries
#include <ESP8266WiFi.h>  //https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <DHT.h>


//Constants
#define UPDATE_TIME 1000
#define DHTTYPE DHT11
//Parameters
String nom = "TemperatureNode";
const char* ssid = "Galaxy A53";
const char* password = "expeditious";

//Variables
unsigned long previousRequest = 0;
//Objects
WiFiClient host;
IPAddress server(192, 168, 127, 85);
// Initialize DHT sensor.
// From left to right facing the sensor:
// S, VCC, GND
const int DHTPin = D3;
DHT dht(DHTPin, DHTTYPE);
float t;
float h;
float f;

const int pResistor = A0;
int lightValue;
String lightDetected;

void setup() {
  pinMode(pResistor, INPUT);

  dht.begin();
  //Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  //Init ESP8266 Wifi

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.print(nom);
  Serial.print(F(" connected to Wifi! IP address : "));
  Serial.println(WiFi.localIP());  // Print the IP address
}

void loop() {
  if ((millis() - previousRequest) > UPDATE_TIME) {
    previousRequest = millis();
    dhtSensorLoop();
    lightSensorLoop();
    requestHost();
  }
}

void requestHost() {
  if (host.connect(server, 81)) {
    host.println("c[" + nom + "]c" + ": Current temperature is t[" + t + "]t Current humidity is h[" + h + "]h Current light level is: l[" + lightValue + "]l Light detected: ld[ " + lightDetected + " ]ld \r");
  }
}

void dhtSensorLoop() {
  // Read humidity
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit
  f = dht.readTemperature(true);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t Temperature: ");
  Serial.print(t);
  Serial.print(" *C %\t Fahrenheit: ");
  Serial.println(f);
}

void lightSensorLoop() {
  lightValue = analogRead(pResistor);
  if (lightValue > 100) {
    lightDetected = "Light detected";
  } else {
    lightDetected = "No light detected";
  }
  Serial.print(lightValue);
  Serial.print(" | ");
  Serial.println(lightDetected);
}
