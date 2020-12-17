//tactile switch class
#ifndef TACTILE_SWITCH_H
#define TACTILE_SWITCH_H

//https://github.com/kristianklein/PushButton
//https://github.com/nathanRamaNoodles/SensorToButton

//New: power on self test for _mode HIGH or LOW - open
//New: normal open switch. Whats about normal closed switch  - open

//Changed: check code witt cppcheck - done
//Changed: state diagram - open
//Changed: PAP - open

//myButton.setDoubleClickTime(milliseconds) sets the maximum amount of time (in milliseconds) between two clicks for a double-click event to be triggered [default: 300].
//myButton.setHoldTime(milliseconds) sets the amount time (in milliseconds) a button must be held down to trigger a hold event [default: 2000].

//New: setMultiClickFeature()bool enable, default true. Consecutive button clicks will only trigger a regular click event - done
//New: _mode = HIGH or LOW for a HIGH side or low side switch - done
//New: messages like BOUNCING, POLLING_SLOW - done
//New: create _event RELEASED_LONG_HOLD upon release, long hold event always triggers a hold event first - done

//New: speed of polling readTime, duration, previous needs 3 more uint32_t - open

//open: calibrate and test switch

//32 Bytes RAM, 162 bytes ROM per Object


#include "arduino.h" //HIGH, LOW, boolean, uint8_t

class TactileSwitch
{
  public:
    //constructor with pin, unique number, internal resistor mode, driver side
    TactileSwitch(uint8_t pin, uint8_t number, uint8_t mode = HIGH, bool enablePullUp = false);

    //specified events
    enum event_t {NO_EVENT, CLICK, MULTI_CLICK, HOLD_LONG, RELEASED_HOLD_LONG, HOLD_VERY_LONG, RELEASED_HOLD_VERY_LONG};

    //specified messages
    enum message_t {NO_MESSAGE, POLLING_SLOW, BOUNCING_PRESSED, BOUNCING_RELEASED};

    //read pin
    void readSwitch(void);

    //get number of clicks
    int8_t getClicks (void) const;

    //get number of switch
    uint8_t getNumber() const;

    //set number of switch
    void setNumber(uint8_t number);

    //get state of FSM
    uint8_t getState();

    //get event of switch like NO_EVENT, CLICK, MULTI_CLICK, HOLD_LONG, HOLD_VERY_LONG
    event_t getEvent() const;

    //get message of switch like NO_MESSAGE, POLLING_LONG, BOUNCING_PRESSED, BOUNCING_RELEASED
    message_t getMessage() const;

    //enable MULTI_CLICK feature default true
    void setMultiClickFeature(bool enable);

    //sets the debounce time (in milliseconds) if pressed [default: DURATION_DEBOUNCE_PRESSED = 20]
    void setDurationDebouncePressed(uint8_t durationDebouncePressed);

    //sets the debounce time (in milliseconds) if released [default: DURATION_DEBOUNCE_RELEASED = 50]
    void setDurationDebounceReleased(uint8_t durationDebounceReleased);

  private:

    //states
    enum state_t
    {
      START,
      AWAIT_PRESS,
      DEBOUNCE_PRESS,
      AWAIT_RELEASE,
      DEBOUNCE_RELEASE,
      AWAIT_MULTI_PRESS,
      LONG_PRESS,
      DEBOUNCE_RELEASE_LONG,
      VERY_LONG_PRESS,
      DEBOUNCE_RELEASE_VERY_LONG,
    };

    //timing
    enum timing_t
    {
      DURATION_POLLING            = 10,//ms minimum time to read the switch
      DURATION_DEBOUNCE_PRESSED   = 20,//ms debounce period to prevent flickering when pressing the switch
      DURATION_DEBOUNCE_RELEASED  = 50,//ms debounce period to prevent flickering when releasing the switch

      DURATION_MULTI_CLICK        = 200,//ms between clicks for a multi click event
      DURATION_LONG_PRESS         = 2000,//ms hold period: how long to wait for pressed+hold event
      DURATION_VERY_LONG_PRESS    = 4000,//ms long hold period: how long to wait for pressed+hold event
      //DURATION_ERROR              = 10000,//ms how long to wait for error
    };

    uint8_t 	_number;	      //number if there are more switches than one
    uint8_t 	_pin;         	//pin the switch is connected to
    uint8_t 	_mode;        	//HIGH side or LOW side button
    bool 		  _enablePullUp;  //enable internal 20k resistor

    event_t     _event;       //switch event like SWITCH_CLICK, SWITCH_MULTI_CLICK
    message_t   _message;     //error message like BOUNCING_PRESSED, BOUNCING_RELEASED

    state_t   _state;         //variable for statemachine
    state_t   _previousState; //to remember previous _state

    int       _clicks;        //number of clicks; -1 if pressed long, 0 = released, 1, 2, ... number of times switch clicked

    uint32_t 	_timePressed;	  //time the switch was pressed
    uint32_t 	_timeReleased;  //time the switch was released

    bool      _multiClickFeature; //disable MULTI_CLICK

    uint32_t  _readTime;//polling should be faster than 10ms

    uint8_t   _durationDebouncePressed;
    uint8_t   _durationDebounceReleased;
};

#endif //TACTILE_SWITCH_H
