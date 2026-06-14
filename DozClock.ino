/*
  By Andryko _______ and Tadhg McDonald-Jensen
  program to display time in dozonal on LCD screen.
  
  see https://tadhg.mcdonald-jensen.ca/clockAndCheatsheet.html for more details
  TODO: add link to more complete dozonal explanation once it is published.
*/



  
/* typedef struct DozonalClockElemStruct */
/* { */
/*   byte OFF[8]; */
/*   byte ON[8]; */
/* } DozonalClockElem; */
  
/* DozonalClockElem topLeft = */
/* { */
/*   { */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b00111, */
/*     0b00100, */
/*     0b00100, */
/*   }, */
/*   { */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b00111, */
/*     0b00111, */
/*     0b00111, */
/*   } */
/* }; */

/* DozonalClockElem topRight = */
/* { */
/*   { */
/* d    0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b11111, */
/*     0b00001, */
/*     0b00001, */
/*   }, */
/*   { */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b00000, */
/*     0b11111, */
/*     0b11111, */
/*     0b11111, */
/*   } */
/* }; */

/* DozonalClockElem bottomRight = */
/* { */
/*   { */
/*     0b00001, */
/*     0b00001, */
/*     0b00001, */
/*     0b00001, */
/*     0b00001, */
/*     0b00001, */
/*     0b11111, */
/*     0b00000, */
/*   }, */
/*   { */
/*     0b11111, */
/*     0b11111, */
/*     0b11111, */
/*     0b11111, */
/*     0b11111, */
/*     0b11111, */
/*     0b11111, */
/*     0b00000, */
/*   } */
/* }; */

/* DozonalClockElem bottomLeft = */
/* { */
/*   { */
/*     0b00100, */
/*     0b00100, */
/*     0b00100, */
/*     0b00100, */
/*     0b00100, */
/*     0b00100, */
/*     0b00111, */
/*     0b00000, */
/*   }, */
/*   { */
/*     0b00111, */
/*     0b00111, */
/*     0b00111, */
/*     0b00111, */
/*     0b00111, */
/*     0b00111, */
/*     0b00111, */
/*     0b00000 */
/*   } */
/* }; */



#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // Format -> (Address,Width,Height )


typedef byte GLYPH[8];

GLYPH ONE_OFF = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11100,
    0b00100,
    0b00100,
};
GLYPH ONE_ON = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11100,
    0b11100,
    0b11100,
};
GLYPH TWO_OFF = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b10000,
    0b10000,
};
GLYPH TWO_ON = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
};

GLYPH THREE_OFF = {
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b11100,
    0b00000,
};
GLYPH THREE_ON = {
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b00000,
};

GLYPH SIX_OFF = {
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b11111,
    0b00000,
};
GLYPH SIX_ON = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b00000,
};

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

void update_digit(byte idx, byte n, byte old_n){
  if((n&2) != (old_n&2)){
    byte top_left     = (n&2) ? C_TWO_ON : C_TWO_OFF;
    lcd.setCursor(2*idx,0);
    lcd.write(top_left);
  }
  if((n&1) != (old_n&1)){
    byte top_right  = (n&1) ? C_ONE_ON : C_ONE_OFF;
    if((n&2) == (old_n&2)){
      lcd.setCursor(2*idx + 1, 0);
    }
    lcd.write(top_right);
  }
  
  if((n&8) != (old_n&8)){
    byte bottom_left  = (n&8) ? C_SIX_ON : C_SIX_OFF;
    lcd.setCursor(2*idx,1);
    lcd.write(bottom_left);
  }
  if((n&4) != (old_n&4)){
    byte bottom_right = (n&4)? C_THREE_ON:C_THREE_OFF;
    if((n&8) == (old_n&8)){
      lcd.setCursor(2*idx + 1, 1);
    }
    lcd.write(bottom_right);
  }
}

