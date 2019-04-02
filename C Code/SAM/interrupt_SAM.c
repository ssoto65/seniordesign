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
	REG_PIOA_PUER |= PIO_PER_P17 + PIO_PER_P18 + PIO_PER_P19 + PIO_PER_P22 + PIO_PER_P27 + PIO_PER_P28;
	//enable glitch filter on PA23 (button debounce)
	REG_PIOA_IFER |= PIO_PER_P17 + PIO_PER_P18 + PIO_PER_P19 + PIO_PER_P22 + PIO_PER_P27 + PIO_PER_P28;
	//Read ISR so that it clears any interrupt flags that might be there
	uint32_t temp = REG_PIOA_ISR;
	//enable input change interrupt on PA23
	REG_PIOA_IER |= PIO_IER_P17 + PIO_IER_P18 + PIO_IER_P19 + PIO_IER_P22 + PIO_IER_P27 + PIO_IER_P28;
	
	REG_PIOA_AIMER |= PIO_AIMER_P17 + PIO_AIMER_P18 + PIO_AIMER_P19 + PIO_AIMER_P22 + PIO_AIMER_P27 + PIO_AIMER_P28;
	
	REG_PIOA_ESR |= PIO_ESR_P17 + PIO_ESR_P18 + PIO_ESR_P19 + PIO_ESR_P22 + PIO_ESR_P27 + PIO_ESR_P28;
	
	REG_PIOA_FELLSR |= PIO_FELLSR_P17 + PIO_FELLSR_P18 + PIO_FELLSR_P19 + PIO_FELLSR_P22 + PIO_FELLSR_P27 + PIO_FELLSR_P28;
	
	
	NVIC_SetPriority(PIOA_IRQn,13);
	
	
	//enable PIOA interrupts
	NVIC_EnableIRQ(PIOA_IRQn);
}

//when PIOA interrupt is triggered this block of code will run