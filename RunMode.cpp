#include "commonFuncs.h"

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

#ifdef USE_TIMING_FOR_INACCURATE_RESONATOR
/**
 * returns the number of milliseconds until the next clock update.
 * See timing_tests.md for some details about how this algorithm works,
 * it is largely based on emperically measured timings from the specific board I was working with
 */
unsigned int time_to_next_tick(){
  static unsigned int cycle = 0;

  /* Implementing 189/349 as an approximation for 346 + 16462310/30398671 */
  cycle += 1;
  if(cycle >= 349){
    cycle = 0;
  }
  //v = [x for x in range(349) if x&1 == 0 or x&0x1F==0x01 or x&0x7F == 0x11]; print(len(v))  #189
  if ((cycle & 1)==0 || (cycle&0x1F)==0x01 or (cycle&0x7F)==0x11){
    return 347;
  } else {
    return 346;
  }
}
#else
/**
 * returns the number of milliseconds until the next clock update.
 * with a perfect internal clock each tick is 25/72 seconds which is (347 + 2/9) milliseconds
 * as such calling this function 9 times it will return 348 twice and 347 the other 7 times.
 */
unsigned int time_to_next_tick(){
  static byte cycle = 0;
  cycle += 1;
  if (cycle >= 9){
    cycle = 0;
  }
  if(cycle == 3 || cycle == 7){
    return 348;
  } else {
    return 347;
  }
}
#endif


/** performs running mode ticking up the clock
  returns the selector dial value when it indicates we have left running mode */
char do_running(unsigned long *n_global){
  unsigned long n = *n_global;
  // re-measure the current time after we leave set-mode and tick the clock starting from this
  unsigned long timekeep = millis();
  char selector_mode; // as long as selector dial reads -1 we stay in running mode
  while((selector_mode = read_selector_dial()) == -1){
    unsigned long new_n = increment_doz(n);
    write_number(new_n);

    n = new_n;
    
    timekeep += time_to_next_tick();
    long time_to_wait = timekeep - millis();
    if(time_to_wait < 0){
      lcd.setCursor(0,0);
      lcd.print("Error!");
      delay(2000);
      return -2;
    }
    delay(time_to_wait);
  }
  *n_global = n;
  return selector_mode;
}
