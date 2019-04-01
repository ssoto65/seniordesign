
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

#define buttonUP 1
#define buttonDOWN 2
#define buttonLEFT 3
#define buttonRIGHT 4
#define buttonA 5
#define buttonB 6

#define paddleLEFT 1
#define paddleRIGHT 2

extern volatile  uint8_t upDown;
extern volatile uint8_t AorB;
extern volatile uint8_t bam_WinOrLose;
extern volatile uint8_t bb_WinOrLose;


volatile int sine[256] = {0x800,0x832,0x864,0x896,0x8c8,0x8fa,0x92c,0x95e,
	0x98f,0x9c0,0x9f1,0xa22,0xa52,0xa82,0xab1,0xae0,
	0xb0f,0xb3d,0xb6b,0xb98,0xbc5,0xbf1,0xc1c,0xc47,
	0xc71,0xc9a,0xcc3,0xceb,0xd12,0xd39,0xd5f,0xd83,
	0xda7,0xdca,0xded,0xe0e,0xe2e,0xe4e,0xe6c,0xe8a,
	0xea6,0xec1,0xedc,0xef5,0xf0d,0xf24,0xf3a,0xf4f,
	0xf63,0xf76,0xf87,0xf98,0xfa7,0xfb5,0xfc2,0xfcd,
	0xfd8,0xfe1,0xfe9,0xff0,0xff5,0xff9,0xffd,0xffe,
	0xfff,0xffe,0xffd,0xff9,0xff5,0xff0,0xfe9,0xfe1,
	0xfd8,0xfcd,0xfc2,0xfb5,0xfa7,0xf98,0xf87,0xf76,
	0xf63,0xf4f,0xf3a,0xf24,0xf0d,0xef5,0xedc,0xec1,
	0xea6,0xe8a,0xe6c,0xe4e,0xe2e,0xe0e,0xded,0xdca,
	0xda7,0xd83,0xd5f,0xd39,0xd12,0xceb,0xcc3,0xc9a,
	0xc71,0xc47,0xc1c,0xbf1,0xbc5,0xb98,0xb6b,0xb3d,
	0xb0f,0xae0,0xab1,0xa82,0xa52,0xa22,0x9f1,0x9c0,
	0x98f,0x95e,0x92c,0x8fa,0x8c8,0x896,0x864,0x832,
	0x800,0x7cd,0x79b,0x769,0x737,0x705,0x6d3,0x6a1,
	0x670,0x63f,0x60e,0x5dd,0x5ad,0x57d,0x54e,0x51f,
	0x4f0,0x4c2,0x494,0x467,0x43a,0x40e,0x3e3,0x3b8,
	0x38e,0x365,0x33c,0x314,0x2ed,0x2c6,0x2a0,0x27c,
	0x258,0x235,0x212,0x1f1,0x1d1,0x1b1,0x193,0x175,
	0x159,0x13e,0x123,0x10a,0xf2,0xdb,0xc5,0xb0,
	0x9c,0x89,0x78,0x67,0x58,0x4a,0x3d,0x32,
	0x27,0x1e,0x16,0xf,0xa,0x6,0x2,0x1,
	0x0,0x1,0x2,0x6,0xa,0xf,0x16,0x1e,
	0x27,0x32,0x3d,0x4a,0x58,0x67,0x78,0x89,
	0x9c,0xb0,0xc5,0xdb,0xf2,0x10a,0x123,0x13e,
	0x159,0x175,0x193,0x1b1,0x1d1,0x1f1,0x212,0x235,
	0x258,0x27c,0x2a0,0x2c6,0x2ed,0x314,0x33c,0x365,
	0x38e,0x3b8,0x3e3,0x40e,0x43a,0x467,0x494,0x4c2,
	0x4f0,0x51f,0x54e,0x57d,0x5ad,0x5dd,0x60e,0x63f,
	0x670,0x6a1,0x6d3,0x705,0x737,0x769,0x79b,0x7cd};


int main(void)
{

	SystemInit();
	REG_WDT_MR |= WDT_MR_WDDIS;
	clock_init();
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
	
	while(1){
		while(AorB != buttonA){
			if (game == BB){
				display_menu_bb();
			}else{
				display_menu_bam();
			}
			
			if(upDown == buttonUP){
				game = BB;
			}
			else if(upDown == buttonDOWN){
				game = BAM;
			}
		}	
			
		if (game == BAM){
			bam_play();
			while(bam_WinOrLose == 37);
			REG_TC0_CCR0 |= TC_CCR_CLKDIS;
			if (bam_WinOrLose == 0){
				display_lose();
				while(AorB != buttonB);
			}
			else{
				display_win();
				while(AorB != buttonB);
			}
		}
		
		else if (game == BB){
			bb_play();
			while (bb_WinOrLose == 37);
			REG_TC0_CCR0 |= TC_CCR_CLKDIS;
			if (bb_WinOrLose == 0){
				display_lose();
				while(AorB != buttonB);
			}
			else{
				display_win();
				while(AorB != buttonB);
			}
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
