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
#include "dateHandling.cpp"
#include "RunMode.cpp"
#include "SetMode.cpp"

typedef byte GLYPH[8];

GLYPH ONE_OFF = {
    B00000,
    B00000,
    B00000,
    B00000,
    B11100,
    B00100,
    B00100,
    B00000,
};
GLYPH ONE_ON = {
    B00000,
    B00000,
    B00000,
    B00000,
    B11100,
    B11100,
    B11100,
    B00000,
};
GLYPH TWO_OFF = {
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B10000,
    B10000,
    B00000,
};
GLYPH TWO_ON = {
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,
    B11111,
    B00000,
};

GLYPH THREE_OFF = {
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B11100,
    B00000,
};
GLYPH THREE_ON = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B00000,
};

GLYPH SIX_OFF = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B11111,
    B00000,
};
GLYPH SIX_ON = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B00000,
};

// ㍙=1, ㍚=2, ㍛=4, ㍜=5, ㍝=6, ㍞=8, ㍟=9, ㍠=A, ㍡=C, ㍢=D, ㍣=E
//                0x12345ul
unsigned long n = 0x00000ul;

Date d = Date(2026, 6, 30);

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
  d.write_date(lcd);
  write_number(n);
}


/** Performs run mode until selector dial suggests we should switch to
    set mode, then runs set mode until dial goes back to run mode.
    Running this function repeatedly will effectively allow either
    operation to run as needed. */
void loop()
{
  byte selected_dig = do_running(&n, d);

  do_set_mode(selected_dig, &n);
}
