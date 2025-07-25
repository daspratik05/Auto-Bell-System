#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

// LCD at I2C address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

// Pin configuration
const int UP_BUTTON = 6;
const int DOWN_BUTTON = 5;
const int SET_BUTTON = 4;
const int BUZZER_PIN = 3;

// Bell times (in minutes since midnight)
int periodTimes[] = {630, 690, 750, 810, 840, 960};  // 10:30, 11:30, 12:30, 1:30, 2:00, 4:00
int numPeriods = sizeof(periodTimes) / sizeof(periodTimes[0]);

void setup() {
  Wire.begin();
  rtc.begin();
  lcd.begin(16, 2);
  lcd.backlight();

  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(SET_BUTTON, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Auto Bell Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();
  int currentMinutes = now.hour() * 60 + now.minute();

  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  if (now.hour() < 10) lcd.print("0");
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());

  for (int i = 0; i < numPeriods; i++) {
    if (currentMinutes == periodTimes[i]) {
      ringBell();
      delay(60000);  // Avoid multiple rings in 1 minute
    }
  }

  delay(1000);  // Update every second
}

void ringBell() {
  lcd.setCursor(0, 1);
  lcd.print("** Bell Ringing **");
  tone(BUZZER_PIN, 200);    // Sound the buzzer
  delay(3000);              // Ring for 3 seconds
  noTone(BUZZER_PIN);       // Stop the buzzer
  lcd.setCursor(0, 1);
  lcd.print("                ");  // Clear message
}
