
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
#include "rtc_SAM.h"


#include "ff.h"
#include "diskio.h"

void initCLK(void);
void SetVcoreUp (unsigned int level);
FRESULT WriteFile(char*, char*, WORD);
void fat_init(void);



#define BAM 0
#define BB 1
extern volatile uint8_t game;
volatile uint8_t play = 0;

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

volatile uint32_t wireless_mode = 0;


#define bb_refresh 2000
#define bam_refresh 25

FIL file;                                               /* Opened file object */
FATFS fatfs;                                            /* File system object */
DIRS dir;                                               /* Directory object   */
FRESULT errCode;                                        /* Error code object  */
FRESULT res;                                            /* Result object      */
UINT bytesRead;                                         /* Bytes read object  */
UINT read;                                              /* Read bytes object  */

unsigned char MST_Data,SLV_Data;
BYTE buffer[32];
int result=1;
uint8_t songByte;

uint8_t musicFlags[10];
#define bam_ball_move 0
#define bam_wall_hit 3
#define bb_paddle_hit 4

#define beep_dur 0x1810
#define block_dur 0x1810

uint16_t beep[6160]; //0x1810
uint16_t block[6160]; //0x1810
uint16_t music_output;

unsigned int bytesWritten;
UINT TEST[1] = {0};
volatile UINT cnt;
volatile UINT readTest[15];
volatile uint32_t songLength = 0;
volatile uint32_t counter = 0;


