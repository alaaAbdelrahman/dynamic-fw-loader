/*
 * soft_spi_intf.c
 *
 * Created: 03/05/2018 09:44:56 ص
 *  Author: reda.abdelhameid
 */ 



/********************************** Headers ***********************************/
//#include <compiler.h>
#include "soft_spi_intf.h"  /* It contains the prototypes of APIs & config. of SPI_ADE */
#include "soft_spi_drv.h"
#include "basic_srv.h"
/*----------------------------------------------------------------------------*/





/*******************************************************************************
                              Global Variables
*******************************************************************************/

/*Indicates the driver state un/init*/
SPI_InfStateType gSPI_INF_STATE = SPI_INF_UNINIT;

/*------------------------------------------------------------------------------
              End Global Variables
------------------------------------------------------------------------------*/





/*******************************************************************************
                         static Global Variables
*******************************************************************************/


/*------------------------------------------------------------------------------
               End static Global Variables
------------------------------------------------------------------------------*/





/*******************************************************************************
                            ******************
************************ SPI_ADE Interface Functions ****************************
                            ******************
*******************************************************************************/



/*******************************************************************************
  Function Name   :  vSPI_InfInit

  Parameters(in)  :  None

  Parameters(out) :  gSPI_INF_STATE

  Return value    :  None

  Sych/Asynch     :  Synchronous

  Reentrancy      :  None Reentrant

  Description     :  This function is responsible of starting the initialization
                     of SPI_ADE interface.
*******************************************************************************/

void vSPI_InfInit(void)
{
  spi_soft_init_master();
}/*end function*/
/*------------------------------------------------------------------------------
                       END Function vSPI_InfInit
------------------------------------------------------------------------------*/





/*******************************************************************************
  Function Name   :  bSPI_InfSyncTxByte

  Parameters(in)  :  u8SPIbyte

  Parameters(out) :  None

  Return value    :  SPI_NOT_SENT
                     SPI_SENT

  Sych/Asynch     :  Synchronous

  Reentrancy      :  None Reentrant

  Description     :  sending one byte from SPI to module
*******************************************************************************/

SPI_SentStatusType bSPI_InfSyncTxByte(uint8_t u8SPIbyte)
{
  spi_soft_write_byte(u8SPIbyte);
  /*---***---*/
  return SPI_SENT;
}/*end function*/
/*------------------------------------------------------------------------------
                       END Function bSPI_InfSyncTxByte
------------------------------------------------------------------------------*/





/*******************************************************************************
  Function Name   :  bSPI_InfSyncTxBuf

  Parameters(in)  :  buffer_ptr
                     buff_size

  Parameters(out) :  None

  Return value    :  SPI_NOT_SENT
                     SPI_SENT

  Sych/Asynch     :  Synchronous

  Reentrancy      :  None Reentrant

  Description     :  transmitting buffer array from SPI to flash
*******************************************************************************/
SPI_SentStatusType bSPI_InfSyncTxBuf(uint8_t* buffer_ptr, uint16_t buff_size)
{
  /****************** Local Variables *************************/
  uint16_t index_loop = 0; /*index loop*/
  /*---------------*****------------------*/
  /*loop index buffer to send Buffer byte, by byte but send most firstly*/
  for(index_loop = 0; index_loop < buff_size; index_loop++)
  {
    spi_soft_write_byte(buffer_ptr[index_loop]);
    /*delay to sending data completely to DR*/
    //vDelay_SW((uint16_t)1);
    /*---***---*/
  }/*end loop*/
  return SPI_SENT;
}/*end function*/
/*------------------------------------------------------------------------------
                       END Function bSPI_InfSyncTxBuf
------------------------------------------------------------------------------*/





/*******************************************************************************
  Function Name   :  bSPI_InfSyncRX

  Parameters(in)  :  u8ptrSPI

  Parameters(out) :  None

  Return value    :  uint8_t

  Sych/Asynch     :  Synchronous

  Reentrancy      :  None Reentrant

  Description     :  receiving byte from SPI to EXTERNAL FLASH
*******************************************************************************/

SPI_ReceivingStatusType bSPI_InfSyncRX(uint8_t* u8ptrSPI)
{
  /****************** Local Variables *************************/
  volatile uint8_t temp = 0;
  /*---------------*****------------------*/
  /*waiting time to transmit*/
  temp = (uint8_t)spi_soft_read_byte();
  /*storing received data*/
  *u8ptrSPI = (uint8_t)temp;
  return SPI_RECEIVED;
}/*end function*/
/*------------------------------------------------------------------------------
                       END Function bSPI_InfSyncRX
------------------------------------------------------------------------------*/





/*******************************************************************************
  Function Name   :  bSPI_InfSyncRXBuf

  Parameters(in)  :  buffer_ptr
                     buff_size

  Parameters(out) :  None

  Return value    :  uint8_t

  Sych/Asynch     :  Synchronous

  Reentrancy      :  None Reentrant

  Description     :  receiving bytes from SPI to EXTERNAL FLASH
*******************************************************************************/

SPI_ReceivingStatusType bSPI_InfSyncRXBuf(uint8_t* buffer_ptr, uint16_t buff_size)
{
  /****************** Local Variables *************************/
  uint8_t temp = 0;
  uint16_t index_loop = 0; 

  /*---------------*****------------------*/
  /*loop index buffer to store Buffer byte by byte but store lest firstly*/
  for(index_loop = 0; index_loop <buff_size; index_loop++)
  {
    temp = (uint8_t)spi_soft_read_byte();
    /*storing received data*/
    buffer_ptr[index_loop ] = (uint8_t)temp;
  }/*end for loop*/
  return SPI_RECEIVED ;
}/*end function*/
/*------------------------------------------------------------------------------
                       END Function bSPI_InfSyncRXBuf
------------------------------------------------------------------------------*/


