
#include "sam.h"
#include "bb_SAM.h"
#include "clk_SAM.h"
#include "dac_SAM.h"
#include "HAL_SDCard.h"
#include "i2c_SAM.h"
#include "bam_SAM.h"
#include "interrupt_SAM.h"
#include "led_SAM.h"
#include "tcc_SAM.h"
#include "uart_SAM.h"

volatile uint8_t game;

int main(void)
{
	SystemInit();
	REG_WDT_MR |= WDT_MR_WDDIS;
	clock_init();
	DAC_init(); //Pin 93/PB13 (DAC Output)
	interrupt_init(); //Pin 24/PA20 (LED) | Pin 22/PA23 (Int Button)
	i2c_init(); //Pin 66/PA3(SDA) | Pin 55/PA4(SCK)
	clk_out(); //init for EBI uP clk // Pin 52/PA6(uP clk)
	//UART_Init(); //Pin 7/PB2(RX) | Pin 9/PB3(TX)
	ebi_init();
	timerInit();
	//NVIC_DisableIRQ(PIOA_IRQn);
	//NVIC_EnableIRQ(UART1_IRQn);
	//SD Card // Pin 28/PA16(SS) | Pin 31/PA14(SPCK) | Pin 33/PA13(MOSI) | Pin 41/PA12(MISO) 
	//__disable_irq();                       
	//__DMB();    
	
	//set PA24 as controllable by the PIO controller (disable peripheral)
	REG_PIOA_PER |= PIO_PER_P24;
	//output disable register (set as input for button)
	REG_PIOA_ODR |= PIO_ODR_P24;
	//disable pull down
	REG_PIOA_PPDDR |= PIO_PPDDR_P24;
	//enable internal pull up resistor on PA24
	REG_PIOA_PUER |= PIO_PUER_P24;
	//enable glitch filter on PA24 (button debounce)
	REG_PIOA_IFER |= PIO_IFER_P24;
	
	
	//bb_play();
	bam_play();
	
	while (1)
	{
		
	}
}
