#include "sam.h"
#include "uart_SAM.h"
#include "led_SAM.h"

#define buttonUP 1
#define buttonDOWN 2
#define buttonLEFT 3
#define buttonRIGHT 4
#define buttonA 5
#define buttonB 6
#define buttonDirstop 7

//Changed to fit orientation of main board
#define accel_down 8
#define accel_up 9
#define accel_right 10
#define accel_left 11
#define accel_stop 12

volatile uint8_t accel_dir;

#define paddleLEFT 1
#define paddleRIGHT 2

#define wireless_on 1
#define wireless_off 0 

extern volatile uint32_t wireless_mode;

volatile uint8_t upDown = 0;
volatile uint8_t AorB = 0;

volatile uint8_t paddleDir = 0;

#define BAM 0
#define BB 1
volatile uint8_t game;

void UART_Init(){
	//configure PIO controller A  - disable means enable peripheral on pins
	//disable PIOA control of PA9 and enable peripheral on pin
	REG_PIOA_PDR |= PIO_PDR_P2;
	//disable PIOA control of PA9 and enable peripheral on pin
	REG_PIOA_PDR |= PIO_PDR_P3;
	REG_PIOA_ABCDSR &=  ~(PIO_ABCDSR_P2);
	REG_PIOA_ABCDSR &=  ~(PIO_ABCDSR_P3);
	
	//configure PMC UART Clock
	//enable UART0 clock
	REG_PMC_PCER0 |= PMC_PCER0_PID9;
	
	//configure buad rate
	REG_UART1_BRGR |= 14;
	//fcpu/16xBR 26=4,000,000/(16x9600) /// 2=4,000,000/(16x115,200) /// 27=50,000,000/(16x115,200) //14 = 25,000,000/(16x115,200)
	
	//parity
	REG_UART1_MR |= UART_MR_PAR_NO;
	
	//mode
	//normal mode default
	
	//enable transmit/receive
	REG_UART1_CR |= UART_CR_TXEN;
	REG_UART1_CR |= UART_CR_RXEN;
	
	//enable interrupt on receive
	//REG_UART1_IER |= UART_IER_RXRDY;
	
	NVIC_SetPriority(UART1_IRQn,7);
	
	//NVIC_EnableIRQ(UART1_IRQn);
	
}

void transmitByte(uint8_t data){
	//wait for ready
	while (!(REG_UART1_SR & UART_SR_TXRDY));
	while (!(REG_UART1_SR & UART_SR_TXEMPTY));
	REG_UART1_THR |= data;
}


void printString(const char myString[]) {
	uint8_t i = 0;
	while (myString[i]) {
		transmitByte(myString[i]);
		i++;
	}
}

void UART1_Handler(void) {
	__disable_irq();        
	// when we receive a byte, transmit that byte back
	uint32_t status = REG_UART1_SR;
	
	wireless_mode = (REG_PIOA_PDSR & PIO_PDSR_P29);
	
	if((wireless_mode >> 29) == wireless_on){
	
		if ((status & UART_SR_RXRDY)){
			//read receive holding register
			volatile uint8_t readByte = REG_UART1_RHR;
			//transmit that byte back
			//transmitByte(readByte);
			if (readByte == buttonUP){
				//set_LED(1,0,0x00FFFFFF); //for debug
				upDown = buttonUP;
			}
			else if (readByte == buttonDOWN){
				//set_LED(2,0,0x00FFFFFF); //for debug
			
				upDown = buttonDOWN;
			}
			else if (readByte == buttonLEFT){
				//set_LED(3,0,0x00FFFFFF); //for debug
				paddleDir = paddleLEFT;
			}
			else if (readByte == buttonRIGHT){
				//set_LED(4,0,0x00FFFFFF); //for debug
				paddleDir = paddleRIGHT;
			}
			else if (readByte == buttonDirstop){
				//set_LED(4,0,0x00FFFFFF); //for debug
				paddleDir = 0;
			}
			else if (readByte == buttonA){
				//set_LED(5,0,0x00FFFFFF); //for debug
				//game = BAM;
			
				AorB = buttonA;
			}
			else if (readByte == buttonB){
				//game = BB;
				//set_LED(6,0,0x00FFFFFF); //for debug
				AorB = buttonB;
			}
			else if (readByte == accel_left){
				//game = BB;
				//set_LED(6,0,0x00FFFFFF); //for debug
				accel_dir = accel_left;
			}
			else if (readByte == accel_right){
			//game = BB;
			//set_LED(6,0,0x00FFFFFF); //for debug
			accel_dir = accel_right;
			}
			else if (readByte == accel_up){
			//game = BB;
			//set_LED(6,0,0x00FFFFFF); //for debug
			accel_dir = accel_up;
			}
			else if (readByte == accel_down){
				//game = BB;
				//set_LED(6,0,0x00FFFFFF); //for debug
				accel_dir = accel_down;
			}
			else if (readByte == accel_stop){
				//game = BB;
				//set_LED(6,0,0x00FFFFFF); //for debug
				accel_dir = 0;
			}
		}
	}
	__enable_irq();        
}

void PIOA_Handler(void) {
	__disable_irq();        
	// reading PIOA_ISR will clear interrupt flags
	uint32_t status = REG_PIOA_ISR;
	
	//uint32_t status = REG_PIOA_FRLHSR;
	
	wireless_mode = (REG_PIOA_PDSR & PIO_PDSR_P29);
	
	if(wireless_mode == wireless_off){
		//UP
		if ((status & PIO_ISR_P17) >= 1){ //pin change interrupt on P23
			upDown = buttonUP;
		} //LEFT
		else if((status & PIO_ISR_P18) >= 1){
			paddleDir = paddleLEFT;
		}//DOWN
		else if((status & PIO_ISR_P19) >= 1){
			upDown = buttonDOWN;
		} //RIGHT PRESSED
		else if((status & PIO_ISR_P22) >= 1){
			paddleDir = paddleRIGHT;
		} //A
		else if((status & PIO_ISR_P27) >= 1){
			AorB = buttonA;
		} //B
		else if((status & PIO_ISR_P28) >= 1){
			AorB = buttonB;
		} //wireless mode
		else if((status & PIO_ISR_P29) >= 1){
			//for(int ii = 0; ii<50;ii++){asm("nop");}
		}
	}
	__enable_irq();        
}

//Example code
/* int main(void)
{
Initialize the SAM system
SystemInit();
UART_Init();
NVIC_EnableIRQ(UART1_IRQn);

REG_PIOA_PER |= PIO_PER_P20; //pio enable PA11
REG_PIOA_OER |= PIO_PER_P20; //set PA11 as output

//printString("Waiting for input\r\n");
while (1)
{
//do nothing while waiting for interrupts to trigger
}
}

void UART1_Handler(void) {
// when we receive a byte, transmit that byte back
uint32_t status = REG_UART1_SR;
if ((status & UART_SR_RXRDY)){
//read receive holding register
uint8_t readByte = REG_UART1_RHR;
//transmit that byte back
transmitByte(readByte);
if (readByte == 0x02){
REG_PIOA_SODR |= PIO_PER_P20; //set PA11 high (LED on)
}
else if (readByte == 0x01){
REG_PIOA_CODR |= PIO_PER_P20; //set PA11 low (LED Off)
}

}
}
*/