#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart_Tiny.h"
#include "button_Tiny.h"
#include "i2c_Tiny.h"


#define accel_right 8
#define accel_left 9
#define accel_up 10
#define accel_down 11
#define accel_stop 12int16_t accel_x;int16_t accel_y;int16_t accel_z;
uint8_t dataready = 0;

int main(void)
{	i2c_init();
	buttonINT();
	uartInit();	tcc_init();			while(1);
}

ISR(TCA0_OVF_vect)
{
	while(!dataready){
			dataready = i2c_read(58);
		}				accel_x = (i2c_read(59) << 8);		accel_x |= i2c_read(60);			accel_y = (i2c_read(61) << 8);		accel_y |= i2c_read(62);			accel_z = (i2c_read(63) << 8);		accel_z |= i2c_read(64);
		
		//Down
		if (((((accel_x > -850) && (accel_x < 850) && (accel_y < -1800))))){
			uartTX(accel_up); 
			asm("nop");
		}
		else if((accel_x > -850) && (accel_x < 850) && (accel_y > 1800)){
			uartTX(accel_down);
			asm("nop");
		}
		else if ((accel_y > -850) && (accel_y < 850) && (accel_x < -1800)){
			uartTX(accel_right);
			asm("nop");
		}
		else if ((accel_y > -850) && (accel_y < 850) && (accel_x > 1800)){
			uartTX(accel_left);
			asm("nop");
		}
		else {
			uartTX(accel_stop);
			asm("nop");
		}

	/* The interrupt flag has to be cleared manually */
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}