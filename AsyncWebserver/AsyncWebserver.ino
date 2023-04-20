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


//JEG ER SEJ

// Replace with your network credentials
const char* ssid = "Galaxy A53";
const char* password = "expeditious";


// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
String motionDetection = "";


const int output = 2;

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head> //Underneath we see the background some of the background setup, and a bit of the button setup. 
         //Most of this has to be integrated into the "pretty" website, as this is just a button and a white background
   
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #c9c9c9;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  //Setting different button colors for active state
      .button:hover {background-color: #c9c9c9}
      .button:active {
        background-color: #029ef7;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
    <h1>ESP Pushbutton Web Server</h1>
    //Here we create the button swith on the website. 
    //The onXXXX are event attributes, that specify what happens on the specific events. And the togglecheckbox simply states what state the LED is in at the given event.
    <button class="button" onmousedown="toggleCheckbox('on');" ontouchstart="toggleCheckbox('on');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('on');">LED PUSHBUTTON</button>
   <script>
   //This function simply sends the commands from the above code to the webserver.
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}



// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// WiFi server for connecting clients
WiFiServer wifiServer(81);
WiFiClient browser;
IPAddress ip(192, 168, 158, 20);
IPAddress gateway(192, 168, 1, 254);
IPAddress subnet(255, 255, 255, 0);


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
  

  // Start server
  server.begin();
}
 
void loop(){  
  clientRequest();
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

      t = strtof(request.substring(request.indexOf("t[") + 2, request.lastIndexOf("]t")).c_str(), nullptr);
      h = strtof(request.substring(request.indexOf("h[") + 2, request.lastIndexOf("]h")).c_str(), nullptr);
      motionDetection = strtof(request.substring(request.indexOf("m[") + 2, request.lastIndexOf("]m")).c_str(), nullptr);
    }
  }
}
