#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 10, 11, 12, 13);

int num_Measure = 128;    // Anzahl der Messungen
int pinSignal = A0;       // Pin für das Mikrofon
long Sound_signal;        // Gelesene Werte vom Mikrofon
long sum = 0;             // Summe der Messwerte
long level = 0;           // Durchschnittswert der Messungen
int soundThreshold = 100; // Schwellenwert für LEDs

const int redLedPin = 4;   // Pin für die rote LED
const int greenLedPin = 5; // Pin für die grüne LED
const int startButtonPin = 2; // Pin für den Start-Button
const int stopButtonPin = 3;  // Pin für den Stop-Button

int startButtonState = 0; // Status des Start-Buttons
int stopButtonState = 0;  // Status des Stop-Buttons
bool collectingData = false; // Zustand der Datensammlung

void setup() {
  pinMode(pinSignal, INPUT);          // Mikrofon als Eingabe
  pinMode(redLedPin, OUTPUT);         // Rote LED als Ausgang
  pinMode(greenLedPin, OUTPUT);       // Grüne LED als Ausgang
  pinMode(startButtonPin, INPUT_PULLUP); // Start-Button mit Pullup
  pinMode(stopButtonPin, INPUT_PULLUP);  // Stop-Button mit Pullup
  Serial.begin(9600);                // Serielle Kommunikation starten
  lcd.begin(16, 2);                  // LCD initialisieren
  lcd.clear();                       // LCD leeren
}

void loop() {
  // Button-Status lesen
  startButtonState = digitalRead(startButtonPin);
  stopButtonState = digitalRead(stopButtonPin);

  // Start-Button gedrückt
  if (startButtonState == LOW) {
    collectingData = true;
    delay(500); // Entprellen
    Serial.println("START_SCRIPT");
  }

  // Stop-Button gedrückt
  if (stopButtonState == LOW) {
    collectingData = false;
    delay(500); // Entprellen
    Serial.println("STOP_SCRIPT");
  }

  // Dezibelmessung und Verarbeitung
  if (collectingData) {
    for (int i = 0; i < num_Measure; i++) {
      Sound_signal = analogRead(pinSignal);
      sum += Sound_signal;
    }
    level = sum / num_Measure;
    sum = 0; // Summe zurücksetzen

    long decibel = level - 33;

    // Dezibelwerte an den Raspberry Pi senden
    Serial.print("DECIBEL: ");
    Serial.println(decibel);

    // Dezibelwerte auf dem LCD anzeigen
    lcd.setCursor(0, 0);
    lcd.print("Sound Level=     ");
    lcd.setCursor(0, 0);
    lcd.print("Sound Level= ");
    lcd.print(decibel);

    lcd.setCursor(0, 1);
    if (decibel < soundThreshold) {
      lcd.print("Intensity= Low  ");
      digitalWrite(redLedPin, LOW);  // Rote LED aus
      digitalWrite(greenLedPin, HIGH); // Grüne LED an
    } else {
      lcd.print("Intensity= High ");
      digitalWrite(redLedPin, HIGH); // Rote LED an
      digitalWrite(greenLedPin, LOW);  // Grüne LED aus
    }
  } else {
    // LEDs ausschalten, wenn keine Datensammlung
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, LOW);
  }

  delay(1000); // Warte 1 Sekunde vor der nächsten Messung
}
