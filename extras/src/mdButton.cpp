/*
 * mdButton.cpp
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

#include "mdButton.h"

mdButton::mdButton(uint8_t pin)
{
  pin_ = pin;
  pinMode(pin_, INPUT_PULLUP);
  digitalWrite(pin_, HIGH);
  buttonState_ = AWAIT_PRESS;
}

//Set attributes
void mdButton::setDebouncePressTime(uint16_t value){debouncePressTime_ = value;}
void mdButton::setDebounceReleaseTime(uint16_t value){debounceReleaseTime_ = value;}
void mdButton::setMultiClickTime(uint16_t value){multiClickTime_ = value;}
void mdButton::setHoldTime(uint16_t value){holdTime_ = value;}

int mdButton::status(void)
{
  if (buttonState_ == AWAIT_PRESS) {
    clicks_ = 0;
    if (digitalRead(pin_) == LOW) {
      buttonState_ = DEBOUNCE_PRESS;
      eventTime_ = millis();  
    }  
  } 
  
  else if (buttonState_ == DEBOUNCE_PRESS) {
    if ((millis() - eventTime_) > debouncePressTime_) { 
      buttonState_ = AWAIT_RELEASE;
      eventTime_ = millis(); 
    }  
  } 
  
  else if (buttonState_ == AWAIT_RELEASE) {
    if (digitalRead(pin_) == HIGH) {
      if ((millis() - eventTime_) > holdTime_) {
        buttonState_ = AWAIT_PRESS;
        return -1;
      }
      buttonState_ = DEBOUNCE_RELEASE;
      eventTime_ = millis();
    }
  } 

  else if (buttonState_ == DEBOUNCE_RELEASE) {
    if ((millis() - eventTime_) > debounceReleaseTime_) { 
      clicks_ += 1;
      buttonState_ = AWAIT_MULTI_PRESS;
      eventTime_ = millis(); 
    }  
  } 
  
  else {   // (buttonState == AWAIT_MULTI_PRESS) 
    if (digitalRead(pin_) == LOW) {
      buttonState_ = DEBOUNCE_PRESS;
      eventTime_ = millis(); 
    } 
    else if ((millis() - eventTime_) > multiClickTime_) {
      buttonState_ = AWAIT_PRESS;
      return clicks_;
    } 
  }
  return 0; 
}

