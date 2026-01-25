#include "SPI_Interface.h"
#include "UART_Interface.h"

void S_SPI1_INIT()
{
  
  SPI_InitType SPI_InitStruct;
  
  /* S_SPI1 initialization, master mode */
  
  S_SPI_DeviceInit(S_SPI1);
  SPI_InitStruct.ClockDivision = SPI_CLKDIV_2;//S_SPI_CLKDIV_128;
  SPI_InitStruct.Mode = S_SPI_MODE_MASTER;
  SPI_InitStruct.CSNSoft = S_SPI_CSNSOFT_ENABLE;
  SPI_InitStruct.SPH = S_SPI_SPH_1;
  SPI_InitStruct.SPO = S_SPI_SPO_1;
  SPI_InitStruct.SWAP = S_SPI_SWAP_DISABLE;
  S_SPI_Init(S_SPI1, &SPI_InitStruct);
  
  S_SPI_Cmd(S_SPI1, ENABLE);
  
}
uint8_t S_SPI1_TX_FRAME(uint8_t* data_buf, uint16_t size)
{
  uint16_t loop ;
  uint32_t timeout = 0 ;
  for (loop = 0 ; loop < size ; loop ++) 
  {
    S_SPI1_tx_byte(data_buf[loop]) ;
    while ((S_SPI_GetStatus(S_SPI1,S_SPI_STS_BSY )) &&((timeout++) <100));
    if(timeout >= 100)
    {
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('&',0);
#endif
      return S_SPI_FAIL ;
    }
    timeout =  S_SPI1_rx_byte() ;timeout = 0;
    timeout = 0;
    while ((S_SPI_GetStatus(S_SPI1,S_SPI_STS_BSY )) &&((timeout++) <100));
    if(timeout >= 100)
    {
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('&',0);
#endif
      return S_SPI_FAIL ;
    }
  }
  
  return S_SPI_SUCCESS ;
}
uint8_t SPI1_rx_frame(uint8_t* data_buf, uint16_t size)
{
  uint16_t loop ;
  uint32_t timeout = 0 ;
  uint8_t dummy_data = 0x11;
  for (loop = 0 ; loop < size ; loop ++)
    
  {
    S_SPI1_tx_byte(dummy_data) ;
    while ((S_SPI_GetStatus(S_SPI1,S_SPI_STS_BSY )) &&(timeout <10000)) //TTODO
    {
      timeout++ ;
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('y',0);
#endif
    }
    if(timeout >= 10000)
    {
      return S_SPI_FAIL ;
    }
    timeout = 0;
    data_buf[loop] =  S_SPI1_rx_byte() ;
    while ((S_SPI_GetStatus(S_SPI1,S_SPI_STS_BSY )) &&(timeout <10000))//TTODO
    {
      timeout++ ;
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('y',0);
#endif
    }
    if(timeout >= 10000)
    {
      return S_SPI_FAIL ;
    }
    
    
    /* for debug only*/
    
  }
  
  return S_SPI_SUCCESS ;
}


void S_SPI2_INIT()
{
  SPI_InitType SPI_InitStruct;
  
  /* S_SPI1 initialization, master mode */
  
  S_SPI_DeviceInit(S_SPI2);
  
  SPI_InitStruct.ClockDivision = SPI_CLKDIV_128 ;//S_SPI_CLKDIV_128;
  SPI_InitStruct.Mode = S_SPI_MODE_MASTER;
  SPI_InitStruct.CSNSoft = S_SPI_CSNSOFT_ENABLE;
  SPI_InitStruct.SPH = SPI_SPH_0 ;             //SPI_SPH_0;    //S_SPI_SPH_1; 
  SPI_InitStruct.SPO = SPI_SPO_0 ;            //SPI_SPO_0;    //S_SPI_SPO_1; 
  SPI_InitStruct.SWAP = S_SPI_SWAP_DISABLE;
  S_SPI_Init(S_SPI2, &SPI_InitStruct);
  
  S_SPI_Cmd(S_SPI2, ENABLE);
  
}

uint8_t S_SPI2_TX_FRAME(uint8_t* data_buf, uint16_t size)
{
  uint16_t loop ;
  uint32_t timeout = 0 ;
  for (loop = 0 ; loop < size ; loop ++) 
  {
    S_SPI2_tx_byte(data_buf[loop]) ;
    while ((S_SPI_GetStatus(S_SPI2,S_SPI_STS_BSY )) &&((timeout++) <100));
    if(timeout >= 100)
    {
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('&',0);
#endif
      return S_SPI_FAIL ;
    }
    timeout =  S_SPI2_rx_byte() ;timeout = 0;
    timeout = 0;
    while ((S_SPI_GetStatus(S_SPI2,S_SPI_STS_BSY )) &&((timeout++) <100));
    if(timeout >= 100)
    {
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('&',0);
#endif
      return S_SPI_FAIL ;
    }
  }
  
  return S_SPI_SUCCESS ;
}
  


uint8_t SPI2_rx_frame(uint8_t* data_buf, uint16_t size)
{
  uint16_t loop ;
  uint32_t timeout = 0 ;
  uint8_t dummy_data = 0x11;
  for (loop = 0 ; loop < size ; loop ++)
    
  {
    S_SPI2_tx_byte(dummy_data) ;
    while ((S_SPI_GetStatus(S_SPI2,S_SPI_STS_BSY )) &&(timeout <1000)) //TTODO
    {
      timeout++ ;
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('y',0);
#endif
    }
    if(timeout >= 1000)
    {
      return S_SPI_FAIL ;
    }
    timeout = 0;
    data_buf[loop] =  S_SPI2_rx_byte() ;
    while ((S_SPI_GetStatus(S_SPI2,S_SPI_STS_BSY )) &&(timeout <1000))//TTODO
    {
      timeout++ ;
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('y',0);
#endif
    }
    if(timeout >= 1000)
    {
      return S_SPI_FAIL ;
    }
    
    
    /* for debug only*/
    
  }
  
  return S_SPI_SUCCESS ;
}
