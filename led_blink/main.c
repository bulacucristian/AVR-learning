#define F_CPU	16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define LED			PB5
#define LED_DDR		DDRB
#define LED_PORT	PORTB

#define DELAYTIME		1000

#define setBit(sfr, bit)		(_SFR_BYTE(sfr) |= (1 << bit))
#define clearBit(sfr, bit)		(_SFR_BYTE(sfr) &= ~(1 << bit))
#define toggleBit(sfr, bit)		(_SFR_BYTE(sfr) ^= (1 << bit))

int main(void)
{
	
	setBit(LED_DDR, LED);
	/*	
	while(1)
	{
		setBit(LED_PORT, LED);
		_delay_ms(DELAYTIME);
		
		clearBit(LED_PORT, LED);
		_delay_ms(DELAYTIME);
	}
	*/
	
	// or you can do it simply, just like that using the toggle function
	
	while(1)
	{
		_delay_ms(DELAYTIME);
		toggleBit(LED_PORT, LED);
				
	}
	
	return 0;	
}



