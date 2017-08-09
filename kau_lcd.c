#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "kau_lcd.h"

void check_busy()
{
	_delay_ms(5);
}
void process()
{
	control|=1<<enable;
	_delay_us(10);
	control&=~(1<<enable);
}
void command_lcd(unsigned char cmd)
{
	void check_busy();
	control&=~(1<<rs|1<<rw);
	data = cmd;
	process();
}
void data_lcd(unsigned char msg)
{
	
	void check_busy();
	control&=~(1<<rw);
	control|=1<<rs;
	data =msg;
	process();
}
void lcd_write(char *str)
{
	while(*str!='\0')
	{
		data_lcd(*str);
		str++;
		_delay_ms(1);
	}
}
void disp_int(int x)
{
	char s[15];
	itoa(x,s,10);
	lcd_write(s);
}
void disp_float (float x,int numberofdigitsafterdecimalpoint)
{
	char s[15];
	dtostrf(x,1,numberofdigitsafterdecimalpoint,s);
	lcd_write(s);
}

void moveto(int r,int c)  // row and column indexing start from 0
{
	switch(r)
	{
		case 0:
		if(c<15)
		command_lcd(0x80+c);
		else
		lcd_write("sorry out of display");
		break;
		case 1:
		if(c<15)
		command_lcd(0x80+64+c);
		else
		lcd_write("sorry out of display");
		break;
		default:
		lcd_write("invalid choice of row");
	}
	_delay_ms(2);
}
void timed_write(int choice,int speed,char *str)
{int i;
	if(choice==0) // choice 0 is for seconds
	{
		while(*str!='\0')
		{
			data_lcd(*str);
			str++;
			for(i=0;i<speed*1000;i++)
			_delay_ms(1);
		}
	}
	else //choice rest is for milli seconds
	{
		while(*str!='\0')
		{
			data_lcd(*str);
			str++;
			for(i=0;i<speed;i++)
			_delay_ms(1);
		}
	}
}void cursor_on()
{
	command_lcd(0b00001110);
}
void cursor_off()
{
	command_lcd(0b00001100);
}
void cursor_blink_on()
{
	command_lcd(0b00001111);
}
void cursor_blink_off()
{
	command_lcd(0b00001110);
}

void lcd_to_fro(int row,char *str,int speed_in_ms,int how_many_times) // for better resolution keep the speed low. i.e.  keep the
{                                                                    // speed_in_ms very high.
	int i=0,j,k;
	if(how_many_times==0)
	{
		while(1)
		{
			i=0;
			while(i<(16-strlen(str)))
			{
				moveto(row,i);
				
				lcd_write(str);
				for(j=0;j<speed_in_ms;j++)
				_delay_ms(1);
				moveto(row,0);
				for(j=0;j<=i;j++)
				lcd_write(" ");
				i++;
			}
			clear_row(1);
			i=16-strlen(str);
			while(i>=0)
			{
				moveto(row,i);
				lcd_write(str);
				for(j=0;j<speed_in_ms;j++)
				_delay_ms(1);
				i--;
				moveto(row,i+strlen(str));
				for(j=0;j<16-(i+strlen(str));j++)
				lcd_write(" ");
			}
		}
	}
	else
	{
		while(k<how_many_times)
		{
			i=0;
			while(i<(16-strlen(str)))
			{
				
				moveto(row,i);
				lcd_write(str);
				for(j=0;j<speed_in_ms;j++)
				_delay_ms(1);
				moveto(row,0);
				for(j=0;j<=i;j++)
				lcd_write(" ");
				i++;
				_delay_ms(2);
			}
			clear_row(1);
			i=16-strlen(str);
			while(i>=0)
			{
				moveto(row,i);
				lcd_write(str);
				for(j=0;j<speed_in_ms;j++)
				_delay_ms(1);
				i--;
				moveto(row,i+strlen(str));
				for(j=0;j<16-(i+strlen(str));j++)
				lcd_write(" ");
				_delay_ms(2);
			}
			k++;
		}
		moveto(row,0);
		lcd_write(str);
	}
	_delay_ms(25);
}
void clear_row(int row)
{moveto(row,0);
	lcd_write("                ");
	moveto(row,0);
}
void center(char *str)
{int i;
	for(i=0;i<(16-strlen(str))/2;i++)
	lcd_write(" ");
	lcd_write(str);
}
void clr_lcd()
{
	command_lcd(0x01);
}
void bounce(int position,int speed,char *str)
{   while(1)
	{
		int i;
		moveto(0,position);
		lcd_write(str);
		for(i=0;i<speed;i++)
		_delay_ms(1);
		clear_row(0);
		moveto(1,position);
		lcd_write(str);
		for(i=0;i<speed;i++)
		_delay_ms(1);
		clear_row(1);
	}
}
void initialize()
{char *str;
	ddr_control|=1<<enable|1<<rw|1<<rs;
	ddr_data=0xFF;
	_delay_ms(20);
	command_lcd(0x01);
	_delay_ms(5);
	command_lcd(0x38);//ox3C
	_delay_ms(5);
	command_lcd(0x0C);
	_delay_ms(5);
	command_lcd(0x80);
	_delay_ms(5);
}