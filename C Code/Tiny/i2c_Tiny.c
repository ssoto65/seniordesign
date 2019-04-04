/*
 * Senior_ATtiny.c
 *
 * Created: 2/11/19 2:31:06 PM
 * Author : NPoindexter
 */ 

#include <avr/io.h>

#define read 1
#define write 0

void i2c_init(void){
	
	//PORTB.DIRSET |= PIN0_bm;
	
	CLKCTRL_MCLKCTRLA |= CLKCTRL_PEN_bm | CLKCTRL_PDIV0_bm;
	
	TWI0_CTRLA |= TWI_SDAHOLD_500NS_gc;
	
	TWI0_MBAUD = 7; //400000 = 10M/(10+2BAUD+10M*2.5n)
	
	TWI0_MCTRLB |= TWI_ACKACT_NACK_gc;
	
	TWI0_MCTRLA |=TWI_ENABLE_bm;
	
	TWI0_MSTATUS |= TWI_BUSSTATE_IDLE_gc;
	}
	

uint8_t i2c_read(uint8_t address){
	//////////////////////////////////////////////////
		uint8_t test;
		//THIS CODE WORKS//
	    TWI0_MADDR = (0x68 << 1) | write;
		while(!(TWI0_MSTATUS & TWI_CLKHOLD_bm));
		TWI0_MDATA = address;
		while(!(TWI0_MSTATUS & TWI_WIF_bm));
		TWI0_MCTRLB |= TWI_MCMD_STOP_gc;
		
	
		TWI0_MADDR = (0x68 << 1) | read;
		
		while(!(TWI0_MSTATUS & TWI_RIF_bm));
		TWI0_MCTRLB |= TWI_MCMD_STOP_gc;
		test = TWI0_MDATA;
		return test;
		/////////////////////////////////////////////////
}

