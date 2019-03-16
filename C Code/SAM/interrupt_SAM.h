/*
 * interrupt_SAM.h
 *
 * Created: 3/16/2019 11:34:50 AM
 *  Author: SamanthaSoto
 */ 


#ifndef INTERRUPT_SAM_H_
#define INTERRUPT_SAM_H_

#include "sam.h"

void interrupt_init(void);
void PIOA_Handler(void);


#endif /* INTERRUPT_SAM_H_ */