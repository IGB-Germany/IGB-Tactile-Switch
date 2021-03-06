//tactile switch class
#include "TactileSwitch.h"

//Constructor
TactileSwitch::TactileSwitch(uint8_t pin, uint8_t number, uint8_t mode, bool enablePullUp):
  _number(number),
  _pin(pin),
  _mode(mode),
  _enablePullUp(enablePullUp),
  _event (NO_EVENT),
  _message (NO_MESSAGE),
  _state (START),
  _previousState(START),
  _clicks(0),
  _timePressed(0),
  _timeReleased(0),
  _multiClickFeature(true),
  _readTime(0),
  _durationDebouncePressed(DURATION_DEBOUNCE_PRESSED),
  _durationDebounceReleased(DURATION_DEBOUNCE_RELEASED)
{
  //Configuration of switch
  if (_mode) //HIGH
  {
    //Signal from HIGH(released) to LOW(pressed)
    pinMode(_pin, INPUT);
    digitalWrite(_pin, LOW);
  }
  else //LOW
  {
    //Signal from LOW(released) to HIGH(pressed)
    //to do optional INPUT_PULLUP
    pinMode(_pin, INPUT_PULLUP);
    //digitalWrite(_pin, HIGH);
  }
}

//get number of switch
uint8_t TactileSwitch::getNumber() const
{
  return _number;
}

//set number of switch
void TactileSwitch::setNumber(uint8_t number)
{
  _number = number;
}

//get message of switch like NO_MESSAGE, POLLING_LONG, BOUNCING_PRESSED, BOUNCING_RELEASED
TactileSwitch::event_t TactileSwitch::getEvent() const
{
  return _event;
}

TactileSwitch::message_t TactileSwitch::getMessage() const
{
  return _message;
}

//get current state
uint8_t TactileSwitch::getState(void)
{
  //read and print state
  if (_state == START || _previousState != _state)
  {
    Serial.print(F("_state: "));
    Serial.println(_state);
  }
  _previousState = _state; //save state

  return _state;
}

//get number of clicks
int8_t TactileSwitch::getClicks(void) const
{
  return _clicks;
}

