#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
unsigned int seconds = 0;
unsigned int minutes = 0;
unsigned int current = 0;
unsigned int previous1 = 0;
unsigned int previous2 = 0;
int counter = 1;
String secStr;
String minStr;
bool finish = false;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  lcd.init();
  lcd.backlight();
  previous1 = millis();
  previous2 = millis();
  lcd.setCursor(0, 0);
  lcd.print("-Chronometer-");
  lcd.setCursor(0, 1);
  lcd.print("1");
  lcd.setCursor(3, 1);
  lcd.print("min.");
}

void loop() {
  current = millis();
  if (digitalRead(2) == 0) {
    delay(250);
    digitalWrite(9, LOW);
    digitalWrite(11, HIGH);
    finish = true;
  }
  if (digitalRead(3) == 0) {
    delay(250);
    counter++;
    lcd.setCursor(0, 1);
    lcd.print("  ");
    lcd.setCursor(0, 1);
    lcd.print(counter);
    if (counter >= 60) counter = 0;
  }
  if (current - previous1 > 1000 && finish == true) {
    previous1 = millis();
    seconds++;
    if (seconds == 60) {
      seconds = 0;
      minutes++;
    }
    if (minutes == 60) {
      minutes = 0;
    }
    secStr = String(seconds);
    minStr = String(minutes);
    if (seconds < 10) secStr = "0" + String(seconds);
    if (minutes < 10) minStr = "0" + String(minutes);
    lcd.setCursor(11, 1);
    lcd.print(minStr + ":" + secStr);
  }
  // Shift one position to the left
  if (current - previous2 > 200 && finish == true) {
    previous2 = millis();
    digitalWrite(8, !digitalRead(8));
  }
  if (minutes == counter) {
    finish = false;
    minutes = 0;
    seconds = 0;
    lcd.setCursor(0, 1);
    lcd.print("  ");
    lcd.setCursor(0, 1);
    lcd.print(counter);
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(11, LOW);
    buzzer();
  }
}

void buzzer() {
  tone(10, 8000, 12500);
  delay(12500);
  noTone(10);
}
