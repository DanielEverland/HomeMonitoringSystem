
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <ESP8266WiFi.h>  //https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h

#define SS_PIN 4 //D2 - GPIO04
#define RST_PIN 5 //D1 - GPIO05
#define SERVO_PIN 15 // D8 - GPIO15

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
String IDs[] = {"2C76454A", "B32CDF0E", "23475B1F"}; // Martin, Martin card, Magnus.
int v = 0;
bool access = false;
Servo servo;

void setup() {
  Serial.begin(115200);   // Initialize serial communication
  SPI.begin();          // Initialize SPI bus
  mfrc522.PCD_Init();   // Initialize MFRC522 RFID reader  
  servo.attach(SERVO_PIN);
}

void loop()
{
   //servo.write(0);  
   
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) // new cards
  {
    return;
  }

  
  if ( ! mfrc522.PICC_ReadCardSerial()) // choose one of the cards
  {
    return;
  }

  // Print CARD ID on serial monitor
  Serial.print("CARD ID :");
  String value= "";  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     value.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     value.concat(String(mfrc522.uid.uidByte[i], HEX));
  }    
  Serial.println();
  value.toUpperCase();
 
// Check if the read ID is authorized
  bool authorized = false;
  for (int i = 0; i < sizeof(IDs) / sizeof(IDs[0]); i++) 
  {
    if (value.substring(0) == IDs[i]) {
      authorized = true;
      break;
    }
  }

  if (authorized)
  {
    Serial.println("Authorized access");
    access = true;
    if(value.substring(0) == IDs[0])
    {
     Serial.println("Access: Martin");
    
     servo.write(180);
     delay(2000);
     servo.write(0);     
    }
    if(value.substring(0) == IDs[2])
    {
     Serial.println("Access: Magnus");
    }
    
  } 
  else 
  {
    Serial.println("Access denied");
    access = false;
    
  }
  
  delay(700);
  Serial.println();
 
}
