#ifndef EMFB_BUTTON_H
#define EMFB_BUTTON_H
#include "Arduino.h"

//=============================
#ifndef EMFB_SETUP_FUNC
#define EMFB_SETUP_FUNC(pin, pinmode) pinMode(pin, (pinmode) ? INPUT : INPUT_PULLUP)
#endif
#ifndef EMFB_READ_FUNC
#define EMFB_READ_FUNC(pin, pinmode) digitalRead(pin) == (pinmode) ? HIGH : LOW
#endif
#ifndef EMFB_MILLIS_FUNC
#define EMFB_MILLIS_FUNC millis()
#endif

#ifndef EMFB_DEB_TIMER
#define EMFB_DEB_TIMER 50
#endif
#ifndef EMFB_HOLD_TIMER
#define EMFB_HOLD_TIMER 500
#endif
#ifndef EMFB_RELEASE_TIMER
#define EMFB_RELEASE_TIMER 500
#endif
//=============================
#define HIGH_P 0
#define LOW_P 1
#define NORM_OPEN 0
#define NORM_CLOSED 1
//=============================
#pragma pack(push,1)
typedef struct {
  bool pinmode: 1;
  bool pinclosed: 1;
  bool lastState: 1;
  bool pressed: 1;
  bool clicked: 1;
  bool released: 1;
  bool held: 1;
  bool hold: 1;
} EMFB_flags;
#pragma pack(pop)
//============================
class EMFButton {
  public:

    bool isPressed() {
      return flag.pressed;
    }
    bool isClicked() {
      return flag.clicked;
    }
    bool isHeld() {
      return flag.held;
    }
    bool isHold() {
      return flag.hold;
    }
    bool isReleased() {
      return flag.released;
    }

    uint8_t hasClicks() {
      return _clicksEnd;
    }
    uint8_t hasClicksNow() {
      if (flag.pressed)
        return _clicks - 1;
      else
        return _clicks;
    }
    uint8_t hasClicksWithHeld() {
      if (flag.held)
        return _clicks - 1;
      else
        return 0;
    }
    uint8_t hasClicksWithHold() {
      if (flag.hold)
        return _clicks - 1;
      else
        return 0;
    }

    uint16_t holdingTime() {
      if (flag.pressed)
        return EMFB_MILLIS_FUNC - _timer;
      else
        return 0;
    }
    uint16_t holdinMillis() {
      if (flag.released && !mode)
        return _timer;
      else
        return 0;
    }

    bool hasSingle() {
      return _clicksEnd == 1;
    }
    bool hasDouble() {
      return _clicksEnd == 2;
    }
    bool hasTriple() {
      return _clicksEnd == 3;
    }
    
    EMFButton(uint8_t Npin, bool Npinmode = 0, bool Npinclosed = 0) {
      _pin = Npin;
      flag.pinmode = Npinmode;
      flag.pinclosed = Npinclosed;
      pinMode(_pin, (flag.pinmode) ? INPUT : INPUT_PULLUP);
    }

    void tick() {
      flag.clicked = 0;
      flag.held = 0;
      flag.hold = 0;
      flag.released = 0;
      _clicksEnd = 0;
      uint16_t t = EMFB_MILLIS_FUNC;
      bool reading = (!flag.pinclosed)^ EMFB_READ_FUNC(_pin, flag.pinmode);

      if (reading != flag.lastState) {
        _lastChange = t;
      }
      if (t - _lastChange > EMFB_DEB_TIMER) {
        flag.pressed = !reading;

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
                flag.held = 1;
              }
              break;

            case held:
              flag.hold = 1;
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

            case held: mode = await; _timer = t - _timer; _clicks = 0; flag.released = 1; break;

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
    };
  private:
    EMFB_flags flag;
    uint8_t _pin;

    uint8_t _clicks: 4;
    uint8_t _clicksEnd: 4;

    enum button_state {await, pressed, held, released} mode: 2;

      uint16_t _timer;
      uint16_t _lastChange;
  };
#endif
