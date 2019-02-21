/*
 * SAM_DAC.c
 *
 * Created: 2/20/19 8:09:43 PM
 *  Author: NPoindexter
 */ 

#include "sam.h"


void DAC_init(void){
	
	//Pin B13 (93)
	
	//enable dacc peripheral clock
	REG_PMC_PCER0 |= PMC_PCER0_PID30;
	//enable dacc interrupts
	//NVIC_EnableIRQ(DACC_IRQn);
	
	//Most settings we want are 0 already
	//tag mode on - use bits in conversion data register to select output channel
	//REG_DACC_MR |= DACC_MR_TAG | DACC_MR_ONE;
	//set half word transfer (16bit)
	//REG_DACC_MR &= ~DACC_MR_WORD;
	//run in free mode (no external trigger)
	//REG_DACC_MR &= ~DACC_MR_TRGEN;
	
	//enable channel 0
	//setting this bit also gives dacc control of the pin and disables the PIO on this pin
	REG_DACC_CHER |= DACC_CHER_CH0;
	
}

void DAC_write(uint16_t data, uint8_t channel){
	if (channel == 0){
		REG_DACC_MR |= DACC_MR_USER_SEL(0);
	}
	else if (channel == 1){
		REG_DACC_MR |= DACC_MR_USER_SEL(1);
	}
	//put data into convert data register
	REG_DACC_CDR = data;
	//after 25 dacc clock cycles the analog voltage will output on the channel pin
}