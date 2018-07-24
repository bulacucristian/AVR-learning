/*
 * timers.c
 *
 * Created: 7/24/2018 10:41:37 AM
 * Author : cristian.bulacu
 */ 

#define F_CPU				16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED			PD7
#define LED_DDR		DDRD
#define LED_PORT	PORTD

#define setBit(sfr, bit)		(_SFR_BYTE(sfr) |= (1 << bit))
#define clearBit(sfr, bit)		(_SFR_BYTE(sfr) &= ~(1 << bit))
#define toggleBit(sfr, bit)		(_SFR_BYTE(sfr) ^= (1 << bit))

volatile uint8_t isr_counter = 0;
volatile uint8_t isr_flag = 0;

int main(void)
{
	setBit(LED_DDR, LED);	
	
	TCCR0A |= (1 << WGM01); // set CTC bit
	
	OCR0A = 156; // total timer ticks
	
	TIMSK0 |= (1 << OCIE0A); // Timer/Counter0 Output Compare Match A Interrupt Enable
	
	TCCR0B |= (1 << CS02) | (1 << CS00); //set presclaer to 1024 and start the timer
	
	sei(); // set external interrupt	
   
    while (1) 
    {
		if(isr_flag == 1)
		{
			isr_flag = 0;
			toggleBit(LED_PORT, LED);
		}
	}
		
	return 0;
}

ISR(TIMER0_COMPA_vect)
{
	isr_counter++;	
	
	if(isr_counter > 100)
	{
		isr_counter = 0;
		isr_flag = 1;		
	}	
}
	

