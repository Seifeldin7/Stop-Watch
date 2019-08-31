
#include <asf.h>
#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/common.h>
unsigned char hours=0;
unsigned char minitues=0;
unsigned char seconds=0;
unsigned char pressed = 0;

void Timer1_Init(void);
void INT0_Init(void);

void Timer1_Init(void)
{
	TCNT1 = 0;
	TCCR1B |= (1<<CS12) | (1<<CS10) | (1<<WGM12);
	OCR1A = 1000;
	SREG |= (1<<7);
	TIMSK |=  (1<<OCIE1A);
	
}
ISR(TIMER1_COMPA_vect)
{
	
	seconds++;
	if(seconds == 60){
		minitues++;
		seconds = 0;
	}
	if(minitues == 60){
		hours++;
		seconds = 0;
		minitues = 0;
	}
}
void INT0_Init(void)
{
	DDRD  &= (~(1<<PD2));               
	GICR  |= (1<<INT0);                 
	MCUCR |= (1<<ISC01);   
	SREG  |= (1<<7);                    
}
ISR(INT0_vect)
{
	if(pressed == 0){
		TCCR1A = 0;
		TCCR1B = 0;
		TIMSK = 0;
		seconds = 0;
		minitues = 0;
		hours = 0;
		SREG  &= ~(1<<7);
		pressed = 1;
	}
	else {
		Timer1_Init();
		pressed = 0;
	}
	
}
int main (void)
{
	DDRC  = 0x0F;
	DDRB  = 0x3F;
	PORTC = 0;
	PORTB = 0;
	Timer1_Init();
	INT0_Init();
	while(1)
	{
		
		PORTB = (1<<PB5);
		PORTC = (PORTC&0xF0) | ((seconds%10)&0x0F);
		_delay_ms(10);
		PORTB = (1<<PB4);
		PORTC = (PORTC&0xF0) | ((seconds/10)&0x0F);
		_delay_ms(10);
		if(minitues > 0){
		PORTB = (1<<PB3);
		PORTC = (PORTC&0xF0) | ((minitues%10)&0x0F);
		_delay_ms(10);
		PORTB = (1<<PB2);
		PORTC = (PORTC&0xF0) | ((minitues/10)&0x0F);
		_delay_ms(10);
		}
		if(hours >0){
		PORTB = (1<<PB1);
		PORTC = (PORTC&0xF0) | ((hours%10)&0x0F);
		_delay_ms(60);
		PORTB = (1<<PB0);
		PORTC = (PORTC&0xF0) | ((hours/10)&0x0F);
		_delay_ms(60);	
		}
		
		
	}
}
