/*
 * ADC.c
 *
 * Created: 7/24/2018 4:01:47 PM
 * Author : cristian.bulacu
 */ 

#define F_CPU			16000000UL;

#include <avr/io.h>
#include <avr/interrupt.h>

void setup_ADC(void);
void start_conversion(void);
void setup_Timer0(void);
void start_Timer0(void);

double duty_cycle = 0;

uint8_t adc_value = 0;

int main(void)
{
	DDRD |= (1 << PORTD6); // Setting digital pin 6 as an output
	
	setup_Timer0();
	setup_ADC();
	
	sei();
	
	start_Timer0();	
	
    while (1) 
    {
		 
    }
	
	return 0;
}

void setup_ADC(void)
{
	ADMUX |= (1 << REFS0); // AVcc with external capacitor at AREF pin
	ADMUX |= (1 << MUX2) | (1 << MUX0); // Input Channel Selection as analog pin 5 on the Arduino board [ADMUX = 5]
	ADMUX |= (1 << ADLAR); // left-ajust the ADC conversion result
	
	ADCSRA |= (1 <<ADEN); // Enable the ADC
	ADCSRA |= (1 << ADIE); // Interrupt enable
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // Setting the prescaler to 128
	
	DIDR0 |= (1 << ADC5D); // Disable the digital input buffer
	
	start_conversion();
	
}

void start_conversion(void)
{
	ADCSRA |= (1 << ADSC);
}

void setup_Timer0(void)
{
	TCCR0A |= (1 << COM0A1) | (1 << WGM00) | (1 << WGM01); // fast PWM setup
	
	TIMSK0 |= (1 << TOIE0); // Timer/Counter0 Overflow Interrupt Enable
	
}

void start_Timer0(void)
{
	TCCR0B |= (1 << CS00); // prescaler set to 1 at the moment and also starts the timer
}

ISR(TIMER0_OVF_vect)
{
	OCR0A = duty_cycle; // setting the new compare value for the interrupt
}

ISR(ADC_vect)
{	
	duty_cycle = ADCH ; //value will be equal to the ADC value
	start_conversion(); // Restart the conversion
}

