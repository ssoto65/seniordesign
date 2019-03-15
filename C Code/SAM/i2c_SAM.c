#include "sam.h"
#include "i2c_SAM.h"


void i2c_init(void){
    //enable i2c peripheral in PMC
    REG_PMC_PCER0 |= PMC_PCER0_PID19;
	
    //disable PIO control of pins to enable peripheral control
    REG_PIOA_PDR |= PIO_PDR_P3;
    REG_PIOA_PDR |= PIO_PDR_P4;
	
    //write direction - not really needed for setup
    REG_TWI0_MMR &= ~TWI_MMR_MREAD; //0=write, 1=read
	
    //enter slave address
    //REG_TWI0_MMR |= TWI_MMR_DADR(0b1101001);
    
    /*from twi.h
    #define TWI_MMR_DADR_Pos 16
    #define TWI_MMR_DADR_Msk (0x7fu << TWI_MMR_DADR_Pos)
    #define TWI_MMR_DADR(value) ((TWI_MMR_DADR_Msk & ((value) << TWI_MMR_DADR_Pos)))
    */
			
    //clockwave form 100khz - 196, 400khz - 46, when fperipheral is 20MHz
	//REG_TWI0_CWGR |= TWI_CWGR_CKDIV(0);
    REG_TWI0_CWGR |= TWI_CWGR_CLDIV(121); //3
    REG_TWI0_CWGR |= TWI_CWGR_CHDIV(121); //3 
    //formula tlow = ((CLDIV x 2^CKDIV)+4)xTper;
    //1/100,000 = ((CLDIV x 2^0)+4)x1/20,000,000;
	
	REG_TWI0_MMR |= TWI_MMR_IADRSZ_1_BYTE;
	
    //disable slave mode
    REG_TWI0_CR |= TWI_CR_SVDIS;
    //enable master mode
    REG_TWI0_CR |= TWI_CR_MSEN;	
}

void i2c_stop(void){
    REG_TWI0_CR |= TWI_CR_STOP;	
}

void i2c_start(uint8_t slave_address, uint8_t mread){ //read=1, write=0
    //set slave address
    REG_TWI0_MMR |= TWI_MMR_DADR(slave_address);
    //set read/write direction
    if (mread == write){ //write
        REG_TWI0_MMR &= ~TWI_MMR_MREAD;
    }
    else if (mread == read){ //read
        REG_TWI0_MMR |= TWI_MMR_MREAD;
    }
    //send start
    REG_TWI0_CR |= TWI_CR_START;
}

uint8_t i2c_readACK(void){	
    uint8_t receiveByte;
    //if the stop bit in the control register is not set, ATSAM4S will automatically ACK after reading TWIn_RHR register
    //RXRDY will be set when data arrives in TWIn_RHR register
    while(!(REG_TWI0_SR & TWI_SR_RXRDY));
    //reading data will clear RXRDY bit in the status register
    receiveByte = REG_TWI0_RHR;
    return receiveByte;
}

uint8_t i2c_readNACK(void){
    uint8_t receiveByte;
    //ATSAM4S requires stop bit to be set before data is set on the TWIn_RHR
    //when stop bit is set ATSAM4S will send a NACK instead of an ACK automatically
    i2c_stop();
    //When data arrives in the TWIn_RHR register RXRDY is set in the Status Register
    while(!(REG_TWI0_SR & TWI_SR_RXRDY));
    //reading the byte of data will set RXRDY to 0
    receiveByte = REG_TWI0_RHR;
    while(!(REG_TWI0_SR & TWI_SR_TXCOMP));
    return receiveByte;
}

void i2c_writeLastByte(uint8_t data){
    //write data or slave register to THR
    REG_TWI0_THR = data;
    //ATSAM4S requires stop bit to be set before data is written to TWIn_THR register
    i2c_stop();
    //wait for ack
    while(!(REG_TWI0_SR & TWI_SR_TXRDY));
    while(!(REG_TWI0_SR & TWI_SR_TXCOMP));	
}

void i2c_write(uint8_t data){
    //write data or slave register to THR
    REG_TWI0_THR = data;
    //wait for ack
    while(!(REG_TWI0_SR & TWI_SR_TXRDY));	
}

int who_am_I(void){
	
	//////////////////////////////////////////////
		//THIS CODE WORKS
		REG_TWI0_MMR |= TWI_MMR_IADRSZ_1_BYTE;
		REG_TWI0_IADR = 117;
		i2c_start(0x69,read);
		int test = i2c_readNACK();
		
		return test;
	///////////////////////////////////////////////
		/*
			if(test1 == 0x73){
				asm("nop");
			}
	///////////////////////////////////////////////
	*/
}

/*
int read_register(uint8_t register){
	
	REG_TWI0_IADR = register;
	i2c_start(0x69,read);
	
	int data = i2c_readNACK();
	
	return data;
}
*/
