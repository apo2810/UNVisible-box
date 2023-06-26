#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#define NUM_LEDS        12        // Nombre de LED dans le cercle
#define DATA_PIN        2         // Broche de données de la LED
#define BOUTON_PIN      1
#define BOUTON_PIN2     3
#define SS_PIN 10
#define RST_PIN 9
#define DFPLAYER_RX_PIN 7
#define DFPLAYER_TX_PIN 8

Adafruit_NeoPixel leds(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Initialisation du module RFID
SoftwareSerial mySoftwareSerial(DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);  // Initialisation du port série pour le lecteur MP3
DFRobotDFPlayerMini myDFPlayer;  // Création d'une instance du lecteur MP3

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  leds.begin();               // Initialise la bibliothèque NeoPixel
  leds.show();                // Éteint toutes les LED au démarrage
  digitalWrite(BOUTON_PIN,HIGH);
  pinMode(BOUTON_PIN, INPUT); 
  digitalWrite(BOUTON_PIN2,HIGH);
  pinMode(BOUTON_PIN2, INPUT);  
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
    delay(10000);  // Attente pour éviter de lire plusieurs fois la même carte
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
    if (digitalRead(BOUTON_PIN) == LOW) {  // Vérifie si le bouton est appuyé
    allumerCercleLED(0, 255, 0);         // Allume le cercle de LED en vert
    delay(3000);                         // Attend 3 secondes
    leds.clear();                        // Éteint toutes les LED
    leds.show();                         // Met à jour l'affichage des LED
    }
    else if (digitalRead(BOUTON_PIN2) == LOW) {  // Vérifie si le bouton est appuyé
    allumerCercleLED(255, 0, 0);         // Allume le cercle de LED en vert
    delay(3000);                         // Attend 3 secondes
    leds.clear();                        // Éteint toutes les LED
    leds.show();                         // Met à jour l'affichage des LED
    }
    else {  
    allumerCercleLED(0, 0, 0);         // Allume le cercle de LED en vert                 
    leds.clear();                        // Éteint toutes les LED
  }
  } else if (cardUID == "5B1D37BB") {
    playSound(1);  // Joue la piste 2 du lecteur MP3
  } else if (cardUID == "CCDB7689") {
    playSound(3);  // Joue la piste 3 du lecteur MP3
  } else if (cardUID == "644CF11D") {
    playSound(4);  // Joue la piste 4 du lecteur MP3
  } else if (cardUID == "15550C09") {
    playSound(5);  // Joue la piste 5 du lecteur MP3
  } else {
    Serial.println("Carte UID non reconnue !");
  }
}

void playStartupSound() {
  playSound(0);  // Joue la piste 0 du lecteur MP3 (musique de démarrage)
}
void allumerCercleLED(byte r, byte g, byte b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, r, g, b);  // Allume chaque LED avec la couleur spécifiée
  }
  leds.show();                       // Met à jour l'affichage des LED
}