int main(void)
{
	NVIC_SetPriorityGrouping(0U);
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
	
	//effects
	fat_init_effects();
	//menu
	fat_init_music();
	
	slowClock_init();
	//NVIC_DisableIRQ(PIOA_IRQn);
	//SD Card // Pin 28/PA16(SS) | Pin 31/PA14(SPCK) | Pin 33/PA13(MOSI) | Pin 41/PA12(MISO) 
	__enable_irq();        
	REG_TC0_CCR0 |= TC_CCR_SWTRG;            
	
	while(1){
		while(AorB != buttonA){
			if (game == BB){
				display_menu_bb();
				game = BAM;
				for (volatile int ind = 0; ind < 0x543; ind++){
					DAC_write(block[ind], 0);
					//music_output = (music_output + effects[ii]) >> 1;
				}
				while(!(upDown == buttonDOWN)){
					if (AorB == buttonA) {
						game = BB;
						 break;}
				}
				}else{
				display_menu_bam();
				game = BB;
				for (volatile int ind = 0; ind < 0x543; ind++){
					DAC_write(block[ind], 0);
					//music_output = (music_output + effects[ii]) >> 1;
				}
				while(!(upDown == buttonUP)){
					if (AorB == buttonA) {
						game = BAM;
						break;
					}
				}
			}
		}
		
	//	play = 1;
		rtcSetTime(0,0,0);
			
		if (game == BAM){
			bam_play();
			play = 1;
			while(bam_WinOrLose == 37);
			//REG_TC0_CCR0 |= TC_CCR_CLKDIS;
			if (bam_WinOrLose == 0){
				display_lose();
				play = 0;
				while(AorB != buttonB);
			}
			else{
				display_win();
				play = 0;
				while(AorB != buttonB);
			}
		}
		
		else if (game == BB){
			bb_play();
			play = 1;
			while (bb_WinOrLose == 37);
			//REG_TC0_CCR0 |= TC_CCR_CLKDIS;
			if (bb_WinOrLose == 0){
				display_lose();
				play = 0;
				while(AorB != buttonB);
			}
			else{
				display_win();
				play = 0;
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

void fat_init_effects(void){
	/////////////// BEEP ///////////////
	errCode = -1;

	while (errCode != FR_OK){                               //go until f_open returns FR_OK (function successful)
		errCode = f_mount(0, &fatfs);                       //mount drive number 0
		errCode = f_opendir(&dir, "/");				    	//root directory

		errCode = f_open(&file, "/beep.wav", FA_READ);
		if(errCode != FR_OK)
		result=0;                                       //used as a debugging flag
		if(errCode == FR_INT_ERR)
		f_close(&file);
	}
	
	for (volatile int ii = 0; ii < 0x1810; ii++){
		errCode = f_read(&file, &TEST, 1, &cnt);
		beep[ii] = (*TEST << 2);
	}
	errCode = f_close(&file);
	errCode = f_mount(0,0);
	
	//////////////////////////////////////
	
	/////////////// BLOCK ///////////////
	errCode = -1;

	while (errCode != FR_OK){                               //go until f_open returns FR_OK (function successful)
		errCode = f_mount(0, &fatfs);                       //mount drive number 0
		errCode = f_opendir(&dir, "/");				    	//root directory

		errCode = f_open(&file, "/block.wav", FA_READ);
		if(errCode != FR_OK)
		result=0;                                       //used as a debugging flag
		if(errCode == FR_INT_ERR)
		f_close(&file);
	}
	
	for (volatile int ii = 0; ii < 0x1810; ii++){
		errCode = f_read(&file, &TEST, 1, &cnt);
		block[ii] = (*TEST << 2);
	}
	errCode = f_close(&file);
	errCode = f_mount(0,0);
	//////////////////////////////////////
	
}

void fat_init_music(void){
	errCode = -1;

	while (errCode != FR_OK){                               //go until f_open returns FR_OK (function successful)
		errCode = f_mount(0, &fatfs);                       //mount drive number 0
		errCode = f_opendir(&dir, "/");				    	//root directory

		errCode = f_open(&file, "/menu.wav", FA_READ);
		if(errCode != FR_OK)
		result=0;                                       //used as a debugging flag
		if(errCode == FR_INT_ERR)
		f_close(&file);
	}
}

void TC0_Handler(void){
	
	wireless_mode = (REG_PIOA_PDSR & PIO_PDSR_P29);
	wireless_mode = ((REG_PIOA_PDSR >> 29) & 1);
	if(wireless_mode == 1){
		//uint32_t status = REG_UART1_SR;
		//while(!(status & UART_SR_ENDRX));
		NVIC_EnableIRQ(UART1_IRQn);
		REG_UART1_IER |= UART_IER_RXRDY;
	}
	else if (wireless_mode == 0){
		NVIC_DisableIRQ(UART1_IRQn);
		REG_UART1_IER = 0;
	}
	
	__disable_irq();        
	//read status register - this clears interrupt flags
	
	uint32_t status = REG_TC0_SR0;
	if ((status & TC_SR_CPCS)>=1){
		//cleared flag
		//increment counter
		counter+=1;
	}
	
	
	if (counter>bb_refresh){
		//reset counter
		counter=0;
		if ((game == BAM) && play){
			mazerefresh();
		}
		else if ((game == BB) && play){
			ballRefresh();
			paddleRefresh();
		}
		
		if(musicFlags[bam_ball_move] == 1){
			for (volatile int ind = 0; ind < beep_dur; ind++){
				//DAC_write(beep[ind], 0);
				music_output = beep[ind];
			}
			musicFlags[bam_ball_move] = 0;
		}
		else if((musicFlags[bb_paddle_hit] == 1) || (musicFlags[bam_wall_hit] == 1) ){
			for (volatile int ind = 0; ind < block_dur; ind++){
				//DAC_write(block[ind], 0);
				music_output = block[ind];
		}
			musicFlags[bb_paddle_hit] = 0;
			musicFlags[bam_wall_hit] = 0;
		}	
	}
	//READ
	errCode = f_read(&file, &TEST, 1, &cnt);
	//music_output = *TEST << 2;
	music_output = (music_output + *TEST) >> 1;
	DAC_write(music_output << 2, 0);
	songLength++;
	if (songLength+1 >=  0x00399DFF){
		f_lseek(&file, (f_tell(&file) - songLength));
		songLength = 0;
	}
	//DAC_write(music_output, 0);
	__enable_irq();        
	
}