//       #########   ###   ###   #########   #####
//      #########   #### ####   #########   #######
//     ##          ## #### #   ##          ##   ##        #    #
//    #########   ##  ##  #   ######      ######  #   # ###  ###    ###   ###
//   ##          ##      #   ##          ##    # #   #  #    #    #   #  #   #
//  ##          ##      #   ##          ##    # #   #  #    #    #   #  #   #
// ######### # ##      # # ##        # ######   ###    ##   ##   ###   #   #
// MADE BY ST3P40
// Special thanks to Veto1d for adding action callbacks
#ifndef EMFB_BUTTON_H
#define EMFB_BUTTON_H
//==============================
#if !((defined EMFB_SETUP_FUNC) || (defined EMFB_READ_FUNC) || (defined EMFB_MILLIS_FUNC))
#include "Arduino.h"
#endif
//=============================
// use this if you don't use Arduino framework or if you use alternative of these functions
#ifndef EMFB_SETUP_FUNC
#define EMFB_SETUP_FUNC(pin, pinmode) pinMode(pin, (pinmode) ? INPUT : INPUT_PULLUP)
#endif
#ifndef EMFB_READ_FUNC
#define EMFB_READ_FUNC(pin, pinmode) digitalRead(pin) ^ pinmode //== (pinmode) ? HIGH : LOW
#endif
#ifndef EMFB_MILLIS_FUNC
#define EMFB_MILLIS_FUNC millis()
#endif
// timers
#ifndef EMFB_DEB_TIMER
#define EMFB_DEB_TIMER 50
#endif
#ifndef EMFB_HOLD_TIMER
#define EMFB_HOLD_TIMER 500
#endif
#ifndef EMFB_RELEASE_TIMER
#define EMFB_RELEASE_TIMER 500
#endif
//options
//#define EMFB_USE_CALLBACKS
//#define EMFB_WITHOUT_DEBOUNCE
//=============================
#define HIGH_P 0
#define LOW_P 1
#define NORM_OPEN 0
#define NORM_CLOSED 1

#define C_CLICKED 0
#define C_RELEASED 1
#define C_HELD 2
#define C_ENDCLICKS 3
//=============================
#pragma pack(push,1)
typedef struct {
  bool pinmode: 1;
#ifndef EMFB_WITHOUT_DEBOUNCE
  bool lastState: 1;
#endif
  bool pressed: 1;
  bool clicked: 1;
  bool released: 1;
  bool held: 1;
  bool clicksEnd: 1;
} EMFB_flags;
#pragma pack(pop)
//============================
class EMFButton {
  public:

    bool isPressed()            //returns true if button is pressed
    {
      return flag.pressed;
    }
    bool isClicked()            //returns true if button is clicked
    {
      return flag.clicked;
    }
    bool isHeld()               //returns true if button is pressed in timeout
    {
      return flag.held;
    }
    bool isHold()               //returns true if button is pressed after timeout
    {
      return mode == held;
    }
    bool isReleased()           //returns true if button is released
    {
      return flag.released;
    }

    uint8_t hasClicks()         //returns clicks after clicking strike ends
    {
      return (flag.clicksEnd) ? _clicks : 0;
    }
    uint8_t hasClicksNow()      //returns clicks during clicking
    {
      return _clicks - flag.pressed;
    }
    uint8_t hasClicksWithHeld() //returns clicks before holding
    {
      return (flag.held) ? _clicks - 1 : 0;
    }
    uint8_t hasClicksWithHold() //returns clicks during holding
    {
      return isHold() ? _clicks - 1 : 0;
    }

    uint16_t holdingTime()      //returns press time
    {
      return (flag.pressed) ? EMFB_MILLIS_FUNC - _timer : 0;
    }
    uint16_t holdinMillis()     //returns whole press time after releasing
    {
      return (flag.released) ? EMFB_MILLIS_FUNC - _timer : 0;
    }

    bool hasSingle()            //returns true if button is clicked once
    {
      return hasClicks() == 1;
    }
    bool hasDouble()            //returns true if button is clicked twice
    {
      return hasClicks() == 2;
    }
    bool hasTriple()            //returns true if button is clicked three times
    {
      return hasClicks() == 3;
    }

    EMFButton(uint8_t Npin, bool Npinmode = 0, bool Npinclosed = 0)
    {
      _pin = Npin;
      flag.pinmode = !Npinmode ^ Npinclosed; //pinmode says what signal are "pressed button", pinclosed inverse value if it is set
      EMFB_SETUP_FUNC(_pin, Npinmode);
    }
#ifdef EMFB_USE_CALLBACKS
    void attach(uint8_t id, void (*handler)())
    {
      _callback[id] = handler;
    }
    void detach(uint8_t id)
    {
      _callback[id] = NULL;
    }
#endif
    void tick()                 // obligatory function(reading button state)
    {
      uint16_t t = EMFB_MILLIS_FUNC;
      bool reading = EMFB_READ_FUNC(_pin, flag.pinmode);

      flag.held = 0;
      if (flag.clicked) {
        flag.clicked = 0;
        _timer = t;
      }
      if (flag.released) {
        flag.released = 0;
        _timer = t;
      }
      if (flag.clicksEnd) {
        flag.clicksEnd = 0;
        _clicks = 0;
      };

#ifdef EMFB_WITHOUT_DEBOUNCE
      flag.pressed = reading;
#else
      if (reading != flag.lastState)
      {
        _lastChange = t;
      }
      if (t - _lastChange > EMFB_DEB_TIMER)
      {
        flag.pressed = reading;
#endif
      if (flag.pressed)
      {
        if (mode != held)
        {
          if (mode == pressed) {
            if (t - _timer >= EMFB_HOLD_TIMER)
            {
              mode = held;
              flag.held = 1;
            }
          }
          else
          {
            mode = pressed;
            _clicks++;
            flag.clicked = 1;
          }
        }
      }
      else
      {
        if (mode == released)
        {
          if (t - _timer >= EMFB_RELEASE_TIMER)
          {
            mode = await;
            flag.clicksEnd = 1;
          }
        }
        else if (mode == held)
        {
          mode = await;
          _clicks = 0;
          flag.released = 1;
        }
        else if (_clicks == 15)
        {
          mode = await;
          flag.clicksEnd = 1;
          flag.released = 1;
        }
        else if (mode)
        {
          mode = released;
          flag.released = 1;
        }
      }
#ifndef EMFB_WITHOUT_DEBOUNCE
    }
    flag.lastState = reading;
#endif
#ifdef EMFB_USE_CALLBACKS
    if (flag.clicked) call(C_CLICKED);
    if (flag.held) call(C_HELD);
    if (flag.released) call(C_RELEASED);
    if (flag.clicksEnd) call(C_ENDCLICKS);
#endif
};

private:
EMFB_flags flag;
uint8_t _pin;

uint8_t _clicks: 4;

enum button_state {await, pressed, held, released} mode: 2;

  uint16_t _timer;
#ifndef EMFB_WITHOUT_DEBOUNCE
  uint16_t _lastChange;
#endif
#ifdef EMFB_USE_CALLBACKS
  void (*_callback[4])() = {};

void call(uint8_t id)
{
  if (*_callback[id]) _callback[id]();
}
#endif
};
#endif