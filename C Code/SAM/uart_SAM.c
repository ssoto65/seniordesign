#include "sam.h"
#include "uart_SAM.h"

void UART_Init(){
	//configure PIO controller A  - disable means enable peripheral on pins
	//disable PIOA control of PA9 and enable peripheral on pin
	REG_PIOA_PDR |= PIO_PDR_P9;
	//disable PIOA control of PA9 and enable peripheral on pin
	REG_PIOA_PDR |= PIO_PDR_P10;
	REG_PIOA_ABCDSR &=  ~(PIO_ABCDSR_P9);
	REG_PIOA_ABCDSR &=  ~(PIO_ABCDSR_P10);
	
	//configure PMC UART Clock
	//enable UART0 clock
	REG_PMC_PCER0 |= PMC_PCER0_PID8;
	
	//configure buad rate
	REG_UART0_BRGR |= 26; // = 4,000,000/(16x9600)
	
	//parity
	REG_UART0_MR |= UART_MR_PAR_NO;
	
	//mode
	//normal mode default
	
	//enable transmit/receive
	REG_UART0_CR |= UART_CR_TXEN;
	REG_UART0_CR |= UART_CR_RXEN;
	
	//enable interrupt on receive
	REG_UART0_IER |= UART_IER_RXRDY;
}

void transmitByte(uint8_t data){
	//wait for ready
	while (!(REG_UART0_SR & UART_SR_TXRDY));
	while (!(REG_UART0_SR & UART_SR_TXEMPTY));
	REG_UART0_THR |= data;
}

void printString(const char myString[]) {
	uint8_t i = 0;
	while (myString[i]) {
		transmitByte(myString[i]);
		i++;
	}
}

//Example code
/* int main(void)
{
	// Initialize the SAM system 
	SystemInit();
	//clock_init();
	UART_Init();
	NVIC_EnableIRQ(UART0_IRQn);
	
	REG_PIOA_PER |= PIO_PER_P11; //pio enable PA11
	REG_PIOA_OER |= PIO_PER_P11; //set PA11 as output
	
	while (1)
	{
		//do nothing while waiting for interrupts to trigger
	}
} 

/////////////////////////////////////////INTERRUPT HANDLER///////////////////////////////////////////
void UART0_Handler(void) {
	// when we receive a byte, transmit that byte back
	uint32_t status = REG_UART0_SR;
	if ((status & UART_SR_RXRDY)){
		//read receive holding register
		uint8_t readByte = REG_UART0_RHR;
		//transmit that byte back
		if ((status & UART_SR_RXRDY)){
			//read receive holding register
			uint8_t readByte = REG_UART0_RHR;
			//transmit that byte back
			transmitByte(readByte);
		}
	}
}
*/