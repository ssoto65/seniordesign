
#include "sam.h"
#include "i2c_SAM.h"
#include "HAL_SDCard.h"
#include "uart_SAM.h"
#include "led_SAM.h"
#include "dac_SAM.h"

volatile int avgCnt = 0;
volatile int accel_test = 1;

volatile int8_t dataready = 0;
volatile int16_t accel_x = 0;
volatile int16_t accel_y = 0;
volatile int16_t accel_z = 0;

void clock_init(void);


int level_one[32][32] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,1,0,0,0,0,0,0,0,0,1,0,2,0,0,2,0,0,0,0,2,0,0,3,0,0,0,2,0,1},
{1,0,0,1,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,0,0,0,2,0,3,3,3,0,2,2,2,1},
{1,0,0,1,0,0,2,2,2,2,0,0,1,0,2,0,0,2,0,0,0,0,2,0,0,3,0,0,0,2,0,1},
{1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,1},
{1,0,0,1,0,2,0,0,0,1,0,0,0,0,2,0,0,2,0,0,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,1,2,2,2,0,0,1,1,1,1,2,2,2,2,2,2,1,0,0,2,0,0,0,0,0,0,2,0,1},
{1,0,0,1,0,2,0,0,0,0,0,0,0,0,2,0,0,2,0,1,0,0,2,0,0,1,1,1,2,2,2,1},
{1,0,0,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,2,0,1},
{1,0,0,1,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,0,0,0,1,0,0,2,1,1,1,1,1,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,2,0,0,0,1,0,0,2,0,0,0,2,0,0,2,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1,0,0,2,0,0,2,2,2,2,2,2,1},
{1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,1,0,0,2,0,0,0,2,0,0,2,0,1},
{1,0,2,2,2,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,2,0,0,0,0,1,0,2,0,0,0,0,1,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,1,2,2,2,0,0,0,1,0,0,0,2,0,0,2,2,2,2,2,2,2,0,0,1},
{1,0,0,0,0,2,0,0,1,0,2,0,0,0,0,1,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,1},
{1,0,0,0,2,2,2,0,1,0,0,0,0,0,0,1,0,0,0,2,0,0,2,0,0,0,0,0,2,0,0,1},
{1,0,0,0,0,2,0,0,1,0,0,0,0,0,0,1,0,0,0,2,0,0,2,0,0,2,0,0,2,0,0,1},
{1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,2,0,0,2,0,0,2,0,0,2,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,2,0,0,0,0,0,2,0,0,2,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,2,0,0,2,0,0,1},
{1,0,2,0,0,0,0,0,0,0,2,0,1,0,0,0,0,0,0,1,0,2,0,0,0,2,0,0,2,0,0,1},
{1,2,2,2,1,1,1,1,1,2,2,2,0,0,0,0,0,0,0,1,2,2,2,0,0,2,0,0,0,0,0,1},
{1,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,2,0,0,0,2,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

int main(void)
{
	SystemInit();
	REG_WDT_MR |= WDT_MR_WDDIS;
	clock_init();
	DAC_init(); //Pin 93/PB13 (DAC Output)
	//interrupt_init(); //Pin 24/PA20 (LED) | Pin 22/PA23 (Int Button)
	i2c_init(); //Pin 66/PA3(SDA) | Pin 55/PA4(SCK)
	clk_out(); //init for EBI uP clk // Pin 52/PA6(uP clk)
	UART_Init(); //Pin 7/PB2(RX) | Pin 9/PB3(TX)
	ebi_init();
	NVIC_EnableIRQ(UART1_IRQn);
	//SD Card // Pin 28/PA16(SS) | Pin 31/PA14(SPCK) | Pin 33/PA13(MOSI) | Pin 41/PA12(MISO) 
	
	
	clear_matrix();
	
	for(int i=0; i < 10; i++){
		for(int ii = 0; ii < 32; ii++){
			//ii = 0;
			for(int jj = 0; jj < 32; jj++){
				
				if(level_one[ii][jj]== 1){
					
					set_LED(ii,jj,0xFF0000);
					
					}else if(level_one[ii][jj]== 2){
						
					set_LED(ii,jj,0x0000FF);
				}
				else if(level_one[ii][jj]== 3){
					
					set_LED(ii,jj,0x00FF00);
				}
			}
		}
	}

		

	
	REG_PIOA_PER |= PIO_PER_P21; //pio enable PA11
	REG_PIOA_OER |= PIO_PER_P21; //set PA11 as output
	
	//UP
	REG_PIOA_PER |= PIO_PER_P5; //pio enable PA
	REG_PIOA_OER |= PIO_PER_P5; //set PA11 as output
	
	//LEFT
	REG_PIOA_PER |= PIO_PER_P7; //pio enable PA
	REG_PIOA_OER |= PIO_PER_P7; //set PA11 as output
	
	//RIGHT
	REG_PIOA_PER |= PIO_PER_P8; //pio enable PA
	REG_PIOA_OER |= PIO_PER_P8; //set PA11 as output
	
	//DOWN
	REG_PIOA_PER |= PIO_PER_P10; //pio enable PA
	REG_PIOA_OER |= PIO_PER_P10; //set PA11 as output
	
	//Initialize ball
	uint8_t xPos = 1;
	uint8_t yPos = 1;
	set_LED(xPos,yPos,0x00FFFFFF);
	
	while (1)
	{
		
		//Accelerometer/I2C Test Code
		//accel_test = who_am_I();
		
		//if (accel_test != 0x73){
		//	volatile int yeet = 0;
		//}
		
		
		//UART Test
		//transmitByte(accel_test);
		
		/*
		//DAC Test Code
		for(int ii = 0; ii < 4096; ii++){
			DAC_write(ii,0);
		}
		*/
		
		accel_data();
		avgCnt++;
		
		
		if(avgCnt == 5){
			avgCnt = 0;
			//RIGHT
			if( (accel_x > -750) && (accel_x < 750) && (accel_y < -1600)){
				
				if (xPos > 0){
					set_LED(xPos,yPos,0x00);
					set_LED(--xPos,yPos,0x00FFFFFF);
				}
					
				
				/*
				set_LED(15,16,0x00FFFFFF);
			
				//set_LED(15,16,0x00);
				set_LED(17,16,0x00);
				set_LED(16,15,0x00);
				set_LED(16,17,0x00);
				*/
			
			}
			//LEFT
			else if( (accel_x > -850) && (accel_x < 850) && (accel_y > 500)){
				
				
				
				
				if (xPos < 31){
					set_LED(xPos,yPos,0x00);
					set_LED(++xPos,yPos,0x00FFFFFF);
				}
				
				/*
				set_LED(17,16,0x00FFFFFF);
			
				set_LED(15,16,0x00);
				//set_LED(17,16,0x00);
				set_LED(16,15,0x00);
				set_LED(16,17,0x00);
				*/
				
			}
			//DOWN
			else if( (accel_y > -850) && (accel_y < 850) && (accel_x < -700)){
				
				
				
				
				if (yPos < 31){
					set_LED(xPos,yPos,0x00);
					set_LED(xPos,++yPos,0x00FFFFFF);
				}
				
				/*
				
				set_LED(16,17,0x00FFFFFF);
				
				set_LED(15,16,0x00);
				set_LED(17,16,0x00);
				set_LED(16,15,0x00);
				*/
				
			}
			//UP
			else if( (accel_y > -850) && (accel_y < 850) && (accel_x > 650)){
				
				
				
				if (yPos > 0){
					set_LED(xPos,yPos,0x00);
					set_LED(xPos,--yPos,0x00FFFFFF);
				}
				
				/*
				set_LED(16,15,0x00FFFFFF);
				
				set_LED(15,16,0x00);
				set_LED(17,16,0x00);
				//set_LED(16,15,0x00);
				set_LED(16,17,0x00);
				*/
				
			/*
			else{
				
				set_LED(15,16,0x00);
				set_LED(17,16,0x00);
				set_LED(16,15,0x00);
				set_LED(16,17,0x00);
				
			}*/
	}
		//the program will loop here until an interrupt is triggered
	}
	}
}

void interrupt_init(void){
	//enable clock for PIOA
	REG_PMC_PCER0 |= PMC_PCER0_PID11;
	
	//Setup Input and Output pins and an interrupt
	//set PA30 as controllable by the PIO controller (disable peripheral)
	REG_PIOA_PER |= PIO_PER_P20;
	//set PA30 as output for LED
	REG_PIOA_OER |= PIO_OER_P20;
	//set PA30 high
	REG_PIOA_SODR |= PIO_SODR_P20;
	
	//set PA23 as controllable by the PIO controller (disable peripheral)
	REG_PIOA_PER |= PIO_PER_P23;
	//output disable register (set as input for button)
	REG_PIOA_ODR |= PIO_ODR_P23;
	//disable pull down
	REG_PIOA_PPDDR |= PIO_PPDDR_P23;
	//enable internal pull up resistor on PA23
	REG_PIOA_PUER |= PIO_PUER_P23;
	//enable glitch filter on PA23 (button debounce)
	REG_PIOA_IFER |= PIO_IFER_P23;
	//Read ISR so that it clears any interrupt flags that might be there
	uint32_t temp = REG_PIOA_ISR;
	//enable input change interrupt on PA23
	REG_PIOA_IER |= PIO_IER_P23;
	//enable PIOA interrupts
	NVIC_EnableIRQ(PIOA_IRQn);
}

//when PIOA interrupt is triggered this block of code will run
void PIOA_Handler(void) {
	/*
	// reading PIOA_ISR will clear interrupt flags
	uint32_t status = REG_PIOA_ISR;
	
	if ((status & PIO_ISR_P23) >= 1){ //pin change interrupt on P23
		//if PA20 high then set low, if PA20 low then set high
		if ((REG_PIOA_PDSR & PIO_ODSR_P20)>=1){
			//set PA20 low
			REG_PIOA_CODR |= PIO_CODR_P20;
		}
		else{
			//set PA20 high
			REG_PIOA_SODR |= PIO_SODR_P20;
		}
	}
	*/
}

void UART1_Handler(void) {
	// when we receive a byte, transmit that byte back
	uint32_t status = REG_UART1_SR;
	if ((status & UART_SR_RXRDY)){
		//read receive holding register
		uint8_t readByte = REG_UART1_RHR;
		//transmit that byte back
		//transmitByte(readByte);
		if (readByte == 0x37){
			REG_PIOA_SODR |= PIO_PER_P21; //set PA11 high (LED on)
		}
		else if (readByte == 0x44){
			REG_PIOA_CODR |= PIO_PER_P21; //set PA11 low (LED Off)
		}
		
	}
}

void accel_data(void){
	while(!dataready){
		REG_TWI0_IADR = 58; //Reading Status Register
		i2c_start(0x69,read);
		dataready = i2c_readNACK();
	}
	
	REG_TWI0_IADR = 59; //Reading XH
	i2c_start(0x69,read);
	accel_x = (i2c_readNACK() << 8);
	REG_TWI0_IADR = 60; //Reading XL
	i2c_start(0x69,read);
	accel_x |= i2c_readNACK();
	
	REG_TWI0_IADR = 61; //Reading YH
	i2c_start(0x69,read);
	accel_y = (i2c_readNACK() << 8);
	REG_TWI0_IADR = 62; //Reading YL
	i2c_start(0x69,read);
	accel_y |= i2c_readNACK();
	
	REG_TWI0_IADR = 63; //Reading ZH
	i2c_start(0x69,read);
	accel_z = (i2c_readNACK() << 8);
	REG_TWI0_IADR = 64; //Reading ZL
	i2c_start(0x69,read);
	accel_z |= i2c_readNACK();
}

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
	
	//12MHz / 6 = 3MHz, 3MHz * (9+1) = 30MHz
	REG_CKGR_PLLBR |= CKGR_PLLBR_MULB(24);
	REG_CKGR_PLLBR |= CKGR_PLLBR_DIVB(6);

	//select PLLB as the master clock
	//master clock source selection - choose main clock
	REG_PMC_MCKR |= PMC_MCKR_CSS_PLLB_CLK;
}