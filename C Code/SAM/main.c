
#include "sam.h"
#include "sam4s.h"
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


#define BAM 0
#define BB 1
extern volatile uint8_t game;


int main(void)
{
	
	int *REG_SCB_ACTLR = 0xE000E008;
	*REG_SCB_ACTLR = 0x10;
	SystemInit();
	REG_WDT_MR |= WDT_MR_WDDIS;
	//clock_init();
	DAC_init(); //Pin 93/PB13 (DAC Output)
	interrupt_init(); //Pin 24/PA20 (LED) | Pin 22/PA23 (Int Button)
	i2c_init(); //Pin 66/PA3(SDA) | Pin 55/PA4(SCK)
	clk_out(); //init for EBI uP clk // Pin 52/PA6(uP clk)
	UART_Init(); //Pin 7/PB2(RX) | Pin 9/PB3(TX)
	ebi_init();
	timerInit();
	//NVIC_DisableIRQ(PIOA_IRQn);
	//SD Card // Pin 28/PA16(SS) | Pin 31/PA14(SPCK) | Pin 33/PA13(MOSI) | Pin 41/PA12(MISO) 
	__enable_irq();                       
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
	//clear_matrix();
	//bb_play();
	//bam_play();
	

	
	
	while (1)
	{
		
		if (game == BAM){
			bam_play();
			while(game == BAM);
		}
		else if (game == BB){
			bb_play();
			while (game == BB);
		}
		
	}
}


void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
	/* These are volatile to try and prevent the compiler/linker optimising them
	away as the variables never actually get used.  If the debugger won't show the
	values of the variables, make them global my moving their declaration outside
	of this function. */
	volatile uint32_t r0;
	volatile uint32_t r1;
	volatile uint32_t r2;
	volatile uint32_t r3;
	volatile uint32_t r12;
	volatile uint32_t lr; /* Link register. */
	volatile uint32_t pc; /* Program counter. */
	volatile uint32_t psr;/* Program status register. */

	r0 = pulFaultStackAddress[ 0 ];
	r1 = pulFaultStackAddress[ 1 ];
	r2 = pulFaultStackAddress[ 2 ];
	r3 = pulFaultStackAddress[ 3 ];

	r12 = pulFaultStackAddress[ 4 ];
	lr = pulFaultStackAddress[ 5 ];
	pc = pulFaultStackAddress[ 6 ];
	psr = pulFaultStackAddress[ 7 ];

	/* When the following line is hit, the variables contain the register values. */
	for( ;; );
}


/* The prototype shows it is a naked function - in effect this is just an
assembly function. */
void HardFault_Handler(void) __attribute__((naked));

/* The fault handler implementation calls a function called
prvGetRegistersFromStack(). */
void HardFault_Handler(void)
{
	__asm volatile
	(
	" tst lr, #4                                                \n"
	" ite eq                                                    \n"
	" mrseq r0, msp                                             \n"
	" mrsne r0, psp                                             \n"
	" ldr r1, [r0, #24]                                         \n"
	" ldr r2, handler2_address_const                            \n"
	" bx r2                                                     \n"
	" handler2_address_const: .word prvGetRegistersFromStack    \n"
	);
}
