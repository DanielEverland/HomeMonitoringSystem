//Libraries
#include <ESP8266WiFi.h> 

//Constants
#define UPDATE_TIME 1000

String nom = "ProximityNode"; //Name for identification, used in requests.
const char* ssid = "Galaxy A53"; //Name of the WIFI.
const char* password = "expeditious"; //Password to said WIFI.

int inputPin = D7; // Sets digital pin D7 as input pin, such that we can recieve a signal whenever motion is being detected.

unsigned long previousRequest = 0; //Initial value for when previous request was sent.



WiFiClient host; //Telling this node what the name of the server node is, and letting it know that is host aswell.
IPAddress server(192, 168, 66, 85); //IP address the server.


//General setup for connecting to the server(Host).
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
 
 //Actual code that handles the proximity sensor.
void loop(){
  int val = digitalRead(inputPin); //Tells the esp to read on D7.
  Serial.println(val);
  if (val == HIGH) { //If statement checking if a High values has been recieved on D7
    Serial.println("Motion detected!"); //If D7 is high, motion has been detected and prints "Motion detected!".
    requestHost("detected"); //Sends a request to the host that the node has detected motion, and updates the information on the server.
    }
  else { //Else statement, that if nothing else is recieved, no other action should be taken.
    Serial.println("No Motion detected!"); //Standard message printing "No motion detected!" if D7 is low.
    requestHost("not detected"); //Sends request to the host, that no motion has been detected. Updates on the server.
    }
    
delay(10); //Delay such that it updates every 10 ms 
}

void requestHost(String Message) { /* function requestMaster */
  ////Request to host
  if ((millis() - previousRequest) > UPDATE_TIME) {  // client connect to server every 500ms
    previousRequest = millis();
    if (host.connect(server, 81)) {  // Connection to the server

    //Sends the formatted request to the host, along with client name, and whether or not motion has been detected
      host.println("c[" + nom + "]c" + " motion m[" + Message + "]m \r"); 
    }
  }
}
