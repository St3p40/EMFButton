# EMFButton

![License](https://img.shields.io/badge/License-MIT-blue)
![Tests](https://img.shields.io/github/languages/top/80Stepko08/EMFButton)
![image](https://user-images.githubusercontent.com/26786944/194292846-aae91c5a-4516-4b27-a5f5-cba0ad0a48a4.png)
![Tests](https://img.shields.io/github/last-commit/80Stepko08/EMFButton)
## Comands
```cpp
EMFButton btn (pin, pinmode, normstate); //button initialization

void tick();                // obligatory function(reading button state)

bool isPressed();            //returns true if button is pressed
bool isClicked();            //returns true if button is clicked
bool isReleased();           //returns true if button is released
bool isHeld();               //returns true if button is pressed in timeout
bool isHold();               //returns true if button is pressed after timeout

bool hasSingle();            //returns true if button is clicked once
bool hasDouble();            //returns true if button is clicked twice
bool hasTriple();            //returns true if button is clicked three times

uint8_t hasClicks();         //returns clicks after stopping clicking
uint8_t hasClicksNow();      //returns clicks
uint8_t hasClicksWithHeld(); //returns clicks before pressing
uint8_t hasClicksWithHold(); //returns clicks before holding

uint16_t holdInMillis();     //returns holding time after released
uint16_t holdingTime();      //returns pressing time
```
## Redefinition
```cpp
// use this if you don't use Arduino framework or if you use alternative of these functions 
#define EMFB_SETUP_FUNC(pin, pinmode) pinMode(pin, (pinmode) ? INPUT : INPUT_PULLUP);
#define EMFB_READ_FUNC(pin, pinmode) digitalRead(pin) == (pinmode) ? HIGH : LOW
#define EMFB_MILLIS_FUNC millis()
// timers
#define EMFB_DEB_TIMER 50
#define EMFB_HOLD_TIMER 500
#define EMFB_RELEASE_TIMER 500
```
## Using example
```cpp

#include "EMFButton.h"

EMFButton btn (3, 0);

void setup() {
  Serial.begin(9600);
}

void loop() {
  btn.tick();

  if (btn.isPressed())
    digitalWrite(13, 1);
  else
    digitalWrite(13, 0);

  if (btn.isClicked())
    Serial.print("Clicked ");
  
  if (btn.isReleased())
    Serial.print("Released ");
  
  if (btn.isHeld())
    Serial.print("Hold " + String(btn.hasClicksWithHeld()) + "\n");
  
  if (btn.hasClicks())
    Serial.print("Clicks:" + String(btn.hasClicks()) + "\n");
}

```
