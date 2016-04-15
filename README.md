analog_led_strip
----------------

Analog RGB LED strip controller using PIR sensor and push button

Requirements
------------

- Arduino Uno (or equivalent)
- Analog LED strip
- DFRobot RGB LED Strip shield
- Pushbutton, normally open (NO)
- PIR Sensor
- 12V, 5A power supply
- Miscellaneous jumper wire

Explanation
-----------

Arduino sketch to control an Analog RGB LED strip using a PIR sensor
and a single pushbutton. When motion is detected via the PIR sensor
the RGB LED strip is controlled via PWM using the Arduino Uno (or
similar) to fade the LEDs from OFF to full brightness.

When motion is no longer detected, the RGB LEDs are once again
controlled via PWM from full brightness to OFF.

The Arduino sketch can currently detect short and long presses of the
attached pushbutton to perform the following actions:

- **Short press:** Cycle colors of RGB LED strip between Red, Green, Blue
and White.
- **Long press:** Cycle between sensor and non-sensor mode. In sensor mode
the RGB LEDs are animated based on motion detection. In non-sensor mode
the LEDs are turned on full time and PIR sensor input is ignored.

Wiring
------

Currently, the following wiring configuration is used:

- PIR sensor attached to Arduino Uno digital pin 2
- Pushbutton attached to Arduino Uni digital pin 7

Analog RGB LED strip should be connected to the DFRobot LED strip shield
as per the instructions [here](http://www.dfrobot.com/wiki/index.php/RGB_LED_Strip_Driver_Shield_SKU%EF%BC%9ADFR0274)

- Finally, the 12V, 5A power supply should be connected to the DFRobot shield
and **NOT** the Arduino Uno

License & Copyright
-------------------
  
Copyright (C) 2016 - Zerosignal (zerosignal1982@gmail.com)
  
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
         
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
               
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
