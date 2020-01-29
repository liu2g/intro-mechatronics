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
PC5-0 : Segment B-G
PB5   : On-board LED
*/

volatile int seg_bin;
int bin_to_segs (int);

ISR(TIMER1_COMPA_vect)
{
	cli();
	if (PORTB0) seg_bin = PIND & 0b00001111; //Lower 4 bits
	else seg_bin = PIND >> 4 & 0b00001111; //Upper 4 bits
	PORTB ^= (1<<PORTB0); //Toggle PB0, PB1 auto toggle on interrupt
	sei();
}

int main(void)
{
	//Pin config
	DDRD = 0;
	DDRB = 0b00000111;
	DDRC = 0b00111111;
	//Timer and interrupt config
	cli();
	TCCR1A = (0<<COM1A1)|(1<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10); //Timer1 CTC by toggling PB1
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
		PORTC |= bin_to_segs(seg_bin) & 0b111111;
		PORTB |= bin_to_segs(seg_bin) >> 4 & 0b0000100;
		/*
		7segs: ABCDEFG
		PORTC: 6543210
		PORTB: XXXXABC
		*/
    }
}

int bin_to_segs (int bin_value) //Converts bin value from switches to 4 bit A-G segs
{
	switch (bin_value)
	{
		case 0x0:
			return 0b1111110;
			break;
		case 0x1:
			return 0b0110000;
			break;
		case 0x2:
			return 0b1101101;
			break;
		case 0x3:
			return 0b1111001;
			break;
		case 0x4:
			return 0b0110011;
			break;
		case 0x5:
			return 0b1011011;
			break;
		case 0x6:
			return 0b1011111;
			break;
		case 0x7:
			return 0b1110000;
			break;
		case 0x8:
			return 0b0000000;
			break;
		case 0x9:
			return 0b1110011;
			break;
		case 0xA:
			return 0b1110111;
			break;
		case 0xb:
			return 0b0011111;
			break;
		case 0xC:
			return 0b1001110;
			break;
		case 0xd:
			return 0b0111101;
			break;
		case 0xE:
			return 0b1001111;
			break;
		case 0xF:
			return 0b1000111;
			break;
		default:
			return 0b0000000;
				
	}
}