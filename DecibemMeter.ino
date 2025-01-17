#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 10, 11, 12, 13);

int num_Measure = 128; // Set the number of measurements   
int pinSignal = A0; // Pin connected to the sound sensor   
int redLed = 5; 
int greenLed = 6; 
int additionalRedLed = 7; // New Red LED for dB over 100
int buzzer = 8; // Buzzer pin
long Sound_signal; // Store the value read from the sound sensor   
long sum = 0; // Store the total value of n measurements   
long level = 0; // Store the average value   
int soundlow = 40;
int soundmedium = 500;

void setup() {
  pinMode(pinSignal, INPUT); // Set the signal pin as input   
  pinMode(redLed, OUTPUT); // Existing red LED
  pinMode(greenLed, OUTPUT); // New green LED
  pinMode(additionalRedLed, OUTPUT); // New red LED
  pinMode(buzzer, OUTPUT); // Buzzer pin
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  // Perform 128 signal readings   
  for (int i = 0; i < num_Measure; i++) {
    Sound_signal = analogRead(pinSignal);
    sum += Sound_signal;
  }

  level = sum / num_Measure; // Calculate the average value   
  Serial.println(level - 33); // Send the decibel value to Raspberry Pi
  lcd.setCursor(0, 0);
  lcd.print("Sound Level= ");
  lcd.print(level - 33);

  // Buzzer logic for dB == 100
  if (level - 33 >= 100) {
    tone(buzzer, 1000);
  }   else {
    noTone(buzzer);
  }


  // Green LED logic for dB <= 100
  if (level - 33 <= 100) {
    digitalWrite(greenLed, HIGH);
    digitalWrite(additionalRedLed, LOW);
  } else {
    digitalWrite(greenLed, LOW);
    digitalWrite(additionalRedLed, HIGH);
  }

  // Existing LED logic
  if (level - 33 < soundlow) {
    lcd.setCursor(0, 1);
    lcd.print("Intensity= Low");
    digitalWrite(redLed, LOW);
  } else if (level - 33 > soundlow && level - 33 < soundmedium) {
    lcd.setCursor(0, 1);
    lcd.print("Intensity=Medium");
    digitalWrite(redLed, LOW);
  } else if (level - 33 > soundmedium) {
    lcd.setCursor(0, 1);
    lcd.print("Intensity= High");
    digitalWrite(redLed, HIGH);
  }

  sum = 0; // Reset the sum of the measurement values  
  delay(1000); // Send data every second
  lcd.clear();
}
