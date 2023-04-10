//https://diyi0t.com/keypad-arduino-esp8266-esp32/

#include "Keypad.h"
#include "Wire.h"
// #include "LiquidCrystal_I2C.h"

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// For Arduino Microcontroller
//byte rowPins[ROWS] = {9, 8, 7, 6}; 
//byte colPins[COLS] = {5, 4, 3, 2}; 

// For ESP8266 Microcontroller
byte rowPins[ROWS] = {1, 2, 3, 4}; 
byte colPins[COLS] = {5, 6, 7, 8}; 

// For ESP32 Microcontroller
//byte rowPins[ROWS] = {23, 22, 3, 21}; 
//byte colPins[COLS] = {19, 18, 5, 17};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//LiquidCrystal_I2C lcd(0x27, 20, 4); 

const int len_key = 5;
char master_key[len_key] = {'1','2','3','4','1'};
char attempt_key[len_key];
int z=0;

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  char key = keypad.getKey();
  Serial.print("*");
  if (key){
    switch(key){
      case '*':
        z=0;
        break;
      case '#':
        delay(100); // added debounce
        checkKEY();
        break;
      default:
         attempt_key[z]=key;
         z++;
      }
  }
}

void checkKEY()
{
   int correct=0;
   int i;
   for (i=0; i<len_key; i++) {
    if (attempt_key[i]==master_key[i]) {
      correct++;
      }
    }
   if (correct==len_key && z==len_key){
    Serial.print("Correct Key");
    delay(3000);
    z=0;
    
    Serial.print("Insert Password");
   }
   else
   {

    Serial.print("Incorrect Key");
    delay(3000);
    z=0;
    Serial.print("Insert Password");
   }
   for (int zz=0; zz<len_key; zz++) {
    attempt_key[zz]=0;
   }
}
