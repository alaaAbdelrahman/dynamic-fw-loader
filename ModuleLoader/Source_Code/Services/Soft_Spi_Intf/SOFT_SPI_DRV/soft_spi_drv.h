/*
 * soft_spi_drv.h
 *
 * Created: 03/05/2018 09:59:59 ص
 *  Author: reda.abdelhameid
 */ 
#include "config.h" 

#ifndef SOFT_SPI_DRV_H_
#define SOFT_SPI_DRV_H_
/* default pins */


//these macros used for any SW usage like SW SPI & DOT_MATRIX LCD to expand speed
#define Set_bit_GPIO(GPIOx ,GPIO_Pin)  (GPIOx->DAT |= GPIO_Pin)
#define Reset_bit_GPIO(GPIOx ,GPIO_Pin)  (GPIOx->DAT &= ~GPIO_Pin)
#define Read_bit_GPIO(GPIOx ,GPIO_Pin)	((GPIOx->STS&GPIO_Pin)?1:0)

void spi_soft_init_master(void);
uint8_t spi_soft_read_byte(void);
void spi_soft_write_byte(uint8_t byte);
/**
  * @}
  */
#endif /* SOFT_SPI_DRV_H_ */