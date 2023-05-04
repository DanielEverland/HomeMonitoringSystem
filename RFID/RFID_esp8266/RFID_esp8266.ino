
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <ESP8266WiFi.h>  //https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/ESP8266WiFi.h


// PIN setup
#define SS_PIN 4 //D2 - GPIO04
#define RST_PIN 5 //D1 - GPIO05
#define SERVO_PIN 15 // D8 - GPIO15
#define UPDATE_TIME 1000
#define BUZZER_PIN 16 //D0 - GPIO16

//NOTE for the Buzzer 
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_DB3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_EB3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST     0


MFRC522 mfrc522(SS_PIN, RST_PIN);   // MFRC522

String IDs[] = {"2C76454A", "B32CDF0E", "23475B1F"}; // Martin, Martin card, Magnus - Approved Key tags
int v = 0;

//Variables 
String nom = "RFIDNode";
const char* ssid = "Galaxy A53";
const char* password = "expeditious";
unsigned long previousRequest = 0;

//Objects
Servo servo;
WiFiClient host;
IPAddress server(192, 168, 66, 85); 

// Star wars sound from - https://github.com/hibit-dev/buzzer/tree/master/src/movies/star_wars
int Smelody[] =
{
  NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_F5, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_F5, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,

  NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
  NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5, NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,

  NOTE_C6, NOTE_G5, NOTE_G5, REST, NOTE_C5,
  NOTE_D5, NOTE_D5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F5,
  NOTE_F5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_D5, NOTE_E5, NOTE_C6, NOTE_C6,
  NOTE_F6, NOTE_DS6, NOTE_CS6, NOTE_C6, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_F5,
  NOTE_C6
};

int Sdurations[] =
{
  8, 8, 8,
  2, 2,
  8, 8, 8, 2, 4,
  8, 8, 8, 2, 4,
  8, 8, 8, 2, 8, 8, 8,
  2, 2,
  8, 8, 8, 2, 4,

  8, 8, 8, 2, 4,
  8, 8, 8, 2, 8, 16,
  4, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 4, 8, 16,
  4, 8, 8, 8, 8, 8,

  8, 16, 2, 8, 8,
  4, 8, 8, 8, 8, 8,
  8, 8, 8, 4, 8, 4, 8, 16,
  4, 8, 4, 8, 4, 8, 4, 8,
  1
};
// Pirates of Caribbean sound from - https://github.com/hibit-dev/buzzer/tree/master/src/movies/pirates_of_the_caribbean
int Pmelody[] =
{
  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REST,

  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_D5, NOTE_E5, NOTE_A4, REST,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REST,
  NOTE_C5, NOTE_A4, NOTE_B4, REST,

  NOTE_A4, NOTE_A4,
  //Repeat of first part
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
  NOTE_A4, NOTE_G4, NOTE_A4, REST,

  NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
  NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, REST,
  NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, REST,
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, REST,

  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
  NOTE_D5, NOTE_E5, NOTE_A4, REST,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, REST,
  NOTE_C5, NOTE_A4, NOTE_B4, REST,
  //End of Repeat

  NOTE_E5, REST, REST, NOTE_F5, REST, REST,
  NOTE_E5, NOTE_E5, REST, NOTE_G5, REST, NOTE_E5, NOTE_D5, REST, REST,
  NOTE_D5, REST, REST, NOTE_C5, REST, REST,
  NOTE_B4, NOTE_C5, REST, NOTE_B4, REST, NOTE_A4,

  NOTE_E5, REST, REST, NOTE_F5, REST, REST,
  NOTE_E5, NOTE_E5, REST, NOTE_G5, REST, NOTE_E5, NOTE_D5, REST, REST,
  NOTE_D5, REST, REST, NOTE_C5, REST, REST,
  NOTE_B4, NOTE_C5, REST, NOTE_B4, REST, NOTE_A4
};

// Pirates of Caribbean sound from - https://github.com/hibit-dev/buzzer/tree/master/src/movies/pirates_of_the_caribbean
int Pdurations[] = {
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,

  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,

  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 8, 4, 8,

  8, 8, 4, 8, 8,
  4, 8, 4, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 4,

  4, 8,
  //Repeat of First Part
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,

  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8,

  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 8, 8,
  8, 8, 8, 4, 8,

  8, 8, 4, 8, 8,
  4, 8, 4, 8,
  8, 8, 4, 8, 8,
  8, 8, 4, 4,
  //End of Repeat

  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 4,
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 2,

  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 8, 8, 8, 4,
  4, 8, 4, 4, 8, 4,
  8, 8, 8, 8, 8, 2
};



void setup()
{
  Serial.begin(115200);   // Initialize serial communication
  SPI.begin();            // Initialize SPI bus
  mfrc522.PCD_Init();     // Initialize MFRC522 RFID reader
  servo.attach(SERVO_PIN);

  //Init ESP8266 Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }
  Serial.print(nom);
  Serial.print(F(" connected to Wifi! IP address : "));
  Serial.println(WiFi.localIP());  // Print the IP address

  //Init Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
}


