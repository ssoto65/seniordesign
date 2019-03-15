

#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#define read	1
#define write	0

void i2c_init(void);
void i2c_stop(void);
void i2c_start(uint8_t slave_address, uint8_t mread);
uint8_t i2c_readACK(void);
uint8_t i2c_readNACK(void);
void i2c_writeLastByte(uint8_t data);
void i2c_write(uint8_t data);
int who_am_I(void);
int read_register(uint8_t register);
void accel_data(void);

#endif /* I2C_DRIVER_H_ */