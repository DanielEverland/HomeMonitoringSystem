//Libraries
#include <ESP8266WiFi.h>//https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h
#include <DHT.h>

//Constants
#define LED D4
#define UPDATE_TIME 500
#define DHTTYPE DHT11  // DHT 11
//Parameters
String nom = "Client0";
const char* ssid = "Galaxy A53";
const char* password = "expeditious";
//Variables
String command;
unsigned long previousRequest = 0;
//Objects
WiFiClient master;
IPAddress server(192, 168, 127, 242);
float t;

// Initialize DHT sensor.
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
 	Serial.print(F(" connected to Wifi! IP address : ")); Serial.println(WiFi.localIP()); // Print the IP address
 	pinMode(LED, OUTPUT);
}

void loop() {
  dhtSensorLoop();
 	requestMaster();
}

void requestMaster( ) { /* function requestMaster */
 	////Request to master
 	if ((millis() - previousRequest) > UPDATE_TIME) { // client connect to server every 500ms
 			previousRequest = millis();
 			if (master.connect(server, 80)) { // Connection to the server
 					master.println(nom + ": Hello! my current state is x" + String(!digitalRead(LED)) + "xt" + t + "t \r");
 					//answer
 					String answer = master.readStringUntil('\r'); 		// receives the answer from the sever
 					master.flush();
 					Serial.println("from " + answer);
 					if (answer.indexOf("x") >= 0) {
 							command = answer.substring(answer.indexOf("x") + 1, answer.length());
 							Serial.print("command received: "); Serial.println(command);
 							if (command == "1") {
 									Serial.println("LED ON");
 									digitalWrite(LED, LOW);
 							} else {
 									Serial.println("LED OFF");
 									digitalWrite(LED, HIGH);
 							}
 					}
 			}
 	}
}

void dhtSensorLoop()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
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
    Serial.print(f);
  }
}
