#include "sam.h"
#include "interrupt_SAM.h"

void interrupt_init(void){
	//enable clock for PIOA
	REG_PMC_PCER0 |= PMC_PCER0_PID11;
	
	
	//set PA23 as controllable by the PIO controller (disable peripheral)
	REG_PIOA_PER |= PIO_PER_P17 + PIO_PER_P18 + PIO_PER_P19 + PIO_PER_P22 + PIO_PER_P27 + PIO_PER_P28 + PIO_PER_P29;
	//output disable register (set as input for button)
	REG_PIOA_ODR |= PIO_PER_P17 + PIO_PER_P18 + PIO_PER_P19 + PIO_PER_P22 + PIO_PER_P27 + PIO_PER_P28 + PIO_PER_P29;
	//disable pull down
	REG_PIOA_PPDDR |= PIO_PER_P17 + PIO_PER_P18 + PIO_PER_P19 + PIO_PER_P22 + PIO_PER_P27 + PIO_PER_P28 + PIO_PER_P29;
	//enable internal pull up resistor on PA23
	REG_PIOA_PUER |= PIO_PER_P17 + PIO_PER_P18 + PIO_PER_P19 + PIO_PER_P22 + PIO_PER_P27 + PIO_PER_P28 + PIO_PER_P29;
	//enable glitch filter on PA23 (button debounce)
	REG_PIOA_IFER |= PIO_PER_P17 + PIO_PER_P18 + PIO_PER_P19 + PIO_PER_P22 + PIO_PER_P27 + PIO_PER_P28 + PIO_PER_P29;
	//Read ISR so that it clears any interrupt flags that might be there
	uint32_t temp = REG_PIOA_ISR;
	//enable input change interrupt on PA23
	REG_PIOA_IER |= PIO_IER_P17 + PIO_IER_P19 + PIO_IER_P27 + PIO_IER_P28;
	
	REG_PIOA_AIMER |= PIO_AIMER_P17 + PIO_AIMER_P19 + PIO_AIMER_P27 + PIO_AIMER_P28;
	
	REG_PIOA_ESR |= PIO_IER_P17 + PIO_IER_P19 + PIO_IER_P27 + PIO_IER_P28;
	
	REG_PIOA_FELLSR |= PIO_IER_P17 + PIO_IER_P19 + PIO_IER_P27 + PIO_IER_P28;
	
	
	NVIC_SetPriority(PIOA_IRQn,13);
	
	
	//enable PIOA interrupts
	NVIC_EnableIRQ(PIOA_IRQn);
}

//when PIOA interrupt is triggered this block of code will run