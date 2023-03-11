#include "EMFButton.h"

EMFButton::EMFButton(uint8_t Npin, bool Npinmode, bool Npinclosed) {
  _pin = Npin;
  flag.pinmode = Npinmode;
  flag.pinclosed = Npinclosed;
  pinMode(_pin, (flag.pinmode) ? INPUT : INPUT_PULLUP);
}

void EMFButton::tick() {
  flag.clicked = 0;
  flag.held = 0;
  flag.hold = 0;
  flag.released = 0;
  _clicksEnd = 0;
  uint16_t t = millis();
  bool reading = digitalRead(_pin) == (flag.pinmode) ? HIGH : LOW;
  if (reading != flag.lastState) {
    _lastChange = t;
  }
  if (t - _lastChange > EMFB_DEB_TIMER) {
    if (reading == (flag.pinclosed) ? flag.pressed : !flag.pressed) {
      flag.pressed = (flag.pinclosed) ? !reading : reading;
    }
    if (flag.pressed) {
      switch (mode) {
		  
        case await:
          mode = pressed;
          _timer = t;
          _clicks = 1;
          flag.clicked = 1;
          break;
		  
        case pressed:
          if (t - _timer >= EMFB_HOLD_TIMER) {
            mode = held;
            _timer = t;
            flag.held = 1;
            _clicksWithHeld = _clicks - 1;
          }
          break;
		  
        case held:
          flag.hold = 1;
          _clicksWithHold = _clicks - 1;
          break;
		  
        case released:
          mode = pressed;
          _timer = t;
          _clicks ++;
          flag.clicked = 1;
          break;
      }
    } else {
      switch (mode) {
        case pressed:  if (_clicks == 15) {
            mode = await;
            _clicksEnd = _clicks;
            _clicks = 0;
          } else mode = released; _timer = t; flag.released = 1; break;
		  
        case held: mode = await; _timer = t; _clicks = 0; flag.released = 1; break;
		
        case released: if (t - _timer >= EMFB_RELEASE_TIMER) {
            mode = await;
            _timer = t;
            _clicksEnd = _clicks;
            _clicks = 0;
          } break;
      }
    }
  }
  flag.lastState = reading;
}
