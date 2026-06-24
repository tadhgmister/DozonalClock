/** the length of the row in the display */
#define END_OF_DISPLAY 16

#define SELECTOR_PIN A0

// /** returns -1 if in running mode, otherwise 0-4 to select a digit to
//     set This implementation just uses bitshift operations to make the
//     selection very efficiently without caring about the actual
//     desirable selection ranges */
// char read_selector_dial_OVEROPTIMIZED(){
//   // read range [0, 2^10), divide by 2^7 to get into [0,8) range and use 5,6,7 as running mode
//   int selector_val = analogRead(SELECTOR_PIN) >> 7; // read() / 128
//   if(selector_val >= 5 || selector_val < 0){
//     return -1;
//   } else {
//     return (char)selector_val;
//   }
// }
const unsigned short analogInputRange = 1024;
const unsigned short halfAnalogInputRange = (analogInputRange/2);
// this is the range to select digits where we need to multiply by the selector number before the division to avoid extra rounding error.
#define digitSelectRange(x) (((x)*halfAnalogInputRange)/5)
/** returns -1 if in running mode, otherwise 0-4 to select a digit to set.

    this implementation makes the upper half of the selectable range
    correspond to running mode while the other half selects a digit */
char read_selector_dial(){
  
  unsigned short a0AnalogValue = analogRead(A0) + 1;
  if (a0AnalogValue > halfAnalogInputRange){
    return -1; // running mode
  } else if (a0AnalogValue > digitSelectRange(4)){
    return 4;
  } else if (a0AnalogValue > digitSelectRange(3)){
    return 3;
  } else if (a0AnalogValue > digitSelectRange(2)){
    return 2;
  } else if (a0AnalogValue > digitSelectRange(1)){
    return 1;
  } else if (a0AnalogValue > 0){
    return 0;
  }
  else{
    return -2; // idk error case?
  }
}

/** indices of custom characters, the setup function maps these to the actual custom glyphs then the code writes these bytes to the screen to display them */
enum CharIdx {
  C_ONE_OFF = 0,
  C_ONE_ON,
  C_TWO_OFF,
  C_TWO_ON,
  C_THREE_OFF,
  C_THREE_ON,
  C_SIX_OFF,
  C_SIX_ON,
};


// /** updates a dozonal character with an attempt to avoid unnecessary character write and cursor operations
//     it is very likely this function is totally extraneous and just using the write_digit would be sufficient, this only exists for effeciency sake */
// void update_digit(byte idx, byte n, byte old_n){
//   if((n&2) != (old_n&2)){
//     byte top_left     = (n&2) ? C_TWO_ON : C_TWO_OFF;
//     lcd.setCursor(2*idx,0);
//     lcd.write(top_left);
//   }
//   if((n&1) != (old_n&1)){
//     byte top_right  = (n&1) ? C_ONE_ON : C_ONE_OFF;
//     if((n&2) == (old_n&2)){
//       lcd.setCursor(2*idx + 1, 0);
//     }
//     lcd.write(top_right);
//   }
  
//   if((n&8) != (old_n&8)){
//     byte bottom_left  = (n&8) ? C_SIX_ON : C_SIX_OFF;
//     lcd.setCursor(2*idx,1);
//     lcd.write(bottom_left);
//   }
//   if((n&4) != (old_n&4)){
//     byte bottom_right = (n&4)? C_THREE_ON:C_THREE_OFF;
//     if((n&8) == (old_n&8)){
//       lcd.setCursor(2*idx + 1, 1);
//     }
//     lcd.write(bottom_right);
//   }
// }
/** the digit index to pass to erase_digit or update_digit for the digit selected by the value selector dial */
#define VAL_SELECTOR_DIGIT 7

/** maps number digit (0 to 4) to the character index on the lcd */
#define DIG_TO_IDX(idx) (byte)(END_OF_DISPLAY - 2 - 2*(idx))
/** replaces the space of a digit with empty space, where:
    - idx=0 is the least significant space on the screen (right most),
    - idx=4 is the most significant digit of the normal clock display,
    - idx=7 is the left most space that can be displayed on a 16 character wide display */
void erase_digit(byte dig){
  lcd.setCursor(DIG_TO_IDX(dig),0);
  lcd.print("  ");
  lcd.setCursor(DIG_TO_IDX(dig),1);
  lcd.print("  ");
}

/** unconditionally writes a dozonal character in the given offset,
 the glyph written is 2 characters wide and spreads over both rows of
 text so the next digit should be aligned 2 indices after */
void write_glyph_raw(byte idx, char n){
  byte top_right    = (n&1) ? C_ONE_ON : C_ONE_OFF;
  byte top_left     = (n&2) ? C_TWO_ON : C_TWO_OFF;
  byte bottom_right = (n&4)? C_THREE_ON:C_THREE_OFF;
  byte bottom_left  = (n&8) ? C_SIX_ON : C_SIX_OFF;

  lcd.setCursor(idx,0);
  lcd.write(top_left);
  lcd.write(top_right);

  lcd.setCursor(idx,1);
  lcd.write(bottom_left);
  lcd.write(bottom_right);
}
/** writes the 5 dozonal digits of the number to the right side lcd display. */
void write_number(unsigned long n){
  write_glyph_raw(END_OF_DISPLAY-10,(byte)(n>>16));
  write_glyph_raw(END_OF_DISPLAY-8, (byte)(n>>12));
  write_glyph_raw(END_OF_DISPLAY-6, (byte)(n>> 8));
  write_glyph_raw(END_OF_DISPLAY-4, (byte)(n>> 4));
  write_glyph_raw(END_OF_DISPLAY-2, (byte)(n>> 0));
}


/** writes a digit to the display where dig is the digit offset (0 is right most) and the value contains the digit in the least significant 4 bits (I.E. 0x0 to 0xF) */
void write_digit_direct(byte dig, char value){
  write_glyph_raw(DIG_TO_IDX(dig), value);
}

/** updates just the digit at the given place for example:
 - digit=0 sets the bits (n & 0x0000F) to the right most position on screen
 - digit=4 sets the bits (n & 0xF0000) to the position that has 4 digits visible to the right of it*/
void update_digit(byte digit, unsigned long n){
  write_digit_direct(digit, (byte)(n>>(4*digit)));
}
