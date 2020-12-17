IGB Arduino Library for Tactile Switches
================================================

* Uses digital pins
* Debounces and reads a single press, a double press, a long hold, a very long hold
* Configuration high or low side

This library allows the user to:
* Read digital pins connected to switches
* Get various information about events
* Configuration of the events

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
<hr>

#### General methods for class
<hr>

###### TactileSwitch(uint8_t buttonPin, uint8_t number, uint8_t mode = HIGH, bool enablePullUp = false)
<hr>
#### Read switch

###### checkUserAction()
###### action getEvent();
	
<hr>

#### Get various information
<hr>


License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact igb@posteo.de.

Distributed as-is; no warranty is given.
