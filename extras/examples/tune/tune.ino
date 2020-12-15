/*
 * tune.ino
 * Version 0.1, 2018-05-21.
 * By Michel Deslierres
 * 
 * An Arduino sketch to "tune" the various delays used in the 
 * mdButton object.
 * 
 * This sketch has a serial command line interpreter (to use in Arduino Serial Monitor)
 * with the following commands:
 * 
 *   help                           - displays the list of commands
 *   times                          - will display button time attributes
 *   debouncePressTime <value>      - will set button.debouncePressTime_ to <value>
 *   debounceReleaseTime <value>    - will set button.debounceReleaseTime_ to <value>
 *   debounceMulticlickTime <value> - will set button.debounceMultiClickTime_ to <value>
 *   debounceHoldTime <value>       - will set button.debounceHoldTime_ to <value>
 *   
 *      - <value> must be between 0 and 10000 (milliseconds)
 *      - the commands are not case sensitive
 *      
 * Tested with 
 *    ESP8266 (WemosD1 mini and Sonoff switch) and a 
 *    Sparkfun RedBoard (Arduino UNO clone).     
 * 
 */

#include "mdButton.h"

#ifdef ESP8266
  #define BIT_RATE 115000
  #define BUTTON_PIN     0    // The Sonoff tactile switch is on GPIO 0 (D3 on NodeMCU/D1 mini)
#else
  #define BIT_RATE 9600
  #define BUTTON_PIN     10   // The tactile switch connected to pin 10 on Arduino UNO
#endif


mdButton button(BUTTON_PIN);

static const char *cmds[6];
static const char *helps[6];

uint16_t debouncePressTime   = DEFAULT_DEBOUNCE_PRESS_TIME; 
uint16_t debounceReleaseTime = DEFAULT_DEBOUNCE_RELEASE_TIME;
uint16_t multiClickTime      = DEFAULT_MULTI_CLICK_TIME;
uint16_t holdTime            = DEFAULT_HOLD_TIME;

void setup() {
  Serial.begin(BIT_RATE);
  cmds[0] = "help";
  cmds[1] = "times";
  cmds[2] = "debouncepresstime";
  cmds[3] = "debouncereleasetime";
  cmds[4] = "multiclicktime";
  cmds[5] = "holdtime";
  
  helps[0] = " - print this output";
  helps[1] = " - display the object time attributes";
  helps[2] = " <n> - set debounce time after button is pressed to <n> ms";
  helps[3] = " <n> - set debounce time after button is released to <n> ms";
  helps[4] = " <n> - set time waiting for a button press in quick succession after the last press to <n> ms";
  helps[5] = " <n> - set time to hold down the button for a long press to <n> ms";
  
  Serial.println("\n\nApplication started");
  Serial.println("setup() completed");
  
  printHelp();
}

void loop() {
  buttonModule();
  inputModule(); 
}


void buttonModule(void) {
  int n = button.status();
  if (n < 0 ) {
    Serial.println("button was held down for a long press");
  } else if (n == 1) {
    Serial.println("button was clicked once");
  } else if (n > 0) {
    Serial.print("button was clicked ");
    Serial.print(n);
    Serial.println(" times consecutively");
  }
}

#define TOKENCOUNT 3
String token[TOKENCOUNT];

int parseString(const char *s) {

  for (int i = 0; i < TOKENCOUNT; i++) {
    token[i] = "";
  }
  int ndx = 0;
  int tokCounter = 0;
  int len = 0;

  // skip leading spaces
  while (s[ndx] == ' ') ndx++;

  while (1) {
    char inChar = s[ndx];
    ndx++;
    if ( (len > 0) && ((inChar == ' ') || (inChar == 0)) ) {
      tokCounter += 1;
      len = 0;
    }
    if ( (inChar == 0) || (tokCounter >= TOKENCOUNT) ) return tokCounter;
    if (inChar != ' ') {
      token[tokCounter] += inChar;
      len++;
    } else {
      // skip extra spaces following inChar == ' '
      while (s[ndx] == ' ') ndx++;
    }
  }
}

void printHelp() {
  Serial.println();
  Serial.println("commands:");
  for (int i = 0; i < 6 ; i++) {
      Serial.print("  ");
      Serial.print(cmds[i]);
      Serial.println(helps[i]);
  }
}  
   
void inputModule() {
  
  static String inputString;
  boolean stringComplete = false;

  while (Serial.available() && !stringComplete) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
  if (stringComplete) {
    int index = 0;
    int counter = parseString(inputString.c_str());
    inputString = "";
    long nvalue = 0;
    int cmd = -1;
    if (counter < 1) { return; } 
    
    token[0].toLowerCase();
    for (int i = 0; i < 6; i++) {
      if (token[0].equals(cmds[i])) {
        cmd = i;
        break;
      }
    } 

    if (cmd < 0) {
      Serial.print("\"");
      Serial.print(token[0]);
      Serial.println("\" is unknown");
      return;
    }  

    if (((cmd < 2) && (counter != 1)) || ((cmd > 1) && (counter != 2))) {
      Serial.println("wrong number of parameters");
     return;
    }

    if (counter == 2) {
      nvalue = token[1].toInt();
      if ((nvalue < 0) || (nvalue > 10000)) {
        Serial.print(token[1]);
        Serial.println(" is out of range");
        return;
      }
    }
    
    switch(cmd) {
      case 0: printHelp(); break;
      //case 1 - fall through 
      case 2: button.setDebouncePressTime(nvalue); debouncePressTime = nvalue; break;
      case 3: button.setDebounceReleaseTime(nvalue);  debounceReleaseTime = nvalue; break;
      case 4: button.setMultiClickTime(nvalue);  multiClickTime = nvalue; break;
      case 5: button.setHoldTime(nvalue); holdTime = nvalue; break;
    }

    if (cmd > 0) {
        // print times after each command 
        Serial.print("DebouncePressTime   "); Serial.println(debouncePressTime);   
        Serial.print("DebounceReleaseTime "); Serial.println(debounceReleaseTime);   
        Serial.print("MultiClickTime      "); Serial.println(multiClickTime);   
        Serial.print("HoldTime            "); Serial.println(holdTime);   
    }    
  }  
}
