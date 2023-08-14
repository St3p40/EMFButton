# EMFButton

![License](https://img.shields.io/badge/License-MIT-blue)
![Tests](https://img.shields.io/github/languages/top/80Stepko08/EMFButton)
[![arduino-library-badge](https://www.ardu-badge.com/badge/EMFButton.svg?)](https://www.ardu-badge.com/EMFButton)
![Tests](https://img.shields.io/github/last-commit/80Stepko08/EMFButton)

Button Press Handling Library
- Works with normal opened and closed buttons with pull-down and pull-up connection
- Has own debounce that can be configured and deactivated
- Pressing, multiclicking (up to 15 clicks), holding, releasing handling
- Works with callbacks
- Changing functions for reading button (for using faster analog, use it without Arduino library or using virtual button)

## Comands
```cpp
// button initialization
EMFButton btn (pin);
EMFButton btn (pin, pinmode);
EMFButton btn (pin, pinmode, normstate);
// reading button state
void tick();                             // obligatory function(reading button state)
// button pressing usage
bool isPressed();                        // returns true if button is pressed
bool isClicked();                        // returns true if button is clicked
bool isReleased();                       // returns true if button is released
bool isHeld();                           // returns true if button is pressed in timeout
bool isHold();                           // returns true if button is pressed after timeout
// basic multiclicking
bool hasSingle();                        // returns true if button is clicked once
bool hasDouble();                        // returns true if button is clicked twice
bool hasTriple();                        // returns true if button is clicked three times
// advanced multiclicking and holding
uint8_t hasClicks();                     // returns clicks after releasing for a long time
uint8_t hasClicksNow();                  // returns clicks
uint8_t hasClicksWithHeld();             // returns clicks before pressing
uint8_t hasClicksWithHold();             // returns clicks before holding
// advanced holding
uint16_t holdInMillis();                 // returns holding time after releasing (ms)
uint16_t holdingTime();                  // returns pressing time (ms)
//callbacks
void attach(action, function);           // attaching function for this action
void detach(action);                     // detaching function for this action
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
// aditional options
#define EMFB_USE_CALLBACKS
#define EMFB_WITHOUT_DEBOUNCE
```
