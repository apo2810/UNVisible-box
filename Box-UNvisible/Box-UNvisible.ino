#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define SS_PIN 10
#define RST_PIN 9
#define DFPLAYER_RX_PIN 7
#define DFPLAYER_TX_PIN 8

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Initialisation du module RFID
SoftwareSerial mySoftwareSerial(DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);  // Initialisation du port série pour le lecteur MP3
DFRobotDFPlayerMini myDFPlayer;  // Création d'une instance du lecteur MP3

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Erreur lors de la communication avec le lecteur MP3 !");
    while (true);
  }

  Serial.println("Prêt !");
  playStartupSound();  // Joue une musique au démarrage
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      cardUID += String(mfrc522.uid.uidByte[i], HEX);
      cardUID.toUpperCase();
    }
    playSoundFromCardUID(cardUID);  // Joue la musique correspondante
    delay(1000);  // Attente pour éviter de lire plusieurs fois la même carte
  }
}




void playSound(uint16_t soundNumber) {
  myDFPlayer.play(soundNumber);
  delay(500);  // Attente pour laisser le temps au lecteur MP3 de démarrer la lecture
  while (myDFPlayer.available()) {
    myDFPlayer.readType();
  }
}
void playSoundFromCardUID(String cardUID) {
  if (cardUID == "49315499") {
    playSound(1);  // Joue la piste 1 du lecteur MP3
  } else if (cardUID == "CCDB7689") {
    playSound(2);  // Joue la piste 2 du lecteur MP3
  } else if (cardUID == "15550C09") {
    playSound(3);  // Joue la piste 3 du lecteur MP3
  } else if (cardUID == "644CF11D") {
    playSound(4);  // Joue la piste 4 du lecteur MP3
  } else if (cardUID == "5B1D37BB") {
    playSound(5);  // Joue la piste 5 du lecteur MP3
  } else {
    Serial.println("Carte UID non reconnue !");
  }
}

void playStartupSound() {
  playSound(0);  // Joue la piste 0 du lecteur MP3 (musique de démarrage)
}
