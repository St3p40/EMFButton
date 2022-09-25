# EMFButton

![License](https://img.shields.io/badge/License-MIT-blue)
![Tests](https://img.shields.io/github/languages/top/80Stepko08/EMFButton)
![Tests](https://img.shields.io/badge/version-0.1.0-blue)
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

uint8_t hasClicks();         //returns clicks 
uint8_t hasClicksWithHeld(); //returns clicks before pressing
uint8_t hasClicksWithHold(); //returns clicks before holding
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
