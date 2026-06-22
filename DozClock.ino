/*
  By Andriyko Goyaniuk and Tadhg McDonald-Jensen
  program to display time in dozonal on LCD screen.
  
  see https://tadhg.mcdonald-jensen.ca/clockAndCheatsheet.html for more details
  TODO: add link to more complete dozonal explanation once it is published.
*/



#include <LiquidCrystal_I2C.h>
// define the lcd first as other files end up using it.
LiquidCrystal_I2C lcd(0x27, 16, 2); // Format -> (Address,Width,Height )


#include "commonFuncs.cpp"
#include "RunMode.cpp"
#include "SetMode.cpp"

typedef byte GLYPH[8];

GLYPH ONE_OFF = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11100,
    0b00100,
    0b00100,
    0b00000,
};
GLYPH ONE_ON = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11100,
    0b11100,
    0b11100,
    0b00000,
};
GLYPH TWO_OFF = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b10000,
    0b10000,
    0b00000,
};
GLYPH TWO_ON = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b00000,
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

// ㍙=1, ㍚=2, ㍛=4, ㍜=5, ㍝=6, ㍞=8, ㍟=9, ㍠=A, ㍡=C, ㍢=D, ㍣=E
//                0x12345ul
unsigned long n = 0x00000ul;

void setup()
{
  // initialize the lcd
  lcd.init();
  // Turn on the Backlight
  //lcd.backlight();
  
  // Create custom characters for dozonal glyphs
  lcd.createChar(C_ONE_OFF, ONE_OFF);
  lcd.createChar(C_ONE_ON, ONE_ON);
  lcd.createChar(C_TWO_OFF, TWO_OFF);
  lcd.createChar(C_TWO_ON, TWO_ON);
  lcd.createChar(C_THREE_OFF, THREE_OFF);
  lcd.createChar(C_THREE_ON, THREE_ON);
  lcd.createChar(C_SIX_OFF, SIX_OFF);
  lcd.createChar(C_SIX_ON, SIX_ON);
  
  
  // Clear the display buffer then display the initial number to render.
  lcd.clear();
  write_number(n);
}


/** Performs run mode until selector dial suggests we should switch to
    set mode, then runs set mode until dial goes back to run mode.
    Running this function repeatedly will effectively allow either
    operation to run as needed. */
void loop()
{
  byte selected_dig = do_running(&n);

  do_set_mode(selected_dig, &n);
}
