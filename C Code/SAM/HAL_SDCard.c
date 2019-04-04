/*******************************************************************************
 *
 *  HAL_SDCard.c - Driver for the SD Card slot
 *
 *  Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/***************************************************************************//**
 * @file       HAL_SDCard.c
 * @addtogroup HAL_SDCard
 * @{
 ******************************************************************************/
//#include "msp430.h"
#include "sam.h"
#include "HAL_SDCard.h"

// Pins from MSP430 connected to the SD Card
#define SPI_SIMO        BIT0  // 0x0001
#define SPI_SOMI        BIT1  // 0x0002
#define SPI_CLK         BIT2  // 0x0004
#define SD_CS           BIT5  // 0x0020

// Ports
#define SPI_SEL         P3SEL
#define SPI_DIR         P3DIR
#define SPI_OUT         P3OUT
#define SPI_REN         P3REN
#define SD_CS_SEL       P3SEL
#define SD_CS_OUT       P3OUT
#define SD_CS_DIR       P3DIR

/***************************************************************************//**
 * @brief   Initialize SD Card
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_init(void)
{
	//enable peripheral clock
	REG_PMC_PCER0 |= PMC_PCER0_PID21;
	
	//SS
	 REG_PIOA_PER |= PIO_PER_P16; //set PA31 as controllable by the PIO controller (disable peripheral)
	 REG_PIOA_OER |= PIO_OER_P16; //set PA31 as output
	 REG_PIOA_SODR |= PIO_PER_P16; //set output high on pin PA31 (CS0)
		
	//Disable pins from PIO
	//REG_PIOA_PDR |= PIO_PDR_P11 | PIO_PDR_P12 | PIO_PDR_P13 | PIO_PDR_P14;

		
	//Enable pins on Peripheral A
	REG_PIOA_ABCDSR &= ~(PIO_PDR_P11 | PIO_PDR_P12 | PIO_PDR_P13 | PIO_PDR_P14);
	//set spi master mode
	REG_SPI_MR |= SPI_MR_MSTR;
	//set fixed peripheral select(peripheral chosen in SP_MR.PCS instead of SPI_THR.PCS)
	REG_SPI_MR &= ~SPI_MR_PS;
	//REG_SPI_MR |= SPI_MR_PCS(0);
	//set polarity and clock phase
	//SPI_setMode(0);
	SPI_setMode(0);
	//set clock generator (1 = peripheral clock rate), otherwise a divisor
	//SCBR = fperipheral clock / SPCK Bit Rate
	//50 MHZ = 25, 25 MHZ = 12
	REG_SPI_CSR |= SPI_CSR_SCBR(12);
	//REG_SPI_CSR |= SPI_CSR_DLYBCT(4);
	//REG_SPI_CSR |= SPI_CSR_DLYBS(4);
	//chip select remains low after transfer
	//REG_SPI_CSR |= SPI_CSR_CSAAT;
	//give peripheral control of pins (Chip select pins are optional)
	REG_SPI_CSR |= SPI_CSR_BITS_8_BIT;
	REG_PIOA_PDR |= PIO_PDR_P11; //NPCS0
	//REG_PIOA_PDR |= PIO_PDR_P31; //NPCS1
	REG_PIOA_PDR |= PIO_PDR_P12; //MISO
	REG_PIOA_PDR |= PIO_PDR_P13; //MOSI
	REG_PIOA_PDR |= PIO_PDR_P14; //SPCK
	//enable SPI
	REG_SPI_CR = SPI_CR_SPIEN;
}

void SPI_setMode(uint8_t mode){	
    /*
    Mode		CPOL	NCPHA
    Mode0		0		1
    Mode1		0		0
    Mode2		1		1
    Mode3		1		0
    */
    if (mode == 0){
        REG_SPI_CSR &= ~SPI_CSR_CPOL;
        REG_SPI_CSR |= SPI_CSR_NCPHA;
    }
    else if (mode == 1){
        REG_SPI_CSR &= ~SPI_CSR_CPOL;
        REG_SPI_CSR &= ~SPI_CSR_NCPHA;
    }
    else if (mode == 2){
        REG_SPI_CSR |= SPI_CSR_CPOL;
        REG_SPI_CSR |= SPI_CSR_NCPHA;
    }
    else if (mode == 3){
        REG_SPI_CSR |= SPI_CSR_CPOL;
        REG_SPI_CSR &= ~SPI_CSR_NCPHA;
    }
}

/***************************************************************************//**
 * @brief   Enable fast SD Card SPI transfers. This function is typically
 *          called after the initial SD Card setup is done to maximize
 *          transfer speed.
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_fastMode(void)
{
    int i = 0; //nothing
}

/***************************************************************************//**
 * @brief   Read a frame of bytes via SPI
 * @param   pBuffer Place to store the received bytes
 * @param   size Indicator of how many bytes to receive
 * @return  None
 ******************************************************************************/

void SDCard_readFrame(uint8_t *pBuffer, uint16_t size)
{
    // Clock the actual data transfer and receive the bytes
    while (size--){
		//wait for transmit register to be empty
		while (!(REG_SPI_SR & SPI_SR_TDRE));
		//send data to transmit register
		REG_SPI_TDR |= 0xff;
		//wait for received data to be ready to be read
		while (!(REG_SPI_SR & SPI_SR_RDRF));
		//read received data
		*pBuffer++ = REG_SPI_RDR;
		
       }
}

/***************************************************************************//**
 * @brief   Send a frame of bytes via SPI
 * @param   pBuffer Place that holds the bytes to send
 * @param   size Indicator of how many bytes to send
 * @return  None
 ******************************************************************************/

void SDCard_sendFrame(uint8_t *pBuffer, uint16_t size)
{
    // Clock the actual data transfer and send the bytes. Note that we
    // intentionally not read out the receive buffer during frame transmission
    // in order to optimize transfer speed, however we need to take care of the
    // resulting overrun condition.
    while (size--){
        //wait for transmit register to be empty
        while (!(REG_SPI_SR & SPI_SR_TDRE));
        //send data to transmit register
		REG_SPI_TDR = *pBuffer++;                            // Write byte
    }
    //Wait for all RX and TX to be done
	//wait for received data to be ready to be read
    while (!(REG_SPI_SR & SPI_SR_RDRF));
	
	
    //dummy read
    uint16_t dummy = REG_SPI_RDR;                                           // Dummy read to empty RX buffer
                                                           // and clear any overrun conditions
}

/***************************************************************************//**
 * @brief   Set the SD Card's chip-select signal to high
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_setCSHigh(void)
{
    //SD_CS_OUT |= SD_CS;
	REG_PIOA_SODR |= PIO_PER_P16; //set output high on pin PA11 (CS0)
}

/***************************************************************************//**
 * @brief   Set the SD Card's chip-select signal to low
 * @param   None
 * @return  None
 ******************************************************************************/

void SDCard_setCSLow(void)
{
    //SD_CS_OUT &= ~SD_CS;
	REG_PIOA_CODR |= PIO_PER_P16; //set output low on pin PA11 (CS0)
}

/***************************************************************************//**
 * @}
 ******************************************************************************/
void delay(int n) {
	for (int ii= 0; ii<4*n;ii++){
		asm("nop");
		//volatile int test = test + 1;
	}
}
		
		