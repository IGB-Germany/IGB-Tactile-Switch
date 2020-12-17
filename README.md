Library for Tactile Switches
================================================

This library allows the user to:
* Read digital switches connected to digital pins
* Read various events
* Read various messages
* Count clicks
* Debounces pressing and releasing
* Configuration for high or low side

Examples are included to get you started.

Repository Contents
-------------------

* **/examples** - Example sketches for the library (.ino). Run these from the Arduino IDE. 
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE. 
* **library.properties** - General library properties for the Arduino package manager. 

Documentation
--------------
The library enables the following functions:

#### General methods for class
###### TactileSwitch(uint8_t buttonPin, uint8_t number, uint8_t mode = HIGH, bool enablePullUp = false)

#### Get events and messages
###### event_t getEvent() const;
###### message_t getMessage() const;
###### void readSwitch(void);

#### Get various information
###### int8_t getClicks (void) const;
###### uint8_t getState();

#### Configuration
###### void setMultiClickFeature(bool enable);
###### void setDurationDebouncePressed(uint8_t durationDebouncePressed);
###### void setDurationDebounceReleased(uint8_t durationDebounceReleased);

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact igb@posteo.de.

Distributed as-is; no warranty is given.
