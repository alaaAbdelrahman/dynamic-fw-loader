/*
 * soft_spi_intf.h
 *
 * Created: 03/05/2018 09:45:07 ص
 *  Author: reda.abdelhameid
 */ 


#ifndef SOFT_SPI_INTF_H_
#define SOFT_SPI_INTF_H_



/********************************** Headers ***********************************/
#include "soft_spi_drv.h"

/******************************************************************************/





/*******************************************************************************
            SPI Interface Configuration
*******************************************************************************/
/* This container contains the configuration parameters of the SPI Interface
 */

/********************** SPI Selected Module *****************************/
/* select the SPI Module is being used to interface*/

#define  SPI_SELCTED_MODULE   SPI_MODULE_2  /*two types of modules: SPI_MODULE_1
SPI_MODULE_2*/
/*---***---*/

/********************** Time out error *****************************/
/* determine time out duration*/

#define  SPI_TIME_OUT_DURATION   (uint16_t)200
/*---***---*/

/*------------------------------------------------------------------------------
                           End of Configuration
  ----------------------------------------------------------------------------*/





/*******************************************************************************
                          Configuration Parameters
*******************************************************************************/


/*SPI_SELCTED_MODULE*/
/* it contains all parameters which selected by SPI_SELCTED_MODULE */
enum
{
  SPI_MODULE_1,
  SPI_MODULE_2
};
/*-------*/
#if 0
enum
{
	FALSE,     /* false = 0, true = 1 */
	TRUE,
};
#endif
/*------------------------------------------------------------------------------
              End of Configuration Parameters
  ----------------------------------------------------------------------------*/





/*******************************************************************************
                        Types definitions
 ******************************************************************************/

/*Indicates SPI interface state*/
typedef enum
{
	SPI_INF_READY ,
  SPI_INF_UNINIT  //= FALSE,
 // SPI_INF_READY  //= TRUE

} SPI_InfStateType;
/*------*/

/*Indicates SPI interface state*/
typedef enum
{
	  SPI_SENT   =(uint8_t)0,
  SPI_NOT_SENT =(uint8_t)1, //= (bool)FALSE, /*false*/
 // SPI_SENT      //= (bool)TRUE

} SPI_SentStatusType;
/*------*/

/*Indicates SPI interface state*/
typedef enum
{
	SPI_RECEIVED,
  SPI_NOT_RECEIVED  // = (bool)FALSE, /*false*/
//  SPI_RECEIVED      //= (bool)TRUE

} SPI_ReceivingStatusType;
/*------*/

/*------------------------------------------------------------------------------
                          END Types definitions
 -----------------------------------------------------------------------------*/





/*******************************************************************************
                         Global variables Flags
 ******************************************************************************/

/*Indicates the driver state un/initialized */
extern SPI_InfStateType  gSPI_INF_STATE;

/*------------------------------------------------------------------------------
            END Global variables Flags
 -----------------------------------------------------------------------------*/





/*******************************************************************************
                           Functions prototypes
 ******************************************************************************/

/*------This is a list of functions provide for upper layer modules.----------*/

/*Initializes SPI Module.called by upper layer*/
void vSPI_InfInit(void);

/*sending one byte from SPI*/
SPI_SentStatusType bSPI_InfSyncTxByte(uint8_t u8SPIbyte);

/*transmitting buffer array from SPI*/
SPI_SentStatusType bSPI_InfSyncTxBuf(uint8_t* buffer_ptr, uint16_t buff_size);

/*receiving byte from SPI*/
SPI_ReceivingStatusType bSPI_InfSyncRX(uint8_t* u8ptrSPI);

/*receiving bytes from SPI*/
SPI_ReceivingStatusType bSPI_InfSyncRXBuf(uint8_t* buffer_ptr, uint16_t buff_size);

/*------------------------------------------------------------------------------
              End function prototypes
 -----------------------------------------------------------------------------*/








#endif /* SOFT_SPI_INTF_H_ */