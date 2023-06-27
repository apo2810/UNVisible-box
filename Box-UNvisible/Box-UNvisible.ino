#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

#define SS_PIN 10
#define RST_PIN 9
#define DFPLAYER_RX_PIN 7
#define DFPLAYER_TX_PIN 8
#define BUTTON_1_PIN 2
#define BUTTON_2_PIN 3
#define BUTTON_3_PIN 4
#define BUTTON_4_PIN 5
#define BUTTON_5_PIN 6
#define LED_GREEN_PIN A0
#define LED_RED_PIN A1

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Initialisation du module RFID
SoftwareSerial mySoftwareSerial(DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);  // Initialisation du port série pour le lecteur MP3
DFRobotDFPlayerMini myDFPlayer;  // Création d'une instance du lecteur MP3

boolean button1Pressed = false;
boolean button2Pressed = false;
boolean button3Pressed = false;
boolean button4Pressed = false;
boolean button5Pressed = false;
boolean correctButtonPressed = false;

// Définition des UID des badges
String uidButton1 = "644CF11D";
String uidButton2 = "CCDB7689";
String uidButton3 = "15550C09";
String uidButton4 = "5B1D37BB";
String uidButton5 = "49315499";

int currentTrackNumber = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("Erreur lors de la communication avec le lecteur MP3 !");
    while (true);
  }

  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  pinMode(BUTTON_3_PIN, INPUT);
  pinMode(BUTTON_4_PIN, INPUT);
  pinMode(BUTTON_5_PIN, INPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);

  digitalWrite(BUTTON_1_PIN, HIGH);
  digitalWrite(BUTTON_2_PIN, HIGH);
  digitalWrite(BUTTON_3_PIN, HIGH);
  digitalWrite(BUTTON_4_PIN, HIGH);
  digitalWrite(BUTTON_5_PIN, HIGH);

  Serial.println("Prêt !");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      cardUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    cardUID.toUpperCase();
    playSoundFromCardUID(cardUID);  // Joue le son correspondant
    delay(1000);  // Attente pour éviter de lire plusieurs fois la même carte

    // Réinitialise l'état des boutons
    button1Pressed = false;
    button2Pressed = false;
    button3Pressed = false;
    button4Pressed = false;
    button5Pressed = false;
    correctButtonPressed = false;
    
  }

  // Vérifie l'état des boutons
  checkButton(BUTTON_1_PIN, button1Pressed, 1);
  checkButton(BUTTON_2_PIN, button2Pressed, 2);
  checkButton(BUTTON_3_PIN, button3Pressed, 3);
  checkButton(BUTTON_4_PIN, button4Pressed, 4);
  checkButton(BUTTON_5_PIN, button5Pressed, 5);
}

void playSoundFromCardUID(String cardUID) {
  if (cardUID == uidButton1) {
    playSound(1);
  } else if (cardUID == uidButton2) {
    playSound(1);
  } else if (cardUID == uidButton3) {
    playSound(3);
  } else if (cardUID == uidButton4) {
    playSound(4);
  } else if (cardUID == uidButton5) {
    playSound(5);
  } else {
    Serial.println("Carte UID non reconnue !");
  }
}

void playSound(int trackNumber) {
  if (trackNumber != currentTrackNumber) {
    currentTrackNumber = trackNumber;
    myDFPlayer.stop();
    myDFPlayer.play(trackNumber);
    delay(100);
  }
}

void checkButton(int buttonPin, boolean &buttonPressed, int buttonNumber) {
  if (digitalRead(buttonPin) == LOW && !buttonPressed) {
    buttonPressed = true;
    if (buttonNumber == currentTrackNumber) {
      Serial.println("Bouton " + String(buttonNumber) + " : Bon bouton appuyé !");
      digitalWrite(LED_GREEN_PIN, HIGH);
      digitalWrite(LED_RED_PIN, LOW);
    } else {
      Serial.println("Bouton " + String(buttonNumber) + " : Mauvais bouton appuyé !");
      digitalWrite(LED_GREEN_PIN, LOW);
      digitalWrite(LED_RED_PIN, HIGH);
    }
  } else if (digitalRead(buttonPin) == HIGH && buttonPressed) {
    buttonPressed = false;
  }
}
