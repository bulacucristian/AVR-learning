/*
 * USART.c
 *
 * Created: 7/23/2018 1:05:52 PM
 * Author : cristian.bulacu
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "usart.h"

/* DEFINES */

#define UART_RECEIVE_INTERRUPT			USART_RX_vect
#define UART_TRANSMIT_INTERRUPT			USART_UDRE_vect
#define UART_STATUS						UCSR0A
#define UART_CONTROL					UCSR0B
#define UART_DATA						UDR0
#define UART_UDRIE						UDRIE0

#define UART_RX_BUFFER_MASK				(UART_RX_BUFFER_SIZE - 1)
#define UART_TX_BUFFER_MASK				(UART_TX_BUFFER_SIZE - 1)

#if(UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK)
#error "RX_BUFFER IS NOT A POWER OF 2!!!"
#endif

#if(UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK)
#error "TX_BUFFER IS NOT A POWER OF 2!!!"
#endif


/* GLOBAL VARIABLES */

static volatile unsigned char uart_tx_buffer[UART_TX_BUFFER_SIZE];
static volatile unsigned char uart_rx_buffer[UART_RX_BUFFER_SIZE];
static volatile unsigned char uart_tx_head;
static volatile unsigned char uart_tx_tail;
static volatile unsigned char uart_rx_head;
static volatile unsigned char uart_rx_tail;
static volatile unsigned char uart_last_received_error;


void initialize_uart(unsigned int baudrate)
{
	uart_tx_head = 0;
	uart_tx_tail = 0;
	uart_rx_head = 0;
	uart_rx_tail = 0;
	
	UBRR0H = (unsigned char)(baudrate>>8);
	UBRR0L = (unsigned char) baudrate;
	 UCSR0A &= ~(_BV(U2X0));
	
	UART_CONTROL = _BV(RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
	
	UCSR0C = (3 << UCSZ00);	
}

void uart_put_char(unsigned char data_char)
{
	unsigned char temporary_head;
	
	temporary_head = (uart_tx_head + 1) & UART_TX_BUFFER_MASK;
	
	while(temporary_head == uart_tx_tail)
	{
		;;
	}
	uart_tx_buffer[temporary_head] = data_char;
	uart_tx_head = temporary_head;
	
	UART_CONTROL |= _BV(UART_UDRIE);
}
void uart_put_string(const char *s )
{
	while (*s)
	uart_put_char(*s++);

}

int available_uart(void)
{
	return(UART_RX_BUFFER_MASK + uart_rx_head - uart_rx_tail) % UART_RX_BUFFER_MASK;
}

void flush_uart(void)
{
	uart_rx_head = uart_rx_tail;
}

unsigned int uart_get_char(void)
{
	unsigned char temporary_tail;
	unsigned char data_to_receive;
	
	if(uart_rx_head == uart_rx_tail)
	{
		return UART_NO_DATA;
	}
	
	temporary_tail = (uart_rx_tail + 1) & UART_RX_BUFFER_MASK;
	uart_rx_tail = temporary_tail;
	
	data_to_receive = uart_rx_buffer[temporary_tail];
	
	return (uart_last_received_error << 8) + data_to_receive;
}

ISR(UART_TRANSMIT_INTERRUPT)
{
	unsigned char temporary_tail;
	
	if(uart_tx_head != uart_tx_tail)
	{
		temporary_tail = (uart_tx_tail + 1) & UART_TX_BUFFER_MASK;
		uart_tx_tail = temporary_tail;
		
		UART_DATA = uart_tx_buffer[temporary_tail];
	}
	else
	{
		UART_CONTROL &= ~_BV(UART_UDRIE);
	}
}

ISR(UART_RECEIVE_INTERRUPT)
{
	
	unsigned char temporary_head;
	unsigned char data;
	unsigned char usr;
	unsigned char last_rx_error;
	
	usr = UART_STATUS;
	data = UART_DATA;
	
	last_rx_error = (usr & (_BV(FE0) | _BV(DOR0)));
	
	temporary_head = (uart_rx_head + 1) & UART_RX_BUFFER_MASK;
	
	if(temporary_head == uart_rx_tail)
	{
		last_rx_error = UART_BUFFER_OVERFLOW >> 8;
	} else{
		uart_rx_head = temporary_head;
		uart_rx_buffer[temporary_head] = data;
	}
	uart_last_received_error = last_rx_error;
}



