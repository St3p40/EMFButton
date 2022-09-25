#ifndef E_M_F_BUTTON_H
#define E_M_F_BUTTON_H
#include "Arduino.h"

#define EMFB_DEB_TIMER 50
#define EMFB_HOLD_TIMER 500
#define EMFB_RELEASE_TIMER 500

class EMFButton {
  public:
    EMFButton(uint8_t Npin) : _pin(Npin), _pinmode(0), _pinclosed(0) {
      pinMode(_pin, INPUT);
    };
    EMFButton(uint8_t Npin, bool Npinmode) : _pin(Npin), _pinmode(Npinmode), _pinclosed(0) {
      pinMode(_pin, (_pinmode) ? INPUT : INPUT_PULLUP);
    };
    EMFButton(uint8_t Npin, bool Npinmode, bool Npinclosed) : _pin(Npin), _pinmode(Npinmode), _pinclosed(Npinclosed) {
      pinMode(_pin, (_pinmode) ? INPUT : INPUT_PULLUP);
    };

    void tick();
    bool isPressed() {
      return _pressed;
    }

    bool isClicked() {
      return _clicked;
    }

    bool isHeld() {
      return _held;
    }

    bool isHold() {
      return _hold;
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

    bool isReleased() {
      return _released;  
    }
	    bool isHold() {
      return _hold;
    }
	
    uint8_t hasClicksWithHeld() {
      return _clicksWithHeld - 1;
    }
	
    uint8_t hasClicksWithHold() {
      return isHold()? _clicks - 1 : 0;
    }
	
    bool hasOnlyHold(){
      return isHold() & _clicks == 1;
    }
	
    bool hasHoldAndSingle(){
      return isHold() & _clicks == 2;
    }
	
    bool hasHoldAndDouble(){
      return isHold() & _clicks == 3;
    }
	
    bool hasOnlyHeld(){
      return !_clicksWithHeld & _held;
    }
	
    bool hasHeldAndSingle(){
      return _clicksWithHeld == 1;
    }
	
    bool hasHeldAndDouble(){
      return _clicksWithHeld == 2;
    }
    
private:
    bool _pinmode:1;
    bool _pinclosed: 1;
    bool _lastState:1;
    bool _pressed:1;
    bool _clicked:1;
    bool _released: 1;
    bool _held:1;
    bool _hold:1;
    uint8_t _pin;
    uint8_t _clicks:4;
    uint8_t _clicksEnd:4;
    uint8_t _clicksWithHeld:4;
    uint8_t _clicksWithHold:4;
    enum button_state {await, pressed, held, released} mode:2;
    uint32_t _timer;
    uint32_t _lastChange;
};
#endif