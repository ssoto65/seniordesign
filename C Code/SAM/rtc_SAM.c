#include "sam.h"
#include "sam4s.h"


void slowClock_init(void){
    //slow clock setup
    //switch from RC osc to external crystal, disable RC osc, disable PIO on xtal pins
    REG_SUPC_CR |= SUPC_CR_XTALSEL | SUPC_CR_KEY_PASSWD;
    //read status register.oscsel bit and wait for slow clock to be selected
    while (!(REG_SUPC_SR & SUPC_SR_OSCSEL));

    //set crystal correction
    REG_RTC_MR &= ~RTC_MR_HIGHPPM;
    //correction = (3906/20*ppm)-1 = 8.765
    REG_RTC_MR |= RTC_MR_CORRECTION(9);
}

uint8_t bcdToDecimal(uint8_t bcdByte){
    return ((((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F));
}

uint8_t decimalToBCD(uint8_t decByte){
    return (((decByte/10) <<4) | (decByte%10));
}

void rtcSetTime(uint8_t hour, uint8_t minute, uint8_t second){
    uint32_t newTime = 0;

    //convert input into BCD and pass values to register
    newTime |= RTC_TIMR_SEC(decimalToBCD(second));
    newTime |= RTC_TIMR_HOUR(decimalToBCD(hour));
    newTime |= RTC_TIMR_MIN(decimalToBCD(minute));
    //printWord (newTime>>16);
    //printWord (newTime);

    //stop rtc time counting
    REG_RTC_CR |= RTC_CR_UPDTIM;
    //check that the time is stopped
    while(!(REG_RTC_SR & RTC_SR_ACKUPD));
    //clear ackupd
    REG_RTC_SCCR |= RTC_SCCR_ACKCLR;

    REG_RTC_TIMR = newTime;
    //clear updtim in rtc_cr
    REG_RTC_CR &= ~RTC_CR_UPDTIM;
    //start clock
    REG_RTC_SCCR |= RTC_SCCR_TIMCLR;
}

uint32_t rtcGetTime(void){
    uint32_t time1=1;
    uint32_t time2=2;
    /*read reg_rtc_timr twice and compare
    if it's the same then the data is good
    otherwise read again twice until it matches
    */
    while (!(time1==time2)){
        time1 = REG_RTC_TIMR;
        time2 = REG_RTC_TIMR;
    }
    return time2;
}

void printTime(void){
    uint32_t timeData = rtcGetTime();

    uint8_t hour = (timeData & RTC_TIMR_HOUR_Msk) >> RTC_TIMR_HOUR_Pos;
    uint8_t minute = (timeData & RTC_TIMR_MIN_Msk) >> RTC_TIMR_MIN_Pos;
    uint8_t second = (timeData & RTC_TIMR_SEC_Msk) >> RTC_TIMR_SEC_Pos;
	/*
    printByte (bcdToDecimal(hour));
    printString(":");
    printByte (bcdToDecimal(minute));
    printString(":");
    printByte (bcdToDecimal(second));
    printString ("\r\n");
    */
}
