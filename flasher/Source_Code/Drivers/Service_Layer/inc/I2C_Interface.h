
#ifndef I2C_Interface_H_
#define I2C_Interface_H_

#include "config.h"

#if (MicroController == Micro_V94XX)                                                  /*check it with ENG.ESmail*/                       
#include "V94XX_I2C.h"
#else
#include "v85xx_i2c.h"
#endif

#include "I2C_Config.h"

/********************************** Headers ***********************************/


void    S_I2C_INIT(uint8_t Slave_Address, uint8_t DevType) ;
uint8_t S_I2C_Key_TX_frame(uint8_t device_address, uint8_t data_len, uint8_t * data_buf);
uint8_t S_I2C_Key_RX_frame(uint8_t device_address, uint8_t data_len, uint8_t * data_buf);
uint8_t S_I2C_Write(uint16_t slave_address , uint32_t location, uint8_t* data_buffer,uint16_t data_size ,uint16_t page_size,uint8_t DevType);
uint8_t S_I2C_Read(uint16_t slave_address , uint32_t location, uint8_t* data_buffer,uint16_t data_size ,uint16_t page_size,uint8_t DevType);
#endif /* I2C_Interface_H_ */
