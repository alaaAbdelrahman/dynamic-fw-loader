
#ifndef SPI_Interface_H_
#define SPI_Interface_H_


#include "config.h"

#if (MicroController == Micro_V94XX)
#include "V94XX_SPI.h" 
#else
#include "v85xx_spi.h" 
#endif

#include "SPI_Config.h"


void S_SPI1_INIT();
uint8_t SPI1_rx_frame(uint8_t* data_buf, uint16_t size);
uint8_t S_SPI1_TX_FRAME(uint8_t* data_buf, uint16_t size);

void S_SPI2_INIT();
uint8_t SPI2_rx_frame(uint8_t* data_buf, uint16_t size);
uint8_t S_SPI2_TX_FRAME(uint8_t* data_buf, uint16_t size);

#endif /* SPI_Interface_H_ */
