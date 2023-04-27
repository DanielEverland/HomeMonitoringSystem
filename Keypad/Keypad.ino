/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-keypad
 */

#include <Keypad.h>
#include <ESP8266WiFi.h> 

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // three columns

// WIFI
#define UPDATE_TIME 1000

String nom = "KeypadNode";
//const char* ssid = "WiFimodem-9538";
//const char* passwordWiFi = "gz2gtywyzn";
const char* ssid = "Galaxy A53";
const char* passwordWiFi = "expeditious";

unsigned long previousRequest = 0;
//Objects
WiFiClient host;
IPAddress server(192, 168, 66, 85); 
// WIFI END

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {D1, D2, D3, D4}; // GIOP18, GIOP5, GIOP17, GIOP16 connect to the row pins
byte pin_column[COLUMN_NUM] = {D5, D6, D7, D8};   // GIOP4, GIOP0, GIOP2 connect to the column pins

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const String password = "1111"; // change your password here
String input_password;

void setup() {
  Serial.begin(9600);
  input_password.reserve(32); // maximum input characters is 33, change if needed

     // WIFI SETIP
   WiFi.begin(ssid, passwordWiFi);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.print(nom);
  Serial.print(F(" connected to Wifi! IP address : "));
  Serial.println(WiFi.localIP());  // Print the IP address
  // WIFI END
  
}

void loop() {
  handleKeypad();
  getHostMessage();
}

void handleKeypad(){
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == '*') {
      Serial.println("Starting over");
      input_password = ""; // clear input password
    } else if (key == 'D') {
      if (password == input_password) {
        Serial.println("The password is correct, ACCESS GRANTED!");
        requestHost("ACCESS GRANTED");

      } else {
        Serial.println("The password is incorrect, ACCESS DENIED!");
        requestHost("ACCESS DENIED");
      }

      input_password = ""; // clear input password
    } else {
      input_password += key; // append new character to input password string
    }
  }
}

void requestHost(String Message) { /* function requestMaster */
  ////Request to host
  if ((millis() - previousRequest) > UPDATE_TIME) {  // client connect to server every 500ms
    previousRequest = millis();
    if (host.connect(server, 81)) {  // Connection to the server
      host.println(": d[" + Message + "]d \r");
    }
  }
}

void getHostMessage() {
  host.setTimeout(200);
  if (host.connected()) {
  String hostMsg = host.readString();
  if (!hostMsg.isEmpty()) {
    Serial.println(hostMsg);
//      String hostSubstring = getSubstring(hostMsg, "h");
//      if (hostSubstring == "Open") {
//        Serial.println("Wuhuuu");
//      }
    }
  }
}

String getSubstring(String request, String identifier) {
  return request.substring(request.indexOf(identifier + "[") + 2, request.lastIndexOf("]" + identifier));
}