// Pirates of Caribbean sound from - https://github.com/hibit-dev/buzzer/tree/master/src/movies/pirates_of_the_caribbean
void Pirates()
{
  int size = sizeof(Pdurations) / sizeof(int);

  for (int i = 0; i < size; i++) 
  {  
    int duration = 1000 / Pdurations[i];      //Calculate durations
    tone(BUZZER_PIN, Pmelody[i], duration);   //Play tone from melodyArray
 
    int pauseBetweenNotes = duration * 1.30;  //Calculate the pause between notes
    delay(pauseBetweenNotes);                 //Delay as pause

    //stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

// Start wars sound from - https://github.com/hibit-dev/buzzer/tree/master/src/movies/star_wars
void playStarWars()
{
  int size = sizeof(Sdurations) / sizeof(int);
  for (int i = 0; i < size; i++)
  {
    int duration = 1000 / Sdurations[i];     //Calculate durations
    tone(BUZZER_PIN, Smelody[i], duration);  //Play tone from melodyArray

    int pauseBetweenNotes = duration * 1.30; //Calculate the pause between notes
    delay(pauseBetweenNotes);                //Delay as pause

    //stop the tone playing:
    noTone(BUZZER_PIN);
  }
}

void checkRFID()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) // new cards
  {
    return;
  }


  if ( ! mfrc522.PICC_ReadCardSerial()) // choose one of the cards
  {
    return;
  }

  
  Serial.print("CARD ID :");                      // Print CARD ID on serial monitor
  String value = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)     //Loop through key tag ID
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    value.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    value.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  value.toUpperCase();

  
  bool authorized = false;
  for (int i = 0; i < sizeof(IDs) / sizeof(IDs[0]); i++) // Check if the read ID is authorized
  {
    if (value.substring(0) == IDs[i])
    {
      authorized = true;
      break;
    }
  }

  if (authorized)                       // If Key is approved send message to Server 
  {
    Serial.println("Authorized access");
    if (value.substring(0) == IDs[0])
    {
      Serial.println("Access Martin");  //Print message to Terminal
      requestHost("Access Martin");     //send message to Server

    }
    if (value.substring(0) == IDs[2])
    {
      Serial.println("Access Magnus");   //Print message to Terminal
      requestHost("Access Magnus");      //send message to Server
    }

  }
  else                                  //If Key is denined
  {
    Serial.println("Access denied");    //Print message to Terminal
    requestHost("Access denied");       //send message to Server

  }
}

void runServo()                         // Function to Run the servo Motor (Door lock simmulation) 
{
  servo.write(180);
  delay(2000);
  servo.write(0);
}

void playError()                        // Function to play Error sound
{
  tone(BUZZER_PIN, NOTE_G4);
  delay(250);
  tone(BUZZER_PIN, NOTE_C4);
  delay(500);
  noTone(BUZZER_PIN);
}


void loop()                             // Main Loop
{
  checkRFID();
  getHostMessage();
}


void requestHost(String msg)            // Function to send message to Server
{ /* function requestMaster */
  ////Request to host
  if ((millis() - previousRequest) > UPDATE_TIME)
  { // client connect to server every 500ms
    previousRequest = millis();
    if (host.connect(server, 81))
    { // Connection to the server
      host.println("c[" + nom + "]c" + ":  a[" + msg + "]a \r");
    }
  }
}

void getHostMessage()                   //Function to get message from the Server
{
  host.setTimeout(200);
  if (host.connected()) {
    String hostMsg = host.readString();
    if (!hostMsg.isEmpty()) {
      Serial.println(hostMsg);
      String hostSubstring = getSubstring(hostMsg, "r");
      handleHostSubstring(hostSubstring);
    }
  }
}

void handleHostSubstring(String hostSubstring)  //Function to handle message from server
{
  if (hostSubstring == "Open") // From Button on webpage
      {
        runServo();
      }
      else if (hostSubstring == "Access Martin") //RFID ACCESS MARTIN
      {
        Serial.println(hostSubstring);
        runServo();
        delay(300);
        playStarWars();
      }
      else if (hostSubstring == "Access Magnus") //RFID ACCESS MAGNUS
      {
        Serial.println(hostSubstring);
        runServo();
        delay(300);
        Pirates();
      }
      else if (hostSubstring == "Access OK") //ACCESS Keypad
      {
        runServo();
        delay(300);
        //playStarWars();
      }
      else
      {
        Serial.println(hostSubstring); //NO ACCESS
        delay(300);
        //PLAY ERROR SOUND!
        playError();
      }
}

String getSubstring(String request, String identifier) 
{
  return request.substring(request.indexOf(identifier + "[") + 2, request.lastIndexOf("]" + identifier));
}
