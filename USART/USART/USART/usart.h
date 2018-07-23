/*
 * usart.h
 *
 * Created: 7/23/2018 1:07:09 PM
 *  Author: cristian.bulacu
 */ 


#ifndef USART_H_
#define USART_H_

#define UART_BAUD_SELECT(desired_baud_rate, cpu_freq)		((cpu_freq)/((desired_baud_rate)*16l) - 1)

#ifndef UART_RX_BUFFER_SIZE
#define UART_RX_BUFFER_SIZE		64
#endif

#ifndef UART_TX_BUFFER_SIZE
#define UART_TX_BUFFER_SIZE		64
#endif

#define UART_BUFFER_OVERFLOW  0x0200
#define UART_NO_DATA          0x0100 

/* TEST IF THE SIZE OF CIRCULAR BUFFERS FITS INTO SRAM */
#if((UART_RX_BUFFER_SIZE + UART_TX_BUFFER_SIZE) >= (RAMEND - 0X60))
#error "SUM 0F (UART_RX_BUFFER_SIZE + UART_TX_BUFFER_SIZE) DOES NOT FIT THE SIZE OF SRAM!!!"
#endif

/* FUNCTIONS PROTOTIPES */

extern void initialize_uart(unsigned int baudrate);
extern void flush_uart(void);
extern unsigned int uart_get_char(void);
extern void uart_put_char(unsigned char data_char);
extern void uart_put_string(const char *s);
extern int available_uart(void);

#endif /* USART_H_ */