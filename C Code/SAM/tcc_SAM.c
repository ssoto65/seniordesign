/*
 * tcc.c
 *
 * Created: 3/16/2019 11:19:26 AM
 *  Author: SamanthaSoto
 */ 

#include "sam.h"
#include "tcc_SAM.h"
#include "bb_SAM.h"
#include "bam_SAM.h"

#define BAM 0
#define BB 1
#define bb_refresh 25
#define bam_refresh 25
volatile uint32_t counter = 0;
extern volatile uint8_t game;

void timerInit(){
	
	NVIC_SetPriority(TC0_IRQn,7);
	//Setup for TC0 - ID 23, TIOA0 - PA0 peripheral B
	//enable interrupts in NVIC for TC0
	NVIC_EnableIRQ(TC0_IRQn);
	
	//PMC setup
	//enable peripheral clock for timer counter channel0
	REG_PMC_PCER0 |= PMC_PCER0_PID23;
	
	//Interrupt Setup
	//mainclock div 2
	REG_TC0_CMR0 |= TC_CMR_TCCLKS_TIMER_CLOCK1;
	//enable couter overflow interrupt
	REG_TC0_IER0 |= TC_IER_COVFS;
	//enable tc clock
	REG_TC0_CCR0 |= TC_CCR_CLKEN;

	
	//PIO setup (not neccessary) because we won't use the pins
}



void TC0_Handler(void){
	//read status register - this clears interrupt flags
	uint32_t status = REG_TC0_SR0;
	if ((status & TC_SR_COVFS)>=1){
		//increment counter
		counter+=1;
	}
	
	if (counter>bb_refresh){
		//reset counter
		counter=0;
		if (game == BAM){
			mazerefresh();	
		}
		else if (game == BB){
			ballRefresh();
			paddleRefresh();
		}
	}
}