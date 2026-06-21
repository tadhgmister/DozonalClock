#include <Arduino.h>
#include "SetMode.h"

unsigned short analogInputRange = 1024;
unsigned long trySetMode(unsigned long n){
  unsigned short halfAnalogInputRange = analogInputRange/2;
  unsigned short digitSelectRange = halfAnalogInputRange/5;
  unsigned long blinkPeriod = 1000; //period of blinks in milliseconds
  bool digitShowing = true; //whether a digit is currently shown during blinking
  bool toShowDigit = true;
  unsigned short currentRange = 6;
  unsigned short nextRange;
  while(1){
    unsigned short a0AnalogValue = analogRead(A0) + 1; //read A0 analog pin, offset by 1 to make math easier
    switch (a0AnalogValue){
      case (a0AnalogValue > halfAnalogInputRange):
       toShowDigit = true;
       nextRange = 6;
       break;
      case (a0AnalogValue > 4*digitSelectRange):
        nextRange = 5;
        break;
      case (a0AnalogValue > 3*digitSelectRange):
        nextRange = 4;
        break;
      case (a0AnalogValue > 2*digitSelectRange):
        nextRange = 3;
        break;
      case (a0AnalogValue > 1*digitSelectRange):
        nextRange = 2;
        break;
      case (a0AnalogValue > 0):
        nextRange = 1;
        break;
    }
     if (nextRange != currentRange){// range has changed
      if (!digitShowing){// if a digit is off
        //TODO:show the digit
        digitShowing = true;
       }
      }
      if(nextRange == 6){
        return n;
      }
     toShowDigit = (millis()%blinkPeriod < blinkPeriod/2) ? true:false;
     if (toShowDigit != digitShowing){
       //TODO: turn the digit corrsponding to the respective range on or off appropriately
       digitShowing = toShowDigit;
     }
  }
}
