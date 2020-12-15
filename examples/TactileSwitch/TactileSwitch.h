//tactile switch class
#ifndef TACTILE_SWITCH_H
#define TACTILE_SWITCH_H

//open: calibrate and test switch
//state machine like https://www.sigmdel.ca/michel/program/esp8266/arduino/switch_debouncing_en.html
//speed of polling getDurationRead()
//get status messages like BOUNCING, NO_ERROR, POLLING_SLOW...
//clean up code

//create LONG_HOLD upon release ?
//create power on self test for _mode HIGH or LOW
//warning: control reaches end of non-void function [-Wreturn-type]
//normal open switch. Whats about normal closed switch ?
//draw state diagram
//draw PAP
//long hold event always triggers a hold event first

//myButton.setDebounceTime(milliseconds) sets the debounce time (in milliseconds) [default: 10].
//myButton.setDoubleClickTime(milliseconds) sets the maximum amount of time (in milliseconds) between two clicks for a double-click event to be triggered [default: 300].
//myButton.setHoldTime(milliseconds) sets the amount time (in milliseconds) a button must be held down to trigger a hold event [default: 2000].
//https://www.reddit.com/r/arduino/comments/99pudq/a_library_that_makes_it_easy_to_use_push_buttons/
//New: setMultiClickFeature()bool enable, default true. Consecutive button clicks will only trigger a regular click event.- done
//New: _mode = HIGH or LOW for a HIGH side or low side switch - done

//44 Bytes RAM, 68 bytes ROM per Object


#include "arduino.h" //HIGH, LOW, boolean, uint8_t

class TactileSwitch
{
  public:
    //constructor with pin, unique number, internal resistor mode, driver side
    TactileSwitch(uint8_t pin, uint8_t number, uint8_t mode = HIGH, bool enablePullUp = false);

    //specified events
    enum event_t {NO_EVENT, CLICK, MULTI_CLICK, HOLD_LONG, HOLD_VERY_LONG};
    
    //specified messages
    enum message_t {NO_MESSAGE, POLLING_LONG, BOUNCING_PRESSED, BOUNCING_RELEASED};

    //read pin
    int readSwitch(void);
    
    //get number of clicks
    uint8_t getClicks(void);

    //get number of switch
    uint8_t getNumber();
    
    //set number of switch
    void setNumber(uint8_t number);

    //get state of FSM
    uint8_t getState();

    //get time between reading switch
    uint8_t getDurationRead();

    //get event of switch like NO_EVENT, CLICK, MULTI_CLICK, HOLD_LONG, HOLD_VERY_LONG
    event_t getEvent();

    //get message of switch like NO_MESSAGE, POLLING_LONG, BOUNCING_PRESSED, BOUNCING_RELEASED
    message_t getMessage();
    
    //enable MULTI_CLICK feature default true
    void setMultiClickFeature(bool enable); 

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

    int       _clicks;        //number of clicks; -1 if pressed long, 0 = released, 1, 2, ... number of times button clicked
    int       _previousClicks;//to remember a change in _clicks

    uint8_t   _durationRead; //polling should be faster than 20ms
    uint32_t  _readTime;
    uint32_t  _previousReadTime;

    uint32_t 	_timePressed;	  //time the switch was pressed
    uint32_t 	_timeReleased;  //time the switch was released

    bool      _multiClickFeature; //disable MULTI_CLICK

};

#endif //TACTILE_SWITCH_H
