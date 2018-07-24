/*
 * pwm_led.c
 *
 * Created: 7/24/2018 9:08:06 AM
 * Author : cristian.bulacu
 */ 

#define F_CPU			16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

double duty_cycle = 100;
uint8_t state = 0;

int main(void)
{
	DDRD |= (1 << PORTD6); //setting PORTD6 an an output
	
	TCCR0A |= (1 << COM0A1); // clear OC0A on Compare Match at BOTTOM (non-inverting mode)
	TCCR0A |= (1 << WGM00) | (1 << WGM01); // fast PWM setup
	
	TIMSK0 |= (1 << TOIE0); // Timer/Counter0 Overflow Interrupt Enable
	
	OCR0A = (duty_cycle/100)*255; // setting the actual compare value for the interrupt
	
	sei(); // here I set the external interrupts
	 
	TCCR0B |= (1 << CS00); // prescaler set to 1 at the moment and also starts the timer
	
	
    while (1) 
    {
		cli();
		_delay_ms(80);
		sei();
		if(state == 0)
		duty_cycle -=10;
		if(duty_cycle == 0)
		{
			state = 1;
		}
		
		if(state == 1)
		{
			cli();
			_delay_ms(80);
			sei();
			duty_cycle +=10;
			if(duty_cycle >100)
			{
				duty_cycle = 100;
				state = 0;
			}
		}
		
	}
		
	return 0;
}

ISR(TIMER0_OVF_vect)
{	
	OCR0A = (duty_cycle/100)*255; // setting the new compare value for the interrupt	
}

