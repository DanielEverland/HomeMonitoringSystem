 /*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/
*********/

// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Website.generated.h"

//void clientRequest();
//String getClientSubstring();

// Replace with your network credentials
const char* ssid = "Galaxy A53";
const char* password = "expeditious";

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
String motionDetection = "";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// WiFi server for connecting clients
WiFiServer wifiServer(81);
WiFiClient browser;
WiFiClient temperetureClient;
WiFiClient proximityClient;
WiFiClient rfidClient;
//WiFiClient 
//WiFiClient
IPAddress ip(192, 168, 158, 20);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);

// Names of nodes
const String proximityName = "ProximityNode";
const String tempHumNode = "TemperatureNode";
const String rfidName = "RFIDNode";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }

  wifiServer.begin();

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", WEBSITE, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.on("/motionDetection", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", motionDetection.c_str());
  });

   // Receive an HTTP GET request
  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(LED_BUILTIN, HIGH);
    rfidClient.println("c[Server]c: r[Open]r");
    request->send(200, "text/plain", "ok");
  });
  

  // Start server
  server.begin();
}
 
void loop(){  
  clientRequest();
}

void handleTemperatureInput(String request)
{
  t = strtof(getClientSubstring(request, "t").c_str(), nullptr);
  h = strtof(getClientSubstring(request, "h").c_str(), nullptr);
}

void handleProximityInput(String request)
{
  motionDetection = getClientSubstring(request, "m");
}

void handleRfidInput(String request)
{
  motionDetection = getClientSubstring(request, "c");
}

void clientRequest() { /* function clientRequest */
  ////Check if client connected
  WiFiClient client = wifiServer.available();
  client.setTimeout(50);
  if (client) {
    if (client.connected()) {
      //Print client IP address
      /*Serial.print(" ->");
      Serial.println(client.remoteIP());*/

      String request = client.readStringUntil('\r');  //receives the message from the client
      Serial.print("Request: ");
      Serial.println(request);

      String clientName = getClientSubstring(request, "c");
      if(clientName == tempHumNode)
      {
        handleTemperatureInput(request);
        temperetureClient = client;
      }
      else if(clientName == proximityName)
      {
        handleProximityInput(request);
        proximityClient = client;
      }
      else if(clientName == rfidName)
      {
        handleRfidInput(request);
        rfidClient = client;
      }
/*
      else if( clientName == keypadName){
         clientName == client
        rfidClient.println("fuckoff")
      }*/

      else
      {
        Serial.println("ERROR: Could not identify client name " + clientName);
      }
    }
  }
}

String getClientSubstring(String request, String identifier) {
  return request.substring(request.indexOf(identifier + "[") + 2, request.lastIndexOf("]" + identifier));
}