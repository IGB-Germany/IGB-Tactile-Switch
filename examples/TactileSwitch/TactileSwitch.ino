//tactile switch class
#include "TactileSwitch.h"

//Arduino UNO
//ROM: 4100 Bytes (12%)
//RAM: 269 Bytes (13%)

//create a TactileSwitch with objectName(analog pin connected, any number, signal you switch HIGH or LOW)
//HIGH
TactileSwitch mySwitch1(A0, 1, HIGH);

//LOW
TactileSwitch mySwitch2(A2, 2, LOW);

//Sparkfun RGB encoder switch is a high mode version and it needs a pull down resistor
TactileSwitch mySwitch3(A4, 3, HIGH);
//Sparkfun RG encoder switch is a low mode version

void setup()
{
  //put your setup code here, to run once:
  //use as fast enough serial out, otherwise you miss switch events
  Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:

  mySwitch1.readSwitch();
  mySwitch2.readSwitch();
  mySwitch3.readSwitch();

  processEventTactileSwitch(mySwitch1);
  processEventTactileSwitch(mySwitch2);
  processEventTactileSwitch(mySwitch3);

  //delay(10);//to test _message = POLLING_LONG
}

void processEventTactileSwitch(TactileSwitch& mySwitch)
{
  //get event
  switch (mySwitch.getEvent())
  {
    case TactileSwitch::NO_EVENT:
      break;

    case TactileSwitch::CLICK:
      {
        Serial.print(F("Switch "));
        Serial.print(mySwitch.getNumber());
        Serial.println(F(" was clicked once!"));
      }
      break;

    case TactileSwitch::MULTI_CLICK:
      {
        Serial.print(F("Switch "));
        Serial.print(mySwitch.getNumber());
        Serial.print(F(" was clicked "));
        Serial.print(mySwitch.getClicks());
        Serial.println(F(" times consecutively in quick succession!"));
      }
      break;

    case TactileSwitch::HOLD_LONG:
      {
        Serial.print(F("Switch "));
        Serial.print(mySwitch.getNumber());
        Serial.println(F(" is held down for a long press!"));
      }
      break;

    case TactileSwitch::RELEASED_HOLD_LONG:
      {
        Serial.print(F("Switch "));
        Serial.print(mySwitch.getNumber());
        Serial.println(F(" is released from a long press!"));
      }
      break;

    case TactileSwitch::HOLD_VERY_LONG:
      {
        Serial.print(F("Switch "));
        Serial.print(mySwitch.getNumber());
        Serial.println(F(" is held down for a very long press!"));
      }
      break;

    case TactileSwitch::RELEASED_HOLD_VERY_LONG:
      {
        Serial.print(F("Switch "));
        Serial.print(mySwitch.getNumber());
        Serial.println(F(" is released from a very long press!"));
      }
      break;

    default:
      {
        //nothing
      }
  }

  //get message
  switch (mySwitch.getMessage())
  {
    case TactileSwitch::NO_MESSAGE:
      break;

    case TactileSwitch::BOUNCING_PRESSED:
      {
        Serial.print(mySwitch.getNumber());
        Serial.println(F(": Bouncing press"));
      }
      break;

    case TactileSwitch::BOUNCING_RELEASED:
      {
        Serial.print(mySwitch.getNumber());
        Serial.println(F(": Bouncing released"));
      }
      break;

    case TactileSwitch::POLLING_SLOW:
      {
        Serial.print(mySwitch.getNumber());
        Serial.print(F(": POLLING_SLOW"));
      }
      break;

    default:
      {
        //nothing
      }
  }

}
