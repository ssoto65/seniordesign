
#include <avr/io.h>
#include "uart_Tiny.h"

void uartInit(void){
	PORTB_DIRCLR = PIN0_bm; //input
	//PORTB.OUTCLR = PIN0_bm;
	
	//1. Set the TxD pin value high, and optionally set the XCK pin low (OUT[n] in PORTx.OUT).
	PORTB.OUTSET = PIN2_bm;

	//2. Set the TxD and optionally the XCK pin as an output (DIR[n] in PORTx.DIR).
	PORTB.DIRSET = PIN2_bm;

	//3. Set the baud rate (in the USARTn.BAUD register) and frame format.
	USART0_BAUD = 115; // (64*3,300,000)/(16*115,200)

	//4. Set the mode of operation (enables XCK pin output in Synchronous mode).
	USART0_CTRLC = USART_CHSIZE_8BIT_gc;

	//5. Enable the transmitter or the receiver, depending on the usage.	USART0_CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}


uint8_t uartRX(void){
	if (USART0_STATUS & USART_RXCIF_bm){
		return USART0_RXDATAL;
	}
}

void uartTX(uint8_t data){
	if (USART0_STATUS & USART_DREIF_bm){
		USART0_TXDATAL = data;	
	}
}