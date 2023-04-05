//Libraries
#include <ESP8266WiFi.h>  //https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <DHT.h>

//Constants
#define LED D4
#define UPDATE_TIME 1000
#define DHTTYPE DHT11  // DHT 11
//Parameters
String nom = "Client0";
const char* ssid = "Galaxy A53";
const char* password = "expeditious";
//Variables
String command;
unsigned long previousRequest = 0;
//Objects
WiFiClient host;
IPAddress server(192, 168, 127, 85);
float t;
float h;
float f;

// Initialize DHT sensor.
// From left to right facing the sensor:
// S, VCC, GND
const int DHTPin = D3;
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];


void dhtSensorLoop();

void setup() {
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
  pinMode(LED, OUTPUT);
}

void loop() {
  dhtSensorLoop();
  requestHost();
}

void requestHost() { /* function requestMaster */
  ////Request to host
  if ((millis() - previousRequest) > UPDATE_TIME) {  // client connect to server every 500ms
    previousRequest = millis();
    if (host.connect(server, 81)) {  // Connection to the server
      host.println(nom + ": Current state is x[" + String(!digitalRead(LED)) + "]x Current temperature is t[" + t + "]t Current humidity is h[" + h + "]h \r");
    }
  }
}

void dhtSensorLoop() {
  // Read humidity
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    strcpy(celsiusTemp, "Failed");
    strcpy(fahrenheitTemp, "Failed");
    strcpy(humidityTemp, "Failed");
  } else {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.println(f);
  }
}
