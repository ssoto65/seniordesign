/*
 * spi_driver.h
 *
 * Created: 2/7/19 2:09:43 PM
 *  Author: NPoindexter
 */ 

#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_

void SPI_setMode(uint8_t mode);
void SPI_selectPeripheral(uint8_t peripheral);
void SPI_deselect(void);
void SPI_init(void);
void slaveselect(void);
void slavedeselect(void);
void flirReset_assert(void);
void flirReset_deassert(void);
uint8_t SPI_byteExchange(uint8_t data);

#endif