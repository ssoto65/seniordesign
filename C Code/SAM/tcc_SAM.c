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
extern volatile uint8_t game;

void timerInit(){
	
	NVIC_SetPriority(TC0_IRQn,1);
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
	//REG_TC1_IER0 |= TC_IER_COVFS;
	
	REG_TC0_IER0 |= TC_IER_CPCS; //enable RC compare interrupt
	REG_TC0_CMR0 |= TC_CMR_CPCTRG; //compare resets counter and clock
	
	//50 MHz = 567
	REG_TC0_RC0 = 284; //1563/2;
	
	//enable tc clock
	REG_TC0_CCR0 |= TC_CCR_CLKEN;

	
	//PIO setup (not neccessary) because we won't use the pins
}