//read switch
void TactileSwitch::readSwitch(void)
{
  //get current state
  getState();

  //get time between polling switch
  if (millis() - _readTime >= DURATION_POLLING) _message = POLLING_SLOW;
  else _message = NO_MESSAGE;
  //save last read time
  _readTime = millis();

  //initalize
  //The button was clicked 102 times consecutively in quick succession.
  if (_state == START)//0
  {
    _clicks = 0;
    _event = NO_EVENT;
    _message = NO_MESSAGE;
    _state = AWAIT_PRESS;
  }

  //1
  else if (_state == AWAIT_PRESS)
  {
    _clicks = 0;
    _event = NO_EVENT;
    _message = NO_MESSAGE;

    //read pin
    //if switch is pressed
    //save _timePressed
    //and change to _state to DEBOUNCE_PRESS
    if (digitalRead(_pin) == _mode) //falling flank HIGH to LOW
    {
      //save time
      _timePressed = millis();
      //change state
      _state = DEBOUNCE_PRESS;
    }
    //else switch is not pressed
    else
    {
      //wait
    }
  }

  //2
  else if (_state == DEBOUNCE_PRESS)
  {
    //read pin
    //if switch is still pressed down
    //wait
    if (digitalRead(_pin) == _mode)
    {
      //and wait until DURATION_DEBOUNCE_PRESS has elapsed
      if ((millis() - _timePressed) > DURATION_DEBOUNCE_PRESSED)
      {
        _state = AWAIT_RELEASE;
        _message = NO_MESSAGE;
      }
    }
    //if switch is not pressed long enough -> go back
    else
    {
      _state = AWAIT_PRESS;
      _message = BOUNCING_PRESSED;
    }
  }

  //3
  else if (_state == AWAIT_RELEASE)
  {
    //initalize if back from Bouncing
    _message = NO_MESSAGE;

    //read _pin
    //if released
    //save _timeReleased
    //and change to _state = DEBOUNCE_RELEASE
    if (digitalRead(_pin) != _mode)
    {
      //save state change time
      _timeReleased = millis();

      _state = DEBOUNCE_RELEASE;
      //only one loop count and than next state
      //does not work if bouncing
      //clicks++;
    }

    //if still pressed and time elapsed -> LONG PRESS
    else
    {
      if ((millis() - _timePressed) > DURATION_LONG_PRESS)
      {
        _event = HOLD_LONG;
        _state = LONG_PRESS;
        //save clicks before waiting
        _clicks = -1;
      }
      else //<= DURATION_LONG_PRESS
      {
        //wait
      }
    }
  }

  //4
  else if (_state == DEBOUNCE_RELEASE)
  {
    //read pin
    //if released long enough
    //change to _state = AWAIT_MULTI_PRESS
    if (digitalRead(_pin) != _mode) //still released ?
    {
      //Serial.println(F("released"));
      //wait long enough
      if ((millis() - _timeReleased) > DURATION_DEBOUNCE_RELEASED)
      {
        //increase clicks
        _clicks ++;
        _state = AWAIT_MULTI_PRESS;
      }
      else
      {
        //wait
      }
    }
    //not released long enough go back
    else
    {
      _state = AWAIT_RELEASE;
      _message = BOUNCING_RELEASED;
    }
  }

  //5
  else if (_state == AWAIT_MULTI_PRESS)
  {
    //time not elapsed ?
    //_multiClickFeature enabled ?
    //read pin
    //if pressed Multiclick
    if ((millis() - _timeReleased) < DURATION_MULTI_CLICK && _multiClickFeature)
    {
      //Next click - Multiclick start
      if (digitalRead(_pin) == _mode) //LOW
      {
        //save state change time
        _timePressed = millis();
        //change _state
        _state = DEBOUNCE_PRESS;
        //increades clicks
        //_clicks += 1; //does not work correct here if bouncing
      }
      //wait
      else
      {
      }

    }
    //End of _multiClickTime
    else
    {
      //end of state machine loop
      //back to start
      _state = AWAIT_PRESS;

      if (_clicks == 1)_event = CLICK;
      else if (_clicks > 1 ) _event = MULTI_CLICK;
      else _event = NO_EVENT;
    }
  }

  //6
  else if (_state == LONG_PRESS)
  {
    //read _pin
    if (digitalRead(_pin) != _mode) //if released
    {
      //save time since released
      _timeReleased = millis();
      //go to debounce
      _state = DEBOUNCE_RELEASE_LONG;
    }
    else //still pressed
    {
      //wait long enough
      if ((millis() - _timePressed) > DURATION_VERY_LONG_PRESS)
      {
        //go to next state
        _state = VERY_LONG_PRESS;
        _event = HOLD_VERY_LONG;
      }
      //stay in this state
      else
      {
        //inversed logic; reset clicks here
        _event = NO_EVENT;
        _message = NO_MESSAGE;
        _clicks = 0;
      }
    }
  }

  //7
  else if (_state == DEBOUNCE_RELEASE_LONG)
  {
    //open: measure first _pin or wait _debounceReleaseTime + _eventTime
    //Solution: best way first check if switch is still pressed than start timer.
    //if switch not released go back instantly
    //means switch ha to be released for an amount of time or we jump back to previous state

    //still released ?
    if (digitalRead(_pin) != _mode)
    {
      //wait long enough
      if ((millis() - _timeReleased) > DURATION_DEBOUNCE_RELEASED)
      {
        _event = RELEASED_HOLD_LONG;
        //go to next state
        _state = AWAIT_PRESS;
      }
    }
    //not released go back
    else
    {
      _state = LONG_PRESS;
      _message = BOUNCING_RELEASED;
    }
  }

  //8
  else if (_state == VERY_LONG_PRESS)
  {
    //read _pin
    if (digitalRead(_pin) != _mode) //if released
    {
      //save time since released
      _timeReleased = millis();
      //go to debounce
      _state = DEBOUNCE_RELEASE_VERY_LONG;
    }
    else //still pressed
    {
      //turned logic; reset clicks here
      _event = NO_EVENT;
      _clicks = 0;
    }
  }

  //9
  else if (_state == DEBOUNCE_RELEASE_VERY_LONG)
  {
    //still released ?
    if (digitalRead(_pin) != _mode)
    {
      //wait long enough
      if ((millis() - _timeReleased) > DURATION_DEBOUNCE_RELEASED)
      {
        _event = RELEASED_HOLD_VERY_LONG;
        //go to next state
        _state = AWAIT_PRESS;
      }
    }
    //not released go back
    else
    {
      _state = VERY_LONG_PRESS;
      _message = BOUNCING_RELEASED;
    }
  }

  //Unknown _state
  else
  {
    _state = START;
    _event = NO_EVENT;
    _clicks = 0;
    _message = NO_MESSAGE;
  }
}

//enable MULTI_CLICK feature default true
void TactileSwitch::setMultiClickFeature(bool enable)
{
  _multiClickFeature = enable;
}

//sets the debounce time (in milliseconds) if pressed [default: DURATION_DEBOUNCE_PRESSED = 20]
void   TactileSwitch::setDurationDebouncePressed(uint8_t durationDebouncePressed)
{
  _durationDebouncePressed = durationDebouncePressed;
}

//sets the debounce time (in milliseconds) if released [default: DURATION_DEBOUNCE_RELEASED = 50].
void TactileSwitch::setDurationDebounceReleased(uint8_t durationDebounceReleased)
{
  _durationDebounceReleased = durationDebounceReleased;
}
