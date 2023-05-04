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
const char *ssid = "Galaxy A53";
const char *password = "expeditious";

// Initialize variables to hold data from nodes
float t = 0.0;
float h = 0.0;
float l = 0.0;
String motionStatus = "No motion";
String keypadStatus = "No access";
String rfidStatus = "No access";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// WiFi server for connecting clients on port 81
WiFiServer wifiServer(81);

// Global WiFiClient object for communication from server to RFID node
WiFiClient rfidClient;

// Define static IP, gateway and subnet for better consistency
IPAddress ip(192, 168, 66, 85);
IPAddress gateway(192, 168, 66, 254);
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
    return motionStatus;
  } else if (var == "LIGHT") {
    return String(l);
  } else if (var == "RFID Status") {
    return rfidStatus;
  } else if (var == "Keypad Status") {
    return keypadStatus;
  }
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  // Connect to Wi-Fi and define static
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  // Start wifiServer when WiFi is connected
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connection established");
  wifiServer.begin();

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Create endpoints 

  // Root initialized with the WEBSITE constant inside Website.generated.h
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Use the processor() as a callback function when web root is loaded (to initialize data on load)
    request->send_P(200, "text/html", WEBSITE, processor);
  });

  // Endpoints for nodes, on request they send a response containing the relevant data
  // For example the /temperature endpoint sends a response back containing the temperature as a C string.
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(t).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.on("/motionDetection", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", motionStatus.c_str());
  });
  server.on("/lightDetection", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(l).c_str());
  });
  server.on("/rfid", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", rfidStatus.c_str());
  });
  server.on("/keypad", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", keypadStatus.c_str());
  });

  // Endpoint used to communicate to the RFID node on request. 
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    // /on is called when lock is called to open
    rfidClient.println("c[Server]c: r[Open]r");
    // It always sends a response containing "ok", which assumes the RFID node always successfully opens the door. 
    // Improvements should be made to react to a response from RFID client that indicates whether it was successful or failed 
    request->send(200, "text/plain", "ok");
  });

  // Start server
  server.begin();
}

void loop() {
  clientRequest();
}

// The following functions are used to parse a request, extract the information and assign to the global variables relevant for the node

// Temperature node uses t, h and l for temperature, humidity and light values respectively
void handleTemperatureInput(String request) {
  t = strtof(getClientSubstring(request, "t").c_str(), nullptr);
  h = strtof(getClientSubstring(request, "h").c_str(), nullptr);
  l = strtof(getClientSubstring(request, "l").c_str(), nullptr);
}

// Proximity node sets a "detected" or "not detected" status
void handleProximityInput(String request) {
  motionStatus = getClientSubstring(request, "m");
}

// The RFID node uses specific strings to identify different users
// The uniqueness is used to play different sounds on the RFID node buzzer
void handleRFIDInput(String request) {
  rfidStatus = getClientSubstring(request, "a");
  if (rfidStatus == "Access Martin") {
    rfidClient.println("r[Access Martin]r");
  } else if (rfidStatus == "Access Magnus") {
    rfidClient.println("r[Access Magnus]r");
  } else {
    rfidClient.println("r[Access NO]r");
  }
}

// The Keypad node either grants or denies access. The RFID node receives this info to open or close the door
void handleKeypadInput(String request) {
  keypadStatus = getClientSubstring(request, "k");
  if (keypadStatus == "ACCESS GRANTED") {
    rfidClient.println("r[Access OK]r");
  } else {
    rfidClient.println("r[Access NO]r");
  }
}

void clientRequest() {
  // Check if client connected
  WiFiClient client = wifiServer.available();
  client.setTimeout(50);
  if (client) {
    if (client.connected()) {
      // Print client IP address

      String request = client.readStringUntil('\r');  // receives the message from the client, use '\r' as String terminator
      Serial.print("Request: ");
      // Print request for debugging purposes
      Serial.println(request);

      // We extract the name of the node who sent the request
      String clientName = getClientSubstring(request, "c");

      // Match the name to a node and handle the request
      if (clientName == tempHumLightNode) {
        handleTemperatureInput(request);
      } else if (clientName == proximityName) {
        handleProximityInput(request);
      } else if (clientName == keypadName) {
        handleKeypadInput(request);
      } else if (clientName == rfidName) {
        // Set the WiFiObject rfidClient
        // Improvement here would be to initialize rfidClient upon initial connection
        rfidClient = client;
        handleRFIDInput(request);
      } else {
        Serial.println("ERROR: Could not identify client name " + clientName);
      }
    }
  }
}

// Function for extracting the information wrapped inside a unique identifier in a request string
String getClientSubstring(String request, String identifier) {
  return request.substring(request.indexOf(identifier + "[") + 2, request.lastIndexOf("]" + identifier));
}
