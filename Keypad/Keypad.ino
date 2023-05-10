/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-keypad
 */

#include <Keypad.h>
#include <ESP8266WiFi.h> 

// Define nombers of rows and colums
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // three columns

// WIFI 
#define UPDATE_TIME 1000

// Define wifi pw and name + Node name
String nom = "KeypadNode";
const char* ssid = "Galaxy A53";
const char* passwordWiFi = "expeditious";

unsigned long previousRequest = 0;
//Objects
WiFiClient host;
IPAddress server(192, 168, 66, 85); 
// WIFI END

// Define keypad array
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pins used by keypad
byte pin_rows[ROW_NUM] = {D1, D2, D3, D4}; // GIOP18, GIOP5, GIOP17, GIOP16 connect to the row pins
byte pin_column[COLUMN_NUM] = {D5, D6, D7, D8};   // GIOP4, GIOP0, GIOP2 connect to the column pins

// Using keypad.h to make keymap
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

// Keypad password
const String password = "1111"; // 
String input_password;

void setup() {
  // Setup serial
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
  // Handles the keypad through keypad.h
  handleKeypad();
  // Get messages from server if there is any
  getHostMessage();
}

void handleKeypad(){
  // Get the pressed key
  char key = keypad.getKey();

  // Logic to find out if the correct key is pressed
  if (key) {
    Serial.println(key);

    if (key == '*') { // Restart if key = *
      Serial.println("Starting over");
      input_password = ""; // clear input password
    } else if (key == 'D') { // Check password
      if (password == input_password) { // Correct key
        Serial.println("The password is correct, ACCESS GRANTED!");
        requestHost("ACCESS GRANTED"); // Sends message to server

      } else { // Wrong key
        Serial.println("The password is incorrect, ACCESS DENIED!");
        requestHost("ACCESS DENIED"); // Sends message to server
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
      host.println("c[" + nom + "]c" + "k[" + Message + "]k \r");
    }
  }
}

void getHostMessage() { // Function to get messages from host.
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

String getSubstring(String request, String identifier) { // Unpack message from host
  return request.substring(request.indexOf(identifier + "[") + 2, request.lastIndexOf("]" + identifier));
}
