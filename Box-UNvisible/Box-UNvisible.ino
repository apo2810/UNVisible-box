#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "SPI.h" // SPI
#include "MFRC522.h" // RFID
#include "SoftwareSerial.h"
#include "DFPlayer_Mini_Mp3.h"

#define SS_PIN 10
#define RST_PIN 9

byte readCard[4];
String cardID = "ccdb7689"; // remplacer par l'ID de votre tag
String tagID = "";
SoftwareSerial mySerial(7, 8); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

// Déclaration 
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Tableau contentent l'ID
byte nuidPICC[4];


void setup()
{ 
  // Init RS232
  Serial.begin(9600);

  // Init SPI bus
  SPI.begin(); 
  mfrc522.PCD_Init(); 
  
  //mp3
   mySerial.begin(9600);
    mp3_set_serial(mySerial);
    mp3_set_volume(30);       // fixe le son (30 maximum)
    mp3_set_EQ(0);            // equalizer de 0 à 5
}

void loop() 
{
  while (getID()) {
    if (tagID == cardID) {
      mp3_next();  // joue mp3/0001.mp3
    }
    else if(tagID == "49315499");{
      mp3_prev();
    }
    
    Serial.print("ID: ");
    Serial.println(tagID);
    delay(2000);
    digitalWrite(6, LOW);
  }
}

boolean getID() {
  if (! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if (! mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  tagID = "";
  
  for (uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}
