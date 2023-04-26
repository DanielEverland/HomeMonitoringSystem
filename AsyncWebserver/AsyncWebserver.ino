/*
  Initial ASyncWebServer setup credit: https://randomnerdtutorials.com/esp8266-dht11dht22-temperature-and-humidity-web-server-with-arduino-ide/
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Header file containing website HTML code
#include "Website.generated.h"

// Network credentials
const char* ssid = "Galaxy A53";
const char* password = "expeditious";

// Initialize variables to hold data from nodes
float t = 0.0;
float h = 0.0;
float l = 0.0;
String motionDetection = "";
String keypads = "";
String rfidStatus = "";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// WiFi server for connecting clients on port 81
WiFiServer wifiServer(81);
WiFiClient browser;
WiFiClient temperetureClient;
WiFiClient proximityClient;
WiFiClient rfidClient;
//WiFiClient 
//WiFiClient
IPAddress ip(192, 168, 127, 85);
IPAddress gateway(192, 168, 127, 254);
IPAddress subnet(255, 255, 255, 0);

// Names of nodes
const String proximityName = "ProximityNode";
const String tempHumLightNode = "TemHumLightNode";
const String tempHumNode = "TemperatureNode";
const String keypadName = "KeypadNode";
const String rfidName = "RFIDNode";

// Function used to initialize values on website on initial load
String processor(const String &var) {
  if (var == "TEMPERATURE") {
    return String(t);
  } else if (var == "HUMIDITY") {
    return String(h);
  } else if (var == "MOTION") {
    return motionDetection;
  } else if (var == "LIGHT") {
    return String(l);
  }
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  // Connect to Wi-Fi
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connection established");
  wifiServer.begin();

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Use the processor() as a callback function when web root is loaded (to initialize data on load)
    request->send_P(200, "text/html", WEBSITE, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.on("/motionDetection", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", motionDetection.c_str());
  });
  server.on("/lightDetection", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(l).c_str());
  });
  server.on("/rfid", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", rfidStatus.c_str());
  });
  server.on("/keypad", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", keypads.c_str());
  });
  server.on("/on", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // /on is called when lock is called to open
    rfidClient.println("c[Server]c: r[Open]r");
    request->send(200, "text/plain", "ok");
  });
  

  // Start server
  server.begin();
}

void loop() {
  clientRequest();
}

void handleTemperatureInput(String request) {
  t = strtof(getClientSubstring(request, "t").c_str(), nullptr);
  h = strtof(getClientSubstring(request, "h").c_str(), nullptr);
  l = strtof(getClientSubstring(request, "l").c_str(), nullptr);
}

void handleProximityInput(String request) {
  motionDetection = getClientSubstring(request, "m");
}

void handleRFIDInput(String request) {
  rfidStatus = getClientSubstring(request, "a");
}

void handleRfidInput(String request)
{
  motionDetection = getClientSubstring(request, "c");
}

void handleKeypadInput(String request) {
  keypads = getClientSubstring(request, "k");
}

void clientRequest() {
  //Check if client connected
  WiFiClient client = wifiServer.available();
  client.setTimeout(50);
  if (client) {
    if (client.connected()) {
      // Print client IP address
      /*Serial.print(" ->");
      Serial.println(client.remoteIP());*/

      String request = client.readStringUntil('\r');  //receives the message from the client, use '\r' as String terminator
      Serial.print("Request: ");
      Serial.println(request);

      String clientName = getClientSubstring(request, "c");
      if (clientName == tempHumLightNode) {
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
      } else if (clientName == keypadName) {
        handleKeypadInput(request);
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