void write_digit(byte idx, byte n){
  byte top_right    = (n&1) ? C_ONE_ON : C_ONE_OFF;
  byte top_left     = (n&2) ? C_TWO_ON : C_TWO_OFF;
  byte bottom_right = (n&4)? C_THREE_ON:C_THREE_OFF;
  byte bottom_left  = (n&8) ? C_SIX_ON : C_SIX_OFF;

  lcd.setCursor(2*idx,0);
  lcd.write(top_left);
  lcd.write(top_right);

  lcd.setCursor(2*idx,1);
  lcd.write(bottom_left);
  lcd.write(bottom_right);
}
/** increments a dozonal integer such that each digit is represented
    as 4 bits (one hexadecimal character) specifically such that
    x&(3<<(4*n)) != 3 for every value of n.

    I.E. the lowest 2 bits are not set at the same time of each nibble.
    I.E. writing the number in hex will never show these characters:
    3 = 0011
    7 = 0111
    B = 1011
    F = 1111
    
 */
unsigned long increment_doz(unsigned long x){
  x += 1;
  unsigned int idx = 0;
  while((x & (3ul<<idx)) == (3ul<<idx) && (3ul<<idx) > 0){
    x += 1ul<<idx;
    idx += 4;
  }
  return x;
}

void setup()
{
  // initialize the lcd
  lcd.init();
  // Turn on the Backlight
  lcd.backlight();
  // Create a custom character
  lcd.createChar(C_ONE_OFF, ONE_OFF);
  lcd.createChar(C_ONE_ON, ONE_ON);
  lcd.createChar(C_TWO_OFF, TWO_OFF);
  lcd.createChar(C_TWO_ON, TWO_ON);
  lcd.createChar(C_THREE_OFF, THREE_OFF);
  lcd.createChar(C_THREE_ON, THREE_ON);
  lcd.createChar(C_SIX_OFF, SIX_OFF);
  lcd.createChar(C_SIX_ON, SIX_ON);
  /* lcd.createChar(0, topLeft.OFF); */
  /* lcd.createChar(1, topLeft.ON); */
  /* lcd.createChar(2, topRight.OFF); */
  /* lcd.createChar(3, topRight.ON); */
  /* lcd.createChar(4, bottomLeft.OFF); */
  /* lcd.createChar(5, bottomLeft.ON); */
  /* lcd.createChar(6, bottomRight.OFF); */
  /* lcd.createChar(7, bottomRight.ON); */
  
}
void loop()
{
  // Clear the display buffer
  lcd.clear(); 
  // Set cursor (Column, Row)
  //lcd.setCursor(0, 0);
//  // print "Hello" at (0, 0)   
//  lcd.print("Hello"); 
//  // Set cursor (Column, Row) 
//  lcd.setCursor(0,1);
//  // print "Tadhg" at (0, 1)
//  lcd.print("Tadhg");
  unsigned long n = 0x88000ul;// 0xD5000ul;
  
  write_digit(0, n>>16);
  write_digit(1, n>>12);
  write_digit(2, n>>8);
  write_digit(3, n>>4);
  write_digit(4, n>>0);
  /* /\** goes 0 to 8, extra millisecond occurs on 0 and 5 *\/ */
  byte cycle = 0; 
  /** this tracks how much time has passed to keep the clock updating
      at precise and accurate rate even if the specific delays between
      refresh is not perfectly accurate.

     Each tick of the clock should be 347.222222 milliseconds from the last tick
     (25/72 s/tick) and the delay is set to account for the time it takes to calculate the next number and update the display */
  unsigned long timekeep = millis();
  while(1){
    unsigned long new_n = increment_doz(n);
    update_digit(0, new_n>>16, n>>16);
    update_digit(1, new_n>>12, n>>12);
    update_digit(2, new_n>>8,  n>>8);
    update_digit(3, new_n>>4,  n>>4);
    update_digit(4, new_n>>0,  n>>0);

    n = new_n;
    timekeep += 346;

    /* Implementing 11/32 as close approximation to measured 1806033/5262500 */
    cycle += 1;
    if(cycle >= 32){
      cycle = 0;
    }
    // ever even number (16/32) except for 5 which are picked to be roughly equally spaced (11/32)
    if ((cycle & 1)==0 && cycle != 4 && cycle != 10 && cycle !=16 && cycle != 22 && cycle != 28){
      timekeep += 1;
    }
    unsigned long t_2 = millis();
    if((long)(timekeep - t_2) < 0){
      lcd.print("Error!");
      return;
    }
    
    delay(timekeep - t_2);
  }
}
