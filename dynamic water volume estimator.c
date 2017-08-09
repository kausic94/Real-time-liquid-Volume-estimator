/*
 * kausic_lcd.c
 *
 * Created: 5/29/2013 11:38:28 AM
 *  Author: kausic
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "kau_lcd.h"
uint32_t counter;
void trigger(void);
void echo_recieve(void);
#define echo PINB6
#define transmit PINB1
#define selected 20
#define unselected 0
float range=0;
#define a_set 5
#define b_set 10
#define h_set 15
int prev_value1=500,prev_value2=500,container=0,parameters=0,pressed=0; 
int para_set_cyliner=0;
int pressed_b=0,pressed_h=0;
int choice=0,para_value,para_set_cube=0;
int a=0,b=0,h=0;
int cuboid_para=a_set;
int main(void)

{   initialize();
	_delay_ms(5);
clr_lcd(); 
 _delay_ms(5);
 lcd_write("dynamic volume");
 _delay_ms(100);
 moveto(1,4);
 _delay_ms(10);
lcd_write("estimator");
_delay_ms(2000);
 clr_lcd();
 moveto(0,0);
 lcd_write("choose container:");
DDRB=0x0F;
PORTB=0xF0;
TCCR1B|=1<<CS11;
sei();
TIMSK|=1<<TOIE1;

ADCSRA|=1<<ADEN;//enable
//ADMUX|=1<<MUX0|1<<MUX1;
//ADMUX|=1<<ADLAR; left adjus tedvalue will be in adc .. it is by default off.
ADMUX|=1<<REFS0;// internal referance voltage
ADCSRA|=1<<ADIE;//interrupt enable;
sei();//global interrupts
ADCSRA|=1<<ADPS0|1<<ADPS1|1<<ADPS2;
ADCSRA|=1<<ADSC;
 	while(1)
    { 
    }
}
ISR(TIMER1_OVF_vect)
{
	counter++;
}
void trigger()
{
	PORTB&=~1<<transmit;
	_delay_us(20);
	PORTB|=1<<transmit;
	_delay_us(12);
	PORTB&=~1<<transmit;
	_delay_us(20);
}

void echo_recieve()
{
	for(int i=0;i<5000;i++)
	{
		if(PINB&(1<<echo))
		{
			TCNT1=0;
			counter=0;
			while(PINB&(1<<echo))
			{
			}
			TCCR1B=0x00;
			moveto(0,10);
			range=(((counter*65536)+TCNT1)*34*0.001)/3.8;
			TCCR1B|=1<<CS11;
			sei();
			TIMSK|=1<<TOIE1;
			break;
		}
	}
}

ISR(ADC_vect)
{               
	switch(ADMUX)
	{
		case 0x40:
		if((prev_value1<1000)&&(ADC>1000))
		container++;
		if((prev_value1>50)&&(ADC<50))
		container--;
		prev_value1=ADC;
		ADMUX=0x43;
		break;
		case 0x43:
		para_value=ADC;
		ADMUX=0x40;
		break;
	}
	
	if(bit_is_clear(PINB,7))
		choice=selected;

if(choice==0)
{
switch(container%3)
{
case 0:
clear_row(1);
_delay_ms(5);
moveto(1,0);
_delay_ms(10);
lcd_write("--->CUBE");
_delay_ms(500);
break;

case 1:
clear_row(1);
_delay_ms(5);
moveto(1,0);
_delay_ms(10);
lcd_write("--->CUBOID");
_delay_ms(100);
break;

case 2:
clear_row(1);
_delay_ms(5);
moveto(1,0);
_delay_ms(10);
lcd_write("--->cylinder");
_delay_ms(100);
break;

}
}

if(choice==selected) // when selected get the parameters
{clr_lcd();
	_delay_ms(10);
	moveto(0,0);
	lcd_write("enter parameters");
	_delay_ms(100);
switch(container%3)
{
	
case 0:
clear_row(1);
_delay_ms(10);
moveto(1,0);
_delay_ms(10);
lcd_write("a=");
_delay_ms(100);
if(para_value>=1000)
{
a=(a+1)%100;
moveto(1,3);
_delay_ms(10);
disp_int(a);
_delay_ms(100);
}
if(para_value<=100)
{
	int a=(a-1)%100;
	moveto(1,3);
	_delay_ms(10);
	disp_int(a);
	_delay_ms(100);
}
moveto(1,3);
_delay_ms(10);
disp_int(a);
_delay_ms(100);
if(bit_is_clear(PINB,7))
para_set_cube=selected;
break;	



case 1:
clear_row(1);
_delay_ms(10);
moveto(1,0);
_delay_ms(10);
lcd_write("a=");
_delay_ms(10);
moveto(1,5);
_delay_ms(10);
lcd_write("b=");
_delay_ms(10);
moveto(1,9);
_delay_ms(10);
lcd_write("h=");
_delay_ms(10);
if(cuboid_para==a_set)
{
if(para_value>=1000)
{
	a=(a+1)%100;
	moveto(1,3);
	_delay_ms(10);
	disp_int(a);
	lcd_write(" ");
	_delay_ms(100);
}
if(para_value<=100)
{
	int a=(a-1)%100;
	moveto(1,3);
	_delay_ms(10);
	disp_int(a);                     // instead of displaying it over here display everything directly at the end.
	lcd_write(" ");
	_delay_ms(100);
}
moveto(1,3);
_delay_ms(10);
disp_int(a);
lcd_write(" ");
_delay_ms(100);

if(pressed==0)
{
if(bit_is_clear(PINB,7))
{cuboid_para=b_set;
	pressed=1;
}
else
pressed=0;	
}
}

if(cuboid_para==b_set)
{ 
	if(para_value>=1000)
	{
		b=(b+1)%100;
		moveto(1,7);
		_delay_ms(10);
		disp_int(b);
		lcd_write(" ");
		_delay_ms(100);
	}
	if(para_value<=100)
	{
		b=(b-1)%100;
		moveto(1,7);
		_delay_ms(10);
		disp_int(b);
	lcd_write(" ");
	_delay_ms(100);
	}
	moveto(1,7);
	_delay_ms(10);
	disp_int(b);
	lcd_write(" ");
	_delay_ms(100);
	
	if(pressed_b==0)
	{
		if(bit_is_clear(PINB,7)&&(b>0))
		{cuboid_para=h_set;
			pressed_b=1;
		}
		else
		pressed_b=0;
	}	
			
}



if(cuboid_para==h_set)
{
	if(para_value>=1000)
	{
		h=(h+1)%100;
		moveto(1,11);
		_delay_ms(10);
		disp_int(h);
		lcd_write(" ");
		_delay_ms(100);
		
	}
	if(para_value<=100)
	{
		h=(h-1)%100;
		moveto(1,11);
		_delay_ms(10);
		disp_int(h);
		lcd_write(" ");
		_delay_ms(100);
		
	}
	moveto(1,11);
	_delay_ms(10);
	disp_int(h);
	lcd_write(" ");
	_delay_ms(100);
	
		if(pressed_h==0)
		{
			if(bit_is_clear(PINB,7)&&(h>0))
			{cuboid_para=selected;
				pressed_h=1;
			}
			else
			pressed_h=0;
		}
	
}
moveto(1,3);
_delay_ms(10);	
disp_int(a);
lcd_write(" ");
_delay_ms(100);  

moveto(1,7);
_delay_ms(10);
disp_int(b);
lcd_write(" ");
_delay_ms(100);

moveto(1,11);
_delay_ms(10);
disp_int(h);
lcd_write(" ");
_delay_ms(100);
break;


case 2:
clear_row(1);
_delay_ms(10);
moveto(1,0);
_delay_ms(10);
lcd_write("r=");
_delay_ms(20);
moveto(1,5);
_delay_ms(10);
lcd_write("h=");
_delay_ms(20);
if(para_set_cyliner==0)
{
if(para_value>=1000)
{
	a=(a+1)%100;
	moveto(1,3);
	_delay_ms(10);
	disp_int(a);
	_delay_ms(200);
}
if(para_value<=100)
{
	int a=(a-1)%100;
	moveto(1,3);
	_delay_ms(10);
	disp_int(a);
	_delay_ms(200);
}
if(bit_is_clear(PINB,7))
para_set_cyliner=50;

}


if(para_set_cyliner==50)
{
	if(para_value>=1000)
	{
		h=(h+1)%100;
		moveto(1,7);
		_delay_ms(10);
		disp_int(h);
		_delay_ms(100);
	}
	if(para_value<=100)
	{
		int h=(h-1)%100;
		moveto(1,7);
		_delay_ms(10);
		disp_int(h);
		_delay_ms(100);
	}
	
	if(bit_is_clear(PINB,7)&&(h>0))
	para_set_cyliner=60;

}

moveto(1,3);
_delay_ms(10);
disp_int(a);
moveto(1,7);
_delay_ms(10);
disp_int(h);
_delay_ms(100);
break;

}

}
if((para_set_cube!=selected)&&(cuboid_para!=selected)&&(para_set_cyliner!=60))
ADCSRA|=1<<ADSC;
if(para_set_cube==selected)
{
	while(1)
	{
	clr_lcd();
	_delay_ms(10);
	moveto(0,0);
	_delay_ms(10);
	lcd_write("volume=");
	_delay_ms(10);
	moveto(0,8);
	trigger();
	echo_recieve();
	disp_float(((a-range)*a*a),2);
	_delay_ms(30);
	moveto(1,9);
	_delay_ms(5);
	disp_float(range,1);
	_delay_ms(400);
	
}}


if(cuboid_para==selected)
{
while(1)
{
	clr_lcd();
	_delay_ms(10);
	moveto(0,0);
	_delay_ms(10);
	lcd_write("volume=");
	_delay_ms(10);
	moveto(0,8);
	trigger();
	echo_recieve();
	disp_float(((h-range)*b*a),2);
	_delay_ms(30);
	moveto(1,9);
	_delay_ms(5);
   disp_float(range,1);
	_delay_ms(400);
}	
}


if(para_set_cyliner==60)
{
	while(1)
	{
		clr_lcd();
		_delay_ms(10);
		moveto(0,0);
		_delay_ms(10);
		lcd_write("volume=");
		_delay_ms(10);
		moveto(0,8);
		trigger();
		echo_recieve();
		disp_float(((h-range)*a*a*3.14),2);
		_delay_ms(30);
		moveto(1,9);
		_delay_ms(5);
		disp_float(range,1);
		_delay_ms(400);
	}
}
}

