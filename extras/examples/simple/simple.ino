/*
 * simple.ino
 * Version 0.1, 2018-05-21.
 * by Michel Deslierres 
 * 
 * Simple example sketch showing use of the 
 * mdButton push button library for Arduino
 * 
 * Tested with 
 *   ESP8266 (WemosD1 mini and Sonoff switch) and a 
 *   Sparkfun RedBoard (Arduino UNO clone).     
 * 
 */

#include "mdButton.h"

#ifdef ESP8266
  #define BIT_RATE    115000
  #define BUTTON_PIN  0      // The Sonoff tactile switch is on GPIO 0 (D3 on NodeMCU/Wemos D1 mini)
#else
  #define BIT_RATE    9600
  #define BUTTON_PIN  10     // The tactile switch is connected to pin 10 on Arduino UNO
#endif

mdButton button(BUTTON_PIN);  // Create the object; default time delays are set.

void setup() {
  Serial.begin(BIT_RATE);
  delay(100);
  Serial.println("\n\nSimple demonstration sketch for mdButton object.");
  Serial.print("Assumes a push button is connected to pin ");
  Serial.print(BUTTON_PIN);
  Serial.println(".");
  
  // This would be a good place to change the default time attributes if necessary
  //
  // button.setDebouncePressTime(10);    // DEFAULT_DEBOUNCE_PRESS_TIME = 15 ms
  // button.setDebouncePressTime(25);    // DEFAULT_DEBOUNCE_RELEASE_TIME = 30 ms
  // button.setDebouncePressTime(350);   // DEFAULT_MULTI_CLICK_TIME = 400 ms
  // button.setDebouncePressTime(1750);  // DEFAULT_HOLD_TIME = 2000 ms
  //
  // Use the example sketch tune.ino to find better values
  
  Serial.println("setup() completed.");
  Serial.println("\nPress the button multiple times to test.");
}

void loop() {
  delay(5);
  int n = button.status();
  if (n < 0 ) {
      Serial.println("The button was held down for a long press.");
  } else if (n == 1) {
      Serial.println("The button was clicked once.");
  } else if (n > 0) {  
      // n > 0 when here
      Serial.print("The button was clicked ");
      Serial.print(n);
      Serial.println(" times consecutively in quick succession.");
  }
  // do nothing when n == 0
}
