#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart_Tiny.h"
#include "button_Tiny.h"
//#include "i2c_Tiny.h"


int main(void)
{
	buttonINT();
	uartInit();	while(1);
}