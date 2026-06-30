

#include <DateTimeFunctions.h>
#include <LiquidCrystal_I2C.h>
DateTimeFunctions dTF;


class Date {
public:
  /** gets the day corresponding to Unix epoch - 1970 Jan 1st */
  Date epoch(){
    return Date(1970, 1, 1);
  }
  /** creates a new date.
      
      Note if the month is not in range [1,12] it is set to 1 (January)
      Note that if the day of the month is invalid (such as being 0) it will be set to the last day of the month
  */
  Date(uint16_t y, uint8_t m, uint8_t day){
    year = y;
    if(1<= m && m <=12){
      month = m;
    } else{
      // WARN("invalid month, using January instead");
      month = 1;
    }
    if(1<= day && day <= dTF.getDIM(month, year)){
      day_of_month = day;
    } else {
      // WARN("INVALID DAY, using the last day of the month");
      day_of_month =  dTF.getDIM(month, year);
    }
  }
  /** change to the next day */
  void increment(){
    if(day_of_month < dTF.getDIM(month, year)){
      day_of_month++;
      return;
    } // otherwise we are at the last day of the month
    day_of_month = 1;
    if(month < 12){
      month++;
      return;
    }
    // we are at the last day of december, roll over year
    month = 1;
    year++;
  }
  /** writes the date to the left side of the lcd formatted as:

      YYYY W
      MMM DD

      Where:
      - YYYY is the year (not strictly 4 digits, will simply be left aligned)
      - W is a single letter for the weekday with Sunday=U, Thursday=R
      - MMM is month abbreviation like Jan, Feb, Mar, etc.
      - DD is zero padded day of the month like 09 or 31
  */
  void write_date(LiquidCrystal_I2C lcd){
    uint8_t col = 0; // write to the left side (column 0)
    lcd.setCursor(col,0);
    write_year_and_weekday(lcd);
    lcd.setCursor(col, 1);
    write_month_and_day(lcd);
  }
  
private:
  const char weekdays[7] = {'U','M','T','W','R','F','S'};
  /** year in gregorian calendar */
  uint16_t year;
  /** month where 1 = January, 2 = Febuary, ... 12 = December */
  uint8_t month;
  /** day of the month, from 1 to 31 (although not all months go up to 31) */
  uint8_t day_of_month;
  
  /**writes the year and a single letter for weekday (with U=Sunday, R=Thursday)
     assumes cursor is already positioned where the text should be written, should write exactly 6 characters. */
  void write_year_and_weekday(LiquidCrystal_I2C lcd){
    // year will just be left aligned, is fine because we pretty much always expect it to be 4 characters.
    size_t chars_in_year = lcd.print(year, DEC);
    switch(chars_in_year){
      // these all fallthrough so we simply pad the year by however many spaces until we have 5 total characters written
    case 1:
      lcd.write(' ');
    case 2:
      lcd.write(' ');
    case 3:
      lcd.write(' ');
    case 4:
      lcd.write(' ');
    case 5:
      // if the year has 5 digits the weekday letter will be right up against the year
      break; 
    default:
      //WARN("Year stored as uint16_t took more than 5 characters or less than 1 character to write in decimal");
      return;
    }
    // write the single letter for the weekday
    lcd.write(weekdays[dTF.getDOW(day_of_month, month, year)]);
  }
  /** writes the month as Jan/Feb/Mar etc and then a space and the day with zero padding.
   assumes cursor is already positioned where the text should be written, should write exactly 6 characters. */
  void write_month_and_day(LiquidCrystal_I2C lcd){
    // we assume this will always take up exactly 3 characters
    size_t chars = lcd.print(dTF.getMNS(month));
    if(chars != 3){
      //WARN("month abbreviated did not take up 3 characters!");
    }
    lcd.write(' ');
    
    lcd.write('0' + (day_of_month/10u));
    lcd.write('0' + (day_of_month%10u));
  }
};
