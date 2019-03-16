#include "sam.h"
#include "interrupt_SAM.h"

void interrupt_init(void){
	//enable clock for PIOA
	REG_PMC_PCER0 |= PMC_PCER0_PID11;
	
	//Setup Input and Output pins and an interrupt
	//set PA30 as controllable by the PIO controller (disable peripheral)
	REG_PIOA_PER |= PIO_PER_P20;
	//set PA30 as output for LED
	REG_PIOA_OER |= PIO_OER_P20;
	//set PA30 high
	REG_PIOA_SODR |= PIO_SODR_P20;
	
	//set PA23 as controllable by the PIO controller (disable peripheral)
	REG_PIOA_PER |= PIO_PER_P23;
	//output disable register (set as input for button)
	REG_PIOA_ODR |= PIO_ODR_P23;
	//disable pull down
	REG_PIOA_PPDDR |= PIO_PPDDR_P23;
	//enable internal pull up resistor on PA23
	REG_PIOA_PUER |= PIO_PUER_P23;
	//enable glitch filter on PA23 (button debounce)
	REG_PIOA_IFER |= PIO_IFER_P23;
	//Read ISR so that it clears any interrupt flags that might be there
	uint32_t temp = REG_PIOA_ISR;
	//enable input change interrupt on PA23
	REG_PIOA_IER |= PIO_IER_P23;
	//enable PIOA interrupts
	NVIC_EnableIRQ(PIOA_IRQn);
}

//when PIOA interrupt is triggered this block of code will run
void PIOA_Handler(void) {
	// reading PIOA_ISR will clear interrupt flags
	uint32_t status = REG_PIOA_ISR;
	
	if ((status & PIO_ISR_P23) >= 1){ //pin change interrupt on P23
		//if PA20 high then set low, if PA20 low then set high
		if ((REG_PIOA_PDSR & PIO_ODSR_P20)>=1){
			//set PA20 low
			REG_PIOA_CODR |= PIO_CODR_P20;
		}
		else{
			//set PA20 high
			REG_PIOA_SODR |= PIO_SODR_P20;
		}
	}
}