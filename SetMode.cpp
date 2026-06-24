
#include "commonFuncs.h"

// TODO: confirm this
#define DIGIT_SET_PIN A1

/** returns true when the button to trigger a digit override should be performed */
bool button_to_trigger_set_is_pressed(){
  return false; // TODO: implement this
}


// maps values from 0 to 11 to their proper bit representation for dozonal.
// used by read_digit_set_dial to convert to proper bit representation
byte value_to_bits_map[12] = {
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

// /** sets the cursor in the appropriate place to indicate a digit is selected for overriding */
// void set_cursor_for_selection_blinking(char selector_digit){
//   lcd.setCursor(END_OF_DISPLAY - 1 -2*selector_digit, 0);
// }

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


void do_set_mode(char selectRange, unsigned long *n){

  /// NOTE:
  //       'range' indicates which glyph would be overriden - the position.
  // while 'value' indicates the actual value that would be written there.
  //
  // comments use the word 'digit' loosely, usually regarding the
  // digit at the selected position but check relevant variable names to be sure.
  
  const unsigned long blinkPeriod = 1000; //period of blinks in milliseconds
  
  // whether a digit at the selected range is currently shown during blinking
  // assume initially the digit is showing as that would be the case duing run mode
  bool digitShowing = true;
  
  // write the digit value selected by the other dial on the left side of the screen
  byte value_to_override = read_digit_set_dial();
  write_digit_direct(VAL_SELECTOR_DIGIT, value_to_override);
  
  for(char new_selRange = selectRange; new_selRange >= 0; new_selRange = read_selector_dial()){
    if(new_selRange != selectRange){ // range has changed
      if(!digitShowing){ // if a digit is off
	// show the no longer selected digit
        update_digit(selectRange, *n);
	digitShowing = true;
      }
      selectRange = new_selRange;
    }
    // get the most recent value of the value set dial and update the display to match if needed.
    byte new_val = read_digit_set_dial();
    if(new_val != value_to_override){
      value_to_override = new_val;
      write_digit_direct(VAL_SELECTOR_DIGIT, value_to_override);
      //need_to_fix_cursor = true;
    }
    
    if(button_to_trigger_set_is_pressed()){
      unsigned long new_n = override_digit(*n, selectRange, value_to_override);
      if(new_n != *n){
	*n = new_n;
	update_digit(selectRange, new_n);
	digitShowing = true;
	//need_to_fix_cursor = true;
      }
    }
    bool toShowDigit = (millis()%blinkPeriod < blinkPeriod/2); // ? true:false ;
    if(toShowDigit != digitShowing){
      if(toShowDigit){
        update_digit(selectRange, *n);
      } else {
	erase_digit(selectRange);
      }
      digitShowing = toShowDigit;
    }
  }
  // at this point the selector dial has been moved to running mode (for loop exited)
  
  if(!digitShowing){// restore the selected digit if needed
    update_digit(selectRange, *n);
  }
  // also clear the left side of the screen of the digit we drew there
  erase_digit(VAL_SELECTOR_DIGIT);
}
