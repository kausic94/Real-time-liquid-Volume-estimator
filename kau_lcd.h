#ifndef kau_lcd
#define kau_lcd
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#define  control PORTC                 // defining macros so that they can be easily replaced
#define ddr_control DDRC
#define  data PORTD
#define ddr_data DDRD
#define  enable 0
#define  rs  1
#define  rw 2

void check_busy(void);
void process(void);
void command_lcd(unsigned char cmd);
void data_lcd(unsigned char msg);
void lcd_write(char *str);
void disp_int(int);
void disp_float(float,int);
void moveto(int,int);
void timed_write(int,int,char*);
void cursor_on();
void cursor_off();
void cursor_blink_on();
void cursor_blink_off();
void lcd_to_fro(int,char*,int,int);
void clear_row(int);
void center(char*);
void clr_lcd();
void bounce(int,int,char*);
void initalize();
#endif
