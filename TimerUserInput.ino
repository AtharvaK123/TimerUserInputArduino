#include <Wire.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
//
#include "SevSeg.h"
SevSeg sevseg;

#define totalCount 0
int counter = totalCount;

int buzzer = 8;

#define delayTime 1000
//
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const byte ROWS = 4;
const byte COLS = 4;

int col = 0;
int row = 0;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int buzzerPin = 10;
int number = 0;

bool beginning = true;
bool valueEntered = false;
bool startTimerSetup = false;
bool startTimer = false;

void setup() {
  //
  
  //

}

void loop() {
  //

  if(beginning == true){
    analogWrite(6, 75);
    lcd.begin(16,2);

    pinMode(buzzerPin, OUTPUT);

    Serial.begin(9600);

    lcd.setCursor(col, row);
    lcd.print("How much time?\n");
    row++;
    beginning = false;
    valueEntered = true;
  }


  char key = keypad.getKey();

  if (key && valueEntered == true) {
    if(key == '#'){
      valueEntered = false;
      startTimerSetup = true;
    }
    else if(key != '*' && key != 'A' && key != 'B' && key != 'C' && key != 'D' && number < 4) {
      Serial.println(key);

      lcd.setCursor(col, row);
      lcd.print(key);

      col += 1;

      if(col % 16 == 0){
        row++;
        col = 0;
      }
      
      counter *= 10;
      counter = counter + (key - '0');

      number ++;
      tone(buzzerPin, 1000, 100);
    }
  }
  //

  if(startTimerSetup == true){
    pinMode(buzzer, OUTPUT);

    byte numDigits = 4;
    
    byte digitPins[] = {50, 51, 52, 53};
    byte segmentPins[] = {42, 43, 44, 45, 46, 47, 48, 49};
    
    bool resistorsOnSegments = false;
    
    byte hardwareConfig = COMMON_CATHODE;
    
    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
    
    sevseg.setBrightness(30);
    
    sevseg.blank();
    startTimerSetup = false;
    startTimer = true;
  }

  if(startTimer == true){
    static unsigned long timer = millis();
    static unsigned long buzzerTimer = 0;
    static bool buzzerOn = false;
    static bool countOn = true;

    if (millis() - timer >= delayTime) {
      if(counter % 2 == 0) {
        sevseg.setNumber(counter, 0);
      } else {
        sevseg.setNumber(counter, -1);
      }

      digitalWrite(buzzer, HIGH);
      buzzerTimer = millis();
      buzzerOn = true;

      if(counter > 0) {
        counter--;
      } else {
        startTimer = false;
        beginning = true;
      }

      timer = millis();
    }

    if (buzzerOn && millis() - buzzerTimer >= 800) {
      digitalWrite(buzzer, LOW);
      buzzerOn = false;
    }

    sevseg.refreshDisplay();
  }
}
