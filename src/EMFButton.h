//       #########   ###   ###   #########   #####
//      #########   #### ####   #########   #######
//     ##          ## #### #   ##          ##   ##        #    #
//    #########   ##  ##  #   ######      ######  #   # ###  ###    ###   ###
//   ##          ##      #   ##          ##    # #   #  #    #    #   #  #   #
//  ##          ##      #   ##          ##    # #   #  #    #    #   #  #   #
// ######### # ##      # # ##        # ######   ###    ##   ##   ###   #   #
// MADE BY ST3P40
// Special thanks to Veto1d for 1.8 update
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
#define EMFB_READ_FUNC(pin, pinmode) digitalRead(pin) == (pinmode) ? HIGH : LOW
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
#define WAS_C_CLICKED(times) times + 2
#define IS_C_CLICKED(times) times + 17
//=============================
#pragma pack(push,1)
typedef struct {
  bool pinmode: 1;
  bool pinclosed: 1;
#ifndef EMFB_WITHOUT_DEBOUNCE
  bool lastState: 1;
#endif
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

    bool isPressed()            //returns true if button is pressed
    {
      return flag.pressed;
    }
    bool isClicked()            //returns true if button is clicked
    {
      return flag.clicked;
    }
    bool isHeld()               //returns true if button is released
    {
      return flag.held;
    }
    bool isHold()               //returns true if button is pressed in timeout
    {
      return flag.hold;
    }
    bool isReleased()           //returns true if button is pressed after timeout
    {
      return flag.released;
    }

    uint8_t hasClicks()         //returns clicks after stopping clicking
    {
      return _clicksEnd;
    }
    uint8_t hasClicksNow()      //returns clicks
    {
      return _clicks - flag.pressed;
    }
    uint8_t hasClicksWithHeld() //returns clicks before pressing
    {
      return (flag.held) ? _clicks - 1 : 0;
    }
    uint8_t hasClicksWithHold() //returns clicks before holding
    {
      return (flag.hold) ? _clicks - 1 : 0;
    }

    uint16_t holdingTime()      //returns holding time after released
    {
      return (flag.pressed) ? EMFB_MILLIS_FUNC - _timer : 0;
    }
    uint16_t holdinMillis()     //returns pressing time
    {
      return (flag.released && !mode) ? _timer : 0;
    }

    bool hasSingle()            //returns true if button is clicked once
    {
      return _clicksEnd == 1;
    }
    bool hasDouble()            //returns true if button is clicked twice
    {
      return _clicksEnd == 2;
    }
    bool hasTriple()            //returns true if button is clicked three times
    {
      return _clicksEnd == 3;
    }

    EMFButton(uint8_t Npin, bool Npinmode = 0, bool Npinclosed = 0)
    {
      _pin = Npin;
      flag.pinmode = Npinmode;
      flag.pinclosed = Npinclosed;
      EMFB_SETUP_FUNC(_pin, flag.pinmode);
    }
#ifdef EMFB_USE_CALLBACKS
    void attach(uint8_t id, void (*handler)())
    {
      _callback[id] = *handler;
    }
    void detach(uint8_t id)
    {
      _callback[id] = NULL;
    }
#endif
    void tick()                 // obligatory function(reading button state)
    {
      flag.clicked = 0;
      flag.held = 0;
      flag.hold = 0;
      flag.released = 0;
      _clicksEnd = 0;
      uint16_t t = EMFB_MILLIS_FUNC;
      bool reading = (!flag.pinclosed)^ EMFB_READ_FUNC(_pin, flag.pinmode);
#ifdef EMFB_WITHOUT_DEBOUNCE
      flag.pressed = reading;
#else
      if (reading != flag.lastState)
      {
        _lastChange = t;
      }
      if (t - _lastChange > EMFB_DEB_TIMER)
      {
#endif
        flag.pressed = !reading;

        if (flag.pressed)
        {
          switch (mode)
          {
            case await:
              mode = pressed;
              _timer = t;
              _clicks = 1;
              flag.clicked = 1;
              call(C_CLICKED);
              call(IS_C_CLICKED(1));
              break;

            case pressed:
              if (t - _timer >= EMFB_HOLD_TIMER)
              {
                mode = held;
                flag.held = 1;
                call(C_HELD);
              }
            break;

          case held:
            flag.hold = 1;
          break;

          case released:
            mode = pressed;
            _timer = t;
            _clicks ++;
            call(IS_C_CLICKED(_clicks));
            flag.clicked = 1;
          break;
         }
      }
      else
      {
        switch (mode)
        {
          case pressed:  if (_clicks == 15)
          {
              mode = await;
              _clicksEnd = _clicks;
              _clicks = 0;
            } else mode = released; _timer = t; flag.released = 1; break;

          case held: mode = await; _timer = t - _timer; _clicks = 0; flag.released = 1; break;

          case released: if (t - _timer >= EMFB_RELEASE_TIMER) {
              mode = await;
              _timer = t;
              _clicksEnd = _clicks;
              call(WAS_C_CLICKED(_clicks));
              _clicks = 0;
            } break;
        }
      }
#ifndef EMFB_WITHOUT_DEBOUNCE
    }
    flag.lastState = reading;
#endif
  };

  private:
    EMFB_flags flag;
    uint8_t _pin;

    uint8_t _clicks: 4;
    uint8_t _clicksEnd: 4;

    enum button_state {await, pressed, held, released} mode: 2;

    uint16_t _timer;
#ifndef EMFB_WITHOUT_DEBOUNCE
    uint16_t _lastChange;
#endif
    void (*_callback[33])() = {};

    void call(uint8_t id)
    {
#ifdef EMFB_USE_CALLBACKS
      if (*_callback[id]) _callback[id]();
#endif
    }
};
#endif
