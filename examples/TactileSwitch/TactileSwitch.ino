//tactile switch class
#include "TactileSwitch.h"

//3134 Bytes (9%)
//211 Bytes (10%)

//create a TactileSwitch with objectName(analog pin connected, any number, signal you switch HIGH or LOW)
//HIGH
TactileSwitch mySwitch1(A0, 1, HIGH);

//LOW
TactileSwitch mySwitch2(A2, 2, LOW);

//Sparkfun RGB encoder switch is a high mode version. It switches a high
TactileSwitch mySwitch3(A4, 3, HIGH);
//Sparkfun RG encoder switch is a low mode version. It switches a low

//Header for printing utility
void printEvent(int event);

void setup()
{
  // put your setup code here, to run once:
  //use as fast enough serial out, otherwise you miss switch events
  Serial.begin(57600);

}

void loop() {
  // put your main code here, to run repeatedly:

  //if(mySwitch1.checkUserAction() != TactileSwitch::NO_EVENT)
  //processEventTactileSwitch(mySwitch1);

  //if(mySwitch2.checkUserAction() != TactileSwitch::NO_EVENT)
  //processEventTactileSwitch(mySwitch2);

  //mySwitch1.readSwitch();//to measure size of class
  //printEvent(mySwitch1.readSwitch());
  //printEvent(mySwitch2.readSwitch());
  //printEvent(mySwitch3.readSwitch());

  mySwitch1.readSwitch();
  mySwitch2.readSwitch();
  mySwitch3.readSwitch();

  processEventTactileSwitch(mySwitch1);
  processEventTactileSwitch(mySwitch2);
  processEventTactileSwitch(mySwitch3);

}

/*
  void printEvent(int event)
  {
  if (event < 0 )
  {
    Serial.println(F("Switch is held down for a long press."));
  }
  else if (event == 1)
  {
    Serial.println(F("Switch was clicked once."));
  }
  else if (event > 1)
  {
    Serial.print(F("Switch was clicked "));
    Serial.print(event);
    Serial.println(F(" times consecutively in quick succession."));
  }
  else if (event == 0)
  {
    // do nothing when event == 0
  }
  }
*/
void processEventTactileSwitch(TactileSwitch& mySwitch)
{

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

    case TactileSwitch::HOLD_VERY_LONG:
      {
        Serial.print(F("Switch "));
        Serial.print(mySwitch.getNumber());
        Serial.println(F(" is held down for a very long press!"));
      }
      break;

    default:
      {
        //nothing
      }
  }

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

    case TactileSwitch::POLLING_LONG:
      {
        Serial.print(mySwitch.getNumber());
        Serial.println(F(": POLLING_LONG"));
      }
      break;

    default:
      {
        //nothing
      }
  }

}
