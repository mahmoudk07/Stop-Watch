#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//************ Declaration of Time Variables ************//
unsigned char Seconds_1 = 4 , Seconds_2 = 4 ;
unsigned char Minutes_1 = 9 , Minutes_2 = 5;
unsigned char Hours_1 = 3 , Hours_2 = 2;
//******************************************************//
// ISR OF Timer1 //
ISR(TIMER1_COMPA_vect){
	if(Seconds_1 == 9)
	{
		if(Seconds_2 == 5 && Seconds_1 == 9)
		{
			if(Minutes_1 == 9 && Seconds_2 == 5 && Seconds_1 == 9){

				if(Minutes_2 == 5 && Minutes_1 == 9 && Seconds_2 == 5 && Seconds_1 == 9)
				{

					if(Hours_1 == 3 && Minutes_2 == 5 && Minutes_1 == 9 && Seconds_2 == 5 && Seconds_1 == 9)
					{
						if(Hours_2 == 2){
							Hours_2 = 0;
							Hours_1 = 0 ;
						}
						else
						{
							Hours_2 ++;
						}
					}
					else
					{
						Hours_1 ++ ;
					}
					Minutes_2 = 0;
				}
				else
				{
					Minutes_2 ++;
				}
				Minutes_1 = 0;
			}
			else
			{
				Minutes_1 ++;
			}
			Seconds_2 = 0;
		}
		else
		{
			Seconds_2 ++;
		}
		Seconds_1 = 0;
	}
	else
	{
		Seconds_1 ++;
	}
}
/////////////////////////////////////////////////////////////////////
// ISR OF INT0 //
ISR(INT0_vect){
	Seconds_1 = 0;
	Seconds_2 = 0;
	Minutes_1 = 0;
	Minutes_2 = 0;
	Hours_1 = 0;
	Hours_2 = 0;
}
/////////////////////////////////////////////////////////////////////
// ISR OF INT1
ISR(INT1_vect){
	TCCR1A = 0;
	TCCR1B = 0;
}
/////////////////////////////////////////////////////////////////////
// ISR OF INT2
ISR(INT2_vect){
	TCCR1A = (1 << FOC1A); // For Non-PWM Mode This Bit Should Be set by 1
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12); // Set Timer Compare Mode and Timer Pre-scaler 1024
}
/////////////////////////////////////////////////////////////////////
// Timer1 Initialization //
void Timer1_Configuration(void){
	TCCR1A = (1 << FOC1A); // For Non-PWM Mode This Bit Should Be set by 1
	TCCR1B = (1 << WGM12) | (1 << CS10) | (1 << CS12); // Set Timer Compare Mode and Timer Pre-scaler 1024
	TCNT1 = 0; // Initialize Timer with 0
	OCR1A = 1000; // SET Compare Value To Timer
	TIMSK = (1 << OCIE1A); // Module Interrupt Enable
	SREG |= (1 << 7); // Enable I-bit
}
///////////////////////////////////////////////////////////////////////
// INT0 Initialization //
void INT0_Configuration(void){
	MCUCR |= (1 << ISC01); // Interrupt Will Occurs with Falling Edge
	GICR |= (1 << INT0); // Module Interrupt Enable
	SREG |= (1 << 7); // Enable I-bit
}
/////////////////////////////////////////////////////////////////////
// INT1 Initialization //
void INT1_Configuration(void){
	MCUCR |= (1 << ISC10) | (1 << ISC11); // Interrupt Will Occurs With Rising Edge
	GICR |= (1 << INT1); // Module Interrupt Enable
	SREG |= (1 << 7); // Enable I-bit
}
/////////////////////////////////////////////////////////////////////
// INT2 Initialization //
void INT2_Configuration(void){
	GICR |= (1 << INT2) ; // Module Interrupt Enable
	SREG |= (1 << 7); // Enable I-bit
}
/////////////////////////////////////////////////////////////////////
int main(void){
	INT0_Configuration();
	INT1_Configuration();
	INT2_Configuration();
	Timer1_Configuration();
	DDRC |= 0x0F; // PC0 to PC3 Are OUTPUT PINS
	PORTC &= 0xF0; // Initialize All 7-segment with 0
	DDRA |= 0x3F; // PA0 to PA5 Are OUTPUT PINS
	DDRD &= 0xF3; // PD2 AND PD3 Are INPUT PINS
	PORTD |= (1 << PD2); // Enable Internal PULL-UP Resistor
	DDRB &= ~(1 << PB2); // PB3 Is Input PIN
	PORTB |= (1 << PB2); // Enable Internal PULL-UP Resistor

	while(1){
		// Refreshing Part1 of Seconds
		PORTC = (PORTC & 0xF0) | (Seconds_1 & 0x0F);
		PORTA |= (1 << PA0);
		_delay_ms(2);
		PORTA = 0x00;

		// Refreshing Part2 of Seconds
		PORTC = (PORTC & 0xF0) | (Seconds_2 & 0x0F);
		PORTA |= (1 << PA1);
		_delay_ms(2);
		PORTA = 0x00;

		// Refreshing Part1 of Minutes
		PORTC = (PORTC & 0xF0) | (Minutes_1 & 0x0F);
		PORTA |= (1 << PA2);
		_delay_ms(2);
		PORTA = 0x00;

		// Refreshing Part2 of Minutes
		PORTC = (PORTC & 0xF0) | (Minutes_2 & 0x0F);
		PORTA |= (1 << PA3);
		_delay_ms(2);
		PORTA = 0x00;

		// Refreshing Part1 of Hours
		PORTC = (PORTC & 0xF0) | (Hours_1 & 0x0F);
		PORTA |= (1 << PA4);
		_delay_ms(2);
		PORTA = 0x00;

		// Refreshing Part2 of Hours
		PORTC = (PORTC & 0xF0) | (Hours_2 & 0x0F);
		PORTA |= (1 << PA5);
		_delay_ms(2);
		PORTA = 0x00;
	}
}
