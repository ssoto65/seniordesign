/*
 * buttonTiny.h
 *
 * Created: 3/17/2019 1:43:02 PM
 *  Author: SamanthaSoto
 */ 


#ifndef BUTTONTINY_H_
#define BUTTONTINY_H_

#include <avr/io.h>

void buttonINT(void);
ISR(PORTA_PORT_vect);



#endif /* BUTTONTINY_H_ */