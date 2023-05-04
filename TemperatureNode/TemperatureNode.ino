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
const int lightSensorPin = A0;

void setup() {
  // Init sensor
  pinMode(lightSensorPin, INPUT);
  dht.begin();

  // Init Serial USB
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  
  // Init ESP8266 Wifi
  WiFi.begin(wifiName, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.print(peerName);
  Serial.print(F(" connected to Wifi! IP address : "));
  Serial.println(WiFi.localIP());  // Print the IP address
}

// Main loop
void loop() {
  // If enough time has passed, do an update
  if ((millis() - previousRequest) > UPDATE_TIME) {

    // Log the time this update happened
    previousRequest = millis();

    // Query temperature and humidity
    queryTemperatureHumidity();

    // Query light levels
    queryLightSensor();

    // Send data to server
    requestHost();
  }
}

void requestHost() {
  if (host.connect(server, 81)) {
    // Actually send the formatted request to the host with client name, temperature, humidity and light levels
    host.println("c[" + peerName + "]c" + ": Current temperature is t[" + temperature + "]t Current humidity is h[" + humidity + "]h Current light level is: l[" + lightValue + "]l \r");
  }
}

// Query temperature and humidity
void queryTemperatureHumidity() {
  // Read humidity
  humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temperature = dht.readTemperature();

  // Debug printing
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t Temperature: ");
  Serial.print(temperature);
}

// Query light sensor
void queryLightSensor() {
  // Read light value from sensor
  lightValue = analogRead(lightSensorPin);

  // Debug print light level
  Serial.print(lightValue);
}
