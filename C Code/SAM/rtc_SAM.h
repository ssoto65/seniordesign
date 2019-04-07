/*
 * rtc_SAM.h
 *
 * Created: 4/6/19 11:38:19 PM
 *  Author: NPoindexter
 */ 


#ifndef RTC_SAM_H_
#define RTC_SAM_H_

void slowClock_init(void);
uint8_t bcdToDecimal(uint8_t bcdByte);
uint8_t decimalToBCD(uint8_t decByte);
void rtcSetTime(uint8_t hour, uint8_t minute, uint8_t second);
uint32_t rtcGetTime(void);




#endif /* RTC_SAM_H_ */