/** the length of the row in the display */
#define END_OF_DISPLAY 16

#define SELECTOR_PIN A0
/** returns -1 if in running mode, otherwise 0-4 to select a digit to set */
char read_selector_dial(){
  // read range [0, 2^10), divide by 2^7 to get into [0,8) range and use 5,6,7 as running mode
  int selector_val = analogRead(SELECTOR_PIN) >> 7; // read() / 128
  if(selector_val >= 5 || selector_val < 0){
    return -1;
  } else {
    return selector_val;
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
/** unconditionally writes a dozonal character in the given offset,
 the glyph written is 2 characters wide and spreads over both rows of
 text so the next digit should be aligned 2 indices after */
void write_digit(byte idx, char n){
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
  write_digit(END_OF_DISPLAY-10,n>>16);
  write_digit(END_OF_DISPLAY-8, n>>12);
  write_digit(END_OF_DISPLAY-6, n>> 8);
  write_digit(END_OF_DISPLAY-4, n>> 4);
  write_digit(END_OF_DISPLAY-2, n>> 0);
}
