/*
 * mdButton.h
 * 
 * A push button Arduino library capable of returning the number of
 * consecutive button presses made in quick succession or if the 
 * button was held down for a long time.
 * 
 * Version 0.2, 2018-05-21.
 * 
 * by Michel Deslierres 
 *
 * See the accompanying file license.txt for the
 * conditions of use.
 * 
 */

#ifndef mdButton_h
#define mdButton_h

#include "Arduino.h"

#define DEFAULT_DEBOUNCE_PRESS_TIME      15
#define DEFAULT_DEBOUNCE_RELEASE_TIME    30
#define DEFAULT_MULTI_CLICK_TIME        400
#define DEFAULT_HOLD_TIME              2000

enum buttonState_t { AWAIT_PRESS, DEBOUNCE_PRESS, AWAIT_RELEASE, DEBOUNCE_RELEASE, AWAIT_MULTI_PRESS };

class mdButton
{
  public:
    mdButton(uint8_t pin); //constructor
  
    // Set attributes
    void setDebouncePressTime(uint16_t value);
    void setDebounceReleaseTime(uint16_t value);
    void setMultiClickTime(uint16_t value);
    void setHoldTime(uint16_t value);
 
    // status, number of clicks since last update
    // -1 = button held, 0 = button up, 1, 2, ... number of times button clicked
    int status();
  
  private:
    uint8_t pin_;                       
    uint16_t debouncePressTime_   = DEFAULT_DEBOUNCE_PRESS_TIME; 
    uint16_t debounceReleaseTime_ = DEFAULT_DEBOUNCE_RELEASE_TIME;
    uint16_t multiClickTime_      = DEFAULT_MULTI_CLICK_TIME;
    uint16_t holdTime_            = DEFAULT_HOLD_TIME;
  
    // State variables 
    long eventTime_; 
    buttonState_t buttonState_;
    int clicks_;
};

#endif
