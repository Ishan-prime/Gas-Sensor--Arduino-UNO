#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int gasSensorPin = A0;  // Analog pin for MQ-2
const int ledPin = 8;         // Warning LED pin
const int buzzerPin = 7;      // Alarm Buzzer pin

// Initialize I2C LCD (Address 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Gas Alarm System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read analog value from gas sensor (0 to 1023)
  int sensorValue = analogRead(gasSensorPin);
  
  // Map raw value to a percentage scale (0% to 100%)
  int gasPercent = map(sensorValue, 0, 1023, 0, 100);

  // Print values to Serial Monitor for debugging
  Serial.print("Gas Raw: ");
  Serial.print(sensorValue);
  Serial.print(" | Level: ");
  Serial.print(gasPercent);
  Serial.println("%");

  // Display data on LCD Screen
  lcd.setCursor(0, 0);
  lcd.print("Gas Level: ");
  lcd.print(gasPercent);
  lcd.print("%   "); // Clear trailing characters

  lcd.setCursor(0, 1);
  
  // Safety Threshold Logic (triggers if gas level exceeds 40%)
  if (gasPercent > 40) {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000); // Send a 1000Hz tone to the buzzer
    lcd.print("STATUS: DANGER! ");
    Serial.println("⚠️ Warning: High gas concentration detected!");
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);    // Turn off the buzzer
    lcd.print("STATUS: NORMAL  ");
  }

  delay(500);
}