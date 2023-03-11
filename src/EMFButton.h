#ifndef EMFB_BUTTON_H
#define EMFB_BUTTON_H
#include "Arduino.h"

#define EMFB_DEB_TIMER 50
#define EMFB_HOLD_TIMER 500
#define EMFB_RELEASE_TIMER 500

#define HIGH_P 0
#define LOW_P 1
#define NORM_OPEN 0
#define NORM_CLOSED 1

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

class EMFButton {
  public:
    EMFButton(uint8_t Npin, bool Npinmode = 0, bool Npinclosed = 0);

    void tick();

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
    uint8_t hasClicksWithHeld() {
      return _clicksWithHeld;
    }
    uint8_t hasClicksWithHold() {
      return _clicksWithHold;
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

  private:
    EMFB_flags flag;
    uint8_t _pin;

    uint8_t _clicks: 4;
    uint8_t _clicksEnd: 4;
    uint8_t _clicksWithHeld: 4;
    uint8_t _clicksWithHold: 4;

    enum button_state {await, pressed, held, released} mode: 2;

    uint16_t _timer;
    uint16_t _lastChange;
 };
#endif
