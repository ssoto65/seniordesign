/*
 * tcc_Tiny.c
 *
 * Created: 4/4/2019 5:18:43 PM
 *  Author: SamanthaSoto
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void tcc_init(void){
	/* enable overflow interrupt */
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;

	/* set Normal mode */
	TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_NORMAL_gc;

	/* disable event counting */
	TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);

	/* set the period */
	TCA0.SINGLE.PER = 0xFFFF;
	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm; /* start timer */
}