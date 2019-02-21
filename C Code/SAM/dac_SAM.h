/*
 * dac_SAM.h
 *
 * Created: 2/20/19 9:20:45 PM
 *  Author: NPoindexter
 */ 


#ifndef DAC_SAM_H_
#define DAC_SAM_H_

void DAC_init(void);

void DAC_write(uint16_t data, uint8_t channel);

#endif /* DAC_SAM_H_ */