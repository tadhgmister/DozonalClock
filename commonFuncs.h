#ifndef COMMON_FUNCS_H
#define COMMON_FUNCS_H
char read_selector_dial();
void erase_digit(byte dig);
void write_glyph_raw(byte idx, char n);
void write_number(unsigned long n);
void write_digit_direct(byte dig, char value);
void update_digit(byte digit, unsigned long n);
#endif
