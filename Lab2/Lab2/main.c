/*
 * Lab2.c
 *
 * Created: 1/28/2020 15:37:17
 * Author : leo85
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

/* Pin config
PD0-7 : Flip switches
PB0-1 : Digit0-1 select
PB2   : Segment A
PC0-5 : Segment B-G
PB5   : On-board LED
*/

ISR(TIMER1_COMPA_vect)
{
	cli();
	PORTB ^= (1<<PORTB0);
	
	sei();
}

int main(void)
{
	//Pin config
	DDRD = 0;
	DDRB = 0b00000111;
	DDRC = 0b00111111;
	DDRB |= (1<<PORTB5); //On-board LED for debug
	//Timer and interrupt config
	cli();
	TCCR1A = (0<<COM1A1)|(1<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10); //Timer1 on CTC
	TCCR1B = (0<<ICNC1)|(0<<ICES1)|(0<<WGM13)|(1<<WGM12)|(1<<CS12)|(0<<CS11)|(0<<CS10); //Timer1 scaled by 256
	TIMSK1 = (0<<ICIE1)|(0<<OCIE1B)|(1<<OCIE1A)|(0<<TOIE1); //Enable match interrupt
	OCR1A = 62500; //1 sec
	//OCR1A = 15625; //250 ms
	//OCR1A = 6250;  //100 ms
	//OCR1A = 1875; //30 ms
	PORTB = (1<<PORTB0);
	sei(); //Enable global interrupt
	
	
    while (1) 
    {
    }
}

