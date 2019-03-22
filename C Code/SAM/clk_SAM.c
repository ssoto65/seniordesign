/*
 * clk.c
 *
 * Created: 3/16/2019 11:28:53 AM
 *  Author: SamanthaSoto
 */ 

#include "sam.h"
#include "clk_SAM.h"



void clock_init(void){
	//enable external crystal
	REG_CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCXTEN;
	//wait for crystal to become ready
	while (!(REG_PMC_SR & PMC_SR_MOSCXTS));
	//select crystal for main clock
	REG_CKGR_MOR |= CKGR_MOR_KEY_PASSWD | CKGR_MOR_MOSCSEL;
	//master clock source selection - choose main clock
	REG_PMC_MCKR |= PMC_MCKR_CSS_MAIN_CLK;
	//wait until main clock ready
	while (!(REG_PMC_SR & PMC_SR_MCKRDY));
	//select processer prescaler (0 - no divisor)
	REG_PMC_MCKR |= PMC_MCKR_PRES_CLK_1;
	
	//select processer prescaler (div 4) 20mhz/4 = 5mhz
	//REG_PMC_MCKR |= PMC_MCKR_PRES_CLK_4;
	//wait until main clock ready
	while (!(REG_PMC_SR & PMC_SR_MCKRDY));
	
	REG_CKGR_PLLBR |= CKGR_PLLBR_PLLBCOUNT(60);
	
	//12MHz / 6 = 2MHz, 2MHz * (24+1) = 50MHz
	REG_CKGR_PLLBR |= CKGR_PLLBR_MULB(24);
	REG_CKGR_PLLBR |= CKGR_PLLBR_DIVB(6);

	//select PLLB as the master clock
	//master clock source selection - choose main clock
	REG_PMC_MCKR |= PMC_MCKR_CSS_PLLB_CLK;
}

