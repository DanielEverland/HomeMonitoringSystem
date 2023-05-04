//Libraries
#include <ESP8266WiFi.h>  //https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <DHT.h>

//Constants
#define UPDATE_TIME 1000
#define DHTTYPE DHT11

// Name of this peer on the network. Used in requests.
const String peerName = "TemHumLightNode";

// The name of the wifi network to connect to
const char* wifiName = "Galaxy A53";

// The password of the wifi network to connect to
const char* wifiPassword = "expeditious";

// The time the previous request was sent
unsigned long previousRequest = 0;

// The server this node connects to
WiFiClient host;

// The IP of the server to connect to
IPAddress server(192, 168, 66, 85);

// Initialize DHT sensor.
// From left to right facing the sensor:
// S, VCC, GND
const int DHTPin = D3;
DHT dht(DHTPin, DHTTYPE);

// Contains the measured temperature in celsius
float temperature;

// Contains the measured humidity
float humidity;

// Contains the light-level
int lightValue;

// The pin used for the light sensor
const int pResistor = A0;

void setup() {
  pinMode(pResistor, INPUT);

  dht.begin();
  //Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  //Init ESP8266 Wifi

  WiFi.begin(wifiName, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.print(peerName);
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
    // Remove light value if unused
    host.println("c[" + peerName + "]c" + ": Current temperature is t[" + temperature + "]t Current humidity is h[" + humidity + "]h Current light level is: l[" + lightValue + "]l \r");
  }
}

void dhtSensorLoop() {
  // Read humidity
  humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temperature = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t Temperature: ");
  Serial.print(temperature);
}

void lightSensorLoop() {
  lightValue = analogRead(pResistor);
  Serial.print(lightValue);
}
