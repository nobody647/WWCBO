/*
  _       ____  _________   __   _       ________    __       ________    ___   __________    ____  ______   ____ _    ____________
  | |     / / / / / ____/ | / /  | |     / /  _/ /   / /      / ____/ /   /   | / ___/ ___/   / __ )/ ____/  / __ \ |  / / ____/ __ \
  | | /| / / /_/ / __/ /  |/ /   | | /| / // // /   / /      / /   / /   / /| | \__ \\__ \   / __  / __/    / / / / | / / __/ / /_/ /
  | |/ |/ / __  / /___/ /|  /    | |/ |/ // // /___/ /___   / /___/ /___/ ___ |___/ /__/ /  / /_/ / /___   / /_/ /| |/ / /___/ _, _/
  |__/|__/_/ /_/_____/_/ |_/     |__/|__/___/_____/_____/   \____/_____/_/  |_/____/____/  /_____/_____/   \____/ |___/_____/_/ |_|


  A project by Ian Flanagan

  Ideas from Trevor, Shane, Joe, and some middle schoolers

*/






#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#include "LowPower.h"
#include <avr/pgmspace.h>
#include "Classes.h"

int BL = 9;
int WK = 2;
int RS = 7;
int E = 6;
int D4 = 13;
int D5 = 12;
int D6 = 11;
int D7 = 10;

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);
RTC_DS1307 rtc;
char buffer[16];
DateTime now;

volatile bool screen  = true;
int loops = 0;
volatile int sleepSecs = 8;
volatile int test = 0; //Used for debouncing button
bool prevLightStatus = true;

void setup() {
  pinMode(BL, INPUT_PULLUP);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Error: ");
  lcd.setCursor(0, 1);

  if (!rtc.begin()) {
    lcd.print("No RTC");
    delay(5000);
    lcd.clear();
  }

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    lcd.print("Time unset");
    delay(5000);
    lcd.clear();
  }
  lcd.print("ya done broke it");
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.adjust(DateTime(2017, 6, 8, 11, 22, 0));
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), wake, RISING);
}

void loop() {
  if (loops <= 2) {
    //noInterrupts();
    screen = true;
    sleepSecs = 1;
  } else {
    interrupts();
  }
  now = rtc.now();
  if (screen) {
    pinMode(BL, INPUT_PULLUP);
  } else {
    pinMode(BL, INPUT);
  }


  lcd.begin(16, 2);
  lcd.clear();
  displayEvent(0);
  displayTime(1);

  loops++;

  if (sleepSecs == 1) {
    sleepSecs = 8;
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  } else {
    sleepSecs = 8;
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

String getNextEvent() {
  if (now.dayOfTheWeek() == 5) {
    for (int i; i < 20; i++) {
      int nMins1 = 60 * getHour();
      int nMins = nMins1 + now.minute();
      strcpy_P(buffer, (char*)pgm_read_word(&(fClassTimes[i])));
      String b = String(buffer);
      int eMinsH = b.substring(0, b.indexOf(':')).toInt() * 60;
      String eMinsM1 = b.substring(1);
      int eMinsM = 0;
      if (eMinsM1.startsWith(":")) {
        eMinsM = eMinsM1.substring(1).toInt();
      } else {
        eMinsM = eMinsM1.substring(2).toInt();
      }
      int eMins = eMinsH + eMinsM;
      int t = eMins - nMins;
      if (t > 0) {
        if (t == 1) {
          sleepSecs = 1;
          int sec = now.second();
          if (sec >= 57) {
            screen = prevLightStatus;
          } else {
            screen = !screen;
          }
          return String(60 - sec) + " seconds";
        }
        prevLightStatus = screen;
      }
      strcpy_P(buffer, (char*)pgm_read_word(&(fClassNames[i])));
      return String(buffer) + " in " + t;
    }

  } else if (now.dayOfTheWeek() >= 6) {
    return "No School!";
    screen = false;
  }
  else {
    for (int i; i < 16; i++) {
      int nMins1 = 60 * getHour();
      int nMins = nMins1 + now.minute();
      strcpy_P(buffer, (char*)pgm_read_word(&(classTimes[i])));
      String b = String(buffer);
      int eMinsH = b.substring(0, b.indexOf(':')).toInt() * 60;
      String eMinsM1 = b.substring(1);
      int eMinsM = 0;
      if (eMinsM1.startsWith(":")) {
        eMinsM = eMinsM1.substring(1).toInt();
      } else {
        eMinsM = eMinsM1.substring(2).toInt();
      }
      int eMins = eMinsH + eMinsM;
      int t = eMins - nMins;
      if (t > 0) {
        if (t == 1) {
          sleepSecs = 1;
          int sec = now.second();
          if (sec >= 57) {
            screen = prevLightStatus;
          } else {
            screen = !screen;
          }
          return String(60 - sec) + " seconds";
        }
        prevLightStatus = screen;
        if (now.dayOfTheWeek() == 1 || now.dayOfTheWeek() == 3 || ttClassNames[0] == "") {
          strcpy_P(buffer, (char*)pgm_read_word(&(classNames[i])));
          return String(buffer) + " in " + t;
        } else {
          strcpy_P(buffer, (char*)pgm_read_word(&(ttClassNames[i])));
          return String(buffer) + " in " + t;
        }
      }
    }
  }

  return "School is over!";
  screen = false;
}

int getHour() {
  return now.hour();
}

void wake() {
  int sec = now.second();
  if (sec != test) {
    screen = !screen;
  }
  test = sec;

  //wakeMillis = millis();
}

void displayTime(int row) {
  lcd.setCursor(0, row);
  lcd.print("Time: ");

  int hour = getHour();
  String ampm = "AM";

  if (hour > 12) {
    hour = hour - 12;
    ampm = "PM";
  }
  if (hour == 12) {
    ampm = "PM";
  }
  if (hour == 0) {
    hour = 12;
    ampm = "AM";
  }
  lcd.print(hour);
  lcd.print(":");

  String minute = String(now.minute());
  if (minute.length() == 1) {
    minute = "0" + minute;
  }
  lcd.print(minute);
  lcd.print(" ");
  lcd.print(ampm);

}
void displayEvent(int row) {
  lcd.setCursor(0, row);
  //lcd.print("hello there");
  String event = getNextEvent();
  lcd.print(event);
}

