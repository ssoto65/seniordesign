/*
 * buttonTiny.c
 *
 * Created: 3/17/2019 1:40:37 PM
 *  Author: SamanthaSoto
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "button_Tiny.h"
#include "uart_Tiny.h"

#define buttonUP 1
#define buttonDOWN 2
#define buttonLEFT 3
#define buttonRIGHT 4
#define buttonA 5
#define buttonB 6
#define buttonDirstop 7

volatile uint8_t buttonPressed = 0;

void buttonINT(void){
	// PORTA
	PORTA.PIN7CTRL = PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN6CTRL = PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN5CTRL = PORT_ISC_RISING_gc;
	PORTA.PIN4CTRL = PORT_ISC_RISING_gc;
	
	PORTA.PIN2CTRL = PORT_ISC_RISING_gc;
	PORTA.PIN1CTRL = PORT_ISC_RISING_gc;
	
	CPUINT.CTRLA &= ~CPUINT_LVL0RR_bm;
	// Example on how to shift the interrupt priority.
	CPUINT.LVL0PRI = PORTA_PORT_vect_num;

	// Enable global interrupts
	sei();
}
ISR(PORTA_PORT_vect){
	uint8_t flag = PORTA_INTFLAGS;
	
	if((PORTA_IN & PIN7_bm) && (flag & PIN7_bm)){
		buttonPressed = buttonLEFT;
		uartTX(buttonPressed);
	}
	else if(!(PORTA_IN & PIN7_bm) && (flag & PIN7_bm)){
		buttonPressed = buttonDirstop;
		uartTX(buttonPressed);
	}
	else if((PORTA_IN & PIN6_bm) && (flag & PIN6_bm)){
		buttonPressed = buttonRIGHT;
		uartTX(buttonPressed);
	}
	
	else if(!(PORTA_IN & PIN6_bm) && (flag & PIN6_bm)){
		buttonPressed = buttonDirstop;
		uartTX(buttonPressed);
	}
	else if(PORTA_IN & PIN5_bm){
		buttonPressed = buttonDOWN;
		uartTX(buttonPressed);
	}
	else if(PORTA_IN & PIN4_bm){
		buttonPressed = buttonUP;
		uartTX(buttonPressed);
	}
	else if(PORTA_IN & PIN2_bm){
		buttonPressed = buttonA;
		uartTX(buttonPressed);
	}
	else if(PORTA_IN & PIN1_bm){
		buttonPressed = buttonB;
		uartTX(buttonPressed);
	}
	
	PORTA_INTFLAGS = 0xFF; //clear all
}