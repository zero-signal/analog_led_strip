/*
 * analog_led_strip.ino
 * -------------------- 
 *
 * Analog RGB LED strip controller using PIR sensor and push button
 * 
 * Version 0.1, April 2016
 *
 * Copyright (C) 2016 - Zerosignal (zerosignal1982@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Tested:   arduino 1.0.5
 */
 
#define DEBOUNCE 20          // debounce period in ms
#define HOLDTIME 2000        // hold period in ms

/* pin assignments */
const int redPin   = 10;     // driving pin for red channel
const int greenPin = 11;     // driving pin for green channel
const int bluePin  = 9;      // driving pin for blue channel

const int buttonPin = 7;     // button input pin
const int sensorPin = 2;     // input for PIR sensor

/* state variables */
int brightness = 255;        // default brightness 
int color[3];                // current color storage
int cols[3];                 // temporary color storage
int col;                     // current color (0 = RED, 1 = GREEN, 2 = BLUE, 3 = WHITE)

int sensorState = LOW;       // configure the initial sensor state

boolean sensorMode = true;   // begin in PIR sensor mode

int buttonVal  = 0;          // value read from button
int buttonLast = 0;          // previous button value

long buttonDn;               // time button was pressed
long buttonUp;               // time button was released

boolean ignoreUp;            // ignore button release on long press

/* function using PWM to set LEDs to specified RGB values */
void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

/* function to increase brightness of LEDs */
void increaseBrightness(){
  brightness += 50;
  if(brightness > 255){
    brightness = 255;        // limit maximum brightness
  }  
}

/* function to decrease brightness of LEDs */
void decreaseBrightness(){
  brightness -= 50;
  if(brightness < 50){
    brightness = 50;        // limit minimum brightness
  }    
}

/* function to update LEDs according to current state */
void updateLeds()
{   
  cols[0] = 0;
  cols[1] = 0;
  cols[2] = 0;   

  if(color[0] > 0){      /* red channel */
    cols[0] = brightness;
  } else {
    cols[0] = 0;
  }
  
  if(color[1] > 0){      /* green channel */
    cols[1] = brightness;
  } else {
    cols[1] = 0;
  }
  
  if(color[2] > 0) {     /* blue channel */
    cols[2] = brightness;
  } else {
    cols[2] = 0;
  }     
  
  if(sensorMode) {
    if(sensorState == HIGH) {
      setColor(cols[0], cols[1], cols[2]);      
    } else {
      setColor(0,0,0);
    }
  } else {
      setColor(cols[0], cols[1], cols[2]);      
  }
}

/* function to fade in LEDs from off to currently set brightness value */
void fadeIn() 
{
  cols[0] = 0;
  cols[1] = 0;
  cols[2] = 0;
  
  for (int i=0;i<=brightness;i++)
  {
    if(color[0] > 0){      /* red channel */
      cols[0] = i;
    } else {
      cols[0] = 0;
    }
    
    if(color[1] > 0){      /* green channel */
      cols[1] = i;
    } else {
      cols[1] = 0;
    }
    
    if(color[2] > 0) {     /* blue channel */
      cols[2] = i;
    } else {
      cols[2] = 0;
    }    
    setColor(cols[0], cols[1], cols[2]);
    delay (10);
  }
}

/* function to fade out LEDs from currently set brightness value to off */
void fadeOut()
{
  cols[0] = 0;
  cols[1] = 0;
  cols[2] = 0;
  
  for (int i=brightness;i>=0;i--)
  {
    if(color[0] > 0){      /* red channel */
      cols[0] = i;
    } else {
      cols[0] = 0;
    }
    
    if(color[1] > 0){      /* green channel */
      cols[1] = i;
    } else {
      cols[1] = 0;
    }
    
    if(color[2] > 0) {     /* blue channel */
      cols[2] = i;
    } else {
      cols[2] = 0;
    }    
    setColor(cols[0], cols[1], cols[2]);
    delay (10);
  } 
}

/* function to handle button press */
void handlePress()
{
  /* cycle colors according to current setting */
  switch(col){
    case 0:
      col = 1;
      color[0] = 0;
      color[1] = brightness;
      color[2] = 0;
      break;
    case 1:
      col = 2;
      color[0] = 0;
      color[1] = 0;
      color[2] = brightness;
      break;    
    case 2:
      col = 3;
      color[0] = brightness;
      color[1] = brightness;
      color[2] = brightness;
      break;         
    case 3:
      col = 0;
      color[0] = brightness;
      color[1] = 0;
      color[2] = 0;
      break;       
  }
}

/* function to handle button hold */
void handleHold()
{
  /* toggle sensor or static mode */
  if(sensorMode){
    sensorMode = false;
  } else {
    sensorMode = true;
  }
}

/* function to handle button input */
void handleButton()
{
  /* read button state */
  buttonVal = digitalRead(buttonPin);
  
  /* test for button press and store down time */
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - buttonUp) > long(DEBOUNCE))
  {
    buttonDn = millis();
  }
  
  /* test for button release and store up time */
  if (buttonVal == HIGH && buttonLast == LOW && (millis() - buttonDn) > long(DEBOUNCE)) {
    if (ignoreUp == false){ 
      handlePress();
    } else {
      ignoreUp = false;
    }
    buttonUp = millis();
  }
  
  // Test for button held down for longer than the hold time
  if (buttonVal == LOW && (millis() - buttonDn) > long(HOLDTIME))
  {
    handleHold();
    ignoreUp = true;
    buttonDn = millis();
  }
  
  buttonLast = buttonVal;  
}

/* setup function to configure pins and apply default settings */ 
void setup() 
{  
  pinMode(redPin, OUTPUT);           // initialise LED driving pins
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT); 
 
  pinMode(sensorPin, INPUT);         // declare sensor as input
  pinMode(buttonPin, INPUT_PULLUP);  // declare button as input and enable internal pullup
  
  col = 0;                           // RED is default
  
  color[0] = brightness;             // define RED color values
  color[1] = 0;
  color[2] = 0;
}

/* main program loop */ 
void loop() 
{   
  /* read the PIR sensor pin */
  int val = digitalRead(sensorPin);  
  
  /* decide what to do based on sensor  reading and current state */
  if (val == HIGH) {               // check if the input is HIGH
    if (sensorState == LOW && sensorMode == true) {      // if we were previously LOW we have just turned on   
      fadeIn();
    }
    sensorState = HIGH;    
  } else {                         // otherwise the input is LOW
    if (sensorState == HIGH && sensorMode == true){      // if we were previously HIGH we have just turned off
      fadeOut();
    }
    sensorState = LOW;    
  }    
  
  /* read button state */
  handleButton();
  
  /* update LED state */
  updateLeds();
}
