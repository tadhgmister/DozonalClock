
#include "commonFuncs.h"

// TODO: confirm this
#define DIGIT_SET_PIN A1

/** returns true when the button to trigger a digit override should be performed */
bool button_to_trigger_set_is_pressed(){
  return false; // TODO: implement this
}


// maps values from 0 to 11 to their proper bit representation for dozonal.
// used by read_digit_set_dial to convert to proper bit representation
unsigned short value_to_bits_map[12] = {
   0,  1,  2, // 0b00xx
   4,  5,  6, // 0b01xx
   8,  9, 0xA,// 0b10xx
  0xC,0xD,0xE // 0b11xx
};

/** returns bit representation of digit to override when triggered (I.E. logical digit from 0 to 11, mapped to one hexidecimal character) */
byte read_digit_set_dial(){
  int dig_selector = analogRead(DIGIT_SET_PIN);
  if (dig_selector < 0 || dig_selector >= 1024){
    return 0; // should never happen but better to be sure this function always returns a valid digit.
  }
  // dig_selector is up to 2^10, dividing by 2^8 gives 0-3 which is range for 'sticks' (multiples of 3)
  int dig = 3*(dig_selector >> 8); // start with sticks, read() / 256

  // the lower digits are below 2^8 (I.E. up to 255 = 0xFF) which we divide by 3
  // 0xF/3 = 15/3 = 0x5, and 2*5 = 0xA and our range is 0xFF so our boundaries are 0x55 and 0xAA.
  // note that since 255 is a possible value we have 86 possible values for one of our ranges and 85=0x55 for the other 2
  // by using < we make the lowest bucket have the extra space, if we used <= we'd put more space in the 2 dot bucket.
  if((dig_selector & 0xFF) > 0xAA){
    dig += 2;
  } else if((dig_selector & 0xFF) > 0x55){
    dig += 1;
  } // else{ dig += 0;}
  return value_to_bits_map[dig];
}

/** sets the cursor in the appropriate place to indicate a digit is selected for overriding */
void set_cursor_for_selection_blinking(char selector_digit){
  lcd.setCursor(END_OF_DISPLAY - 1 - 2*selector_digit, 0);
}

/** 
  overrides one dozonal digit as performed by setting mode
  n - the original number to edit
  digit_index - index from 0 (least significant digit) to 4 (most significant displayed digit)
  value - value in dozonal bit representation to set
  returns the new value of n with the chosen digit overriden
 */
unsigned long override_digit(unsigned long n, int digit_index, byte value){
   // first clear the bits of the corresponding digit
   unsigned long clearing_bits = 0xF<<(digit_index*4);
   // ~ inverts bits so the 0b1111 on the chosen digit will be set to 0s and the rest will be 1s
   // then & clears all bits that are 0 in the mask and leaves all bits that are 1 in the mask
   n = n & ~clearing_bits;
   
   // filter to only edit one digit in case of some mistake
   // also cast to unsigned long so it is the rigth type when we left shift it to the rigth spot.
   unsigned long new_dig = value & 0xF;
   
   // use | to set bits of the new digit
   return n | (new_dig << (digit_index*4));
}


void do_set_mode(char selector_dig, unsigned long *n){
  // we will want to blink the cursor at the selected digit so enable blinking
  lcd.blink();
  // there are a bunch of cases where we might redraw the screen, any will raise this flag and then at the end of the loop we return the cursor to the actual selected position.
  bool need_to_fix_cursor = true;
  
  // write the digit selected by the other dial on the left side of the screen
  byte value_to_override = read_digit_set_dial();
  write_digit(0, value_to_override);
  
  for(char new_selection = selector_dig; new_selection != -1; new_selection = read_selector_dial()){
    if(new_selection != selector_dig){
      selector_dig = new_selection;
      need_to_fix_cursor = true;
    }
    byte new_val = read_digit_set_dial();
    if(new_val != value_to_override){
      value_to_override = new_val;
      write_digit(0, value_to_override);
      need_to_fix_cursor = true;
    }
    
    if(button_to_trigger_set_is_pressed()){
      unsigned long new_n = override_digit(*n, selector_dig, value_to_override);
      if(new_n != *n){
	*n = new_n;
	write_number(new_n);
	need_to_fix_cursor = true;
      }
    }
    if(need_to_fix_cursor){
      set_cursor_for_selection_blinking(selector_dig);
      need_to_fix_cursor = false;
    }
  }
  // selection has gone back to -1 for running mode,
  // disable blinking as it will not matter for running mode.
  lcd.noBlink();
  // also clear the left side of the screen of the digit we drew there
  lcd.setCursor(0,0);
  lcd.print("  ");
  lcd.setCursor(0,1);
  lcd.print("  ");
}



	      
// unsigned long trySetMode(unsigned long n){
//   unsigned short halfAnalogInputRange = analogInputRange/2;
//   unsigned short digitSelectRange = halfAnalogInputRange/5;
//   unsigned long blinkPeriod = 1000; //period of blinks in milliseconds
//   bool digitShowing = true; //whether a digit is currently shown during blinking
//   bool toShowDigit = true;
//   unsigned short currentRange = 6;
//   unsigned short nextRange;
//   while(1){
//     unsigned short a0AnalogValue = analogRead(A0) + 1; //read A0 analog pin, offset by 1 to make math easier

//     if (a0AnalogValue > halfAnalogInputRange){
//       toShowDigit = true;
//       nextRange = 6;
//     }
//     else if (a0AnalogValue > 4*digitSelectRange){
//       nextRange = 5;
//     }
//     else if (a0AnalogValue > 3*digitSelectRange){
//       nextRange = 4;
//     }
//     else if (a0AnalogValue > 2*digitSelectRange){
//       nextRange = 3;
//     }
//     else if (a0AnalogValue > 1*digitSelectRange){
//       nextRange = 2;
//     }
//     else if (a0AnalogValue > 0){
//       nextRange = 1;
//     }
//      if (nextRange != currentRange){// range has changed
//       if (!digitShowing){// if a digit is off
//         //TODO:show the digit
//         digitShowing = true;
//        }
//       }
//       if(nextRange == 6){
//         return n;
//       }
//      toShowDigit = (millis()%blinkPeriod < blinkPeriod/2) ? true:false;
//      if (toShowDigit != digitShowing){
//        //TODO: turn the digit corrsponding to the respective range on or off appropriately
//        digitShowing = toShowDigit;
//      }
//   }
// }

