
#ifndef SPI_Config_H_
#define SPI_Config_H_


#include"config.h"

#define S_SPI_FAIL                                0x01
#define S_SPI_SUCCESS                             0x00



#if (MicroController == Micro_V94XX)
#define S_SPI1                                SPI
#define S_SPI2                                SPI
#else
#define S_SPI1                                SPI1
#define S_SPI2                                SPI2
#endif


#if (MicroController == Micro_V94XX)            /*Define new Micro*/
#define S_SPI1_tx_byte(data)                  SPI->TXDAT = data
#define S_SPI1_rx_byte()                      SPI->RXDAT
#define S_SPI2_tx_byte(data)                  SPI->TXDAT = data
#define S_SPI2_rx_byte()                      SPI->RXDAT
#else
#define S_SPI1_tx_byte(data)                  SPI1->TXDAT = data
#define S_SPI1_rx_byte()                      SPI1->RXDAT
#define S_SPI2_tx_byte(data)                  SPI2->TXDAT = data
#define S_SPI2_rx_byte()                      SPI2->RXDAT
#endif


#define S_SPI_DeviceInit(DEVICE)              SPI_DeviceInit(DEVICE)
#define S_SPI_CLKDIV_128                      SPI_CLKDIV_128
#define S_SPI_MODE_MASTER                     SPI_MODE_MASTER
#define S_SPI_CSNSOFT_ENABLE                  SPI_CSNSOFT_ENABLE
#define S_SPI_SPH_1                           SPI_SPH_1
#define S_SPI_SPO_1                           SPI_SPO_1
#define S_SPI_SWAP_DISABLE                    SPI_SWAP_DISABLE
#define S_SPI_STS_BSY                         SPI_STS_BSY

#define S_SPI_Init(Device,SPIStructINIT)      SPI_Init(Device,SPIStructINIT)
#define S_SPI_Cmd(Device,State)               SPI_Cmd(Device,State)
#define S_SPI_GetStatus(Device,STATE)         SPI_GetStatus(Device,STATE)

#endif /* SPI_Config_H_ */
