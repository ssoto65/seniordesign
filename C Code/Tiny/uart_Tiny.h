#include <avr/io.h>

void uartInit(void);
uint8_t uartRX(void);
void uartTX(uint8_t data);