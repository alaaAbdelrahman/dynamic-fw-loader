/*
* flash_intf.c
*
* Created: 29/04/2018 01:11:53 م
*  Author: reda.abdelhameid
*/ 
/**
* This file was run with Python script version 1.0.0
* @file    FLASH.c
* @brief
* @copyright
*
* THIS CODE IS PROPERTY OF ELSEWEDY ELECTROMETER COMPANY AND ITS LICENSEES. COPYING
* USING, OR MODIFYING THIS CODE BY ANY OTHER PARTY IS PROHIBITED.
* THIS CODE AND INFORMATION ARE PROVIDED AS IT IS WITHOUT WARRANTY OF ANY
* KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
* PARTICULAR PURPOSE.
*
* <h2><center>&copy; COPYRIGHT 2015 www.sewedy.com.eg</center></h2>
*
* <b>References</b>

* - Ref_Documents folder .
*
* <b>Edit History For File</b>

*  This section contains comments describing changes made to this file.

*  Notice that changes are listed in reverse chronological order.

* <table border>
* <tr>
*   <td><b> when </b></td>
*   <td><b> who </b></td>
*   <td><b> what, where, why </b></td>
* </tr>
* <tr>
*   <td> 21/3/2016 </td>
*   <td> Mohamed Hamed elsaidy </td>
*   <td>  Create the file</td>
* </tr>
* </table>

*/
/** @addtogroup HAL
*@{
*/
/** @addtogroup FLASH
*@{
*/
/*============================================================================================
*         Include Files
* ===========================================================================================*/
/*

#include "Types.h"*/
#include "config.h"

#if (MicroController == Micro_V94XX)
#include "V94XX_GPIO.h"
#else
#include "v85xx_gpio.h"
#endif

#include "GPIO_Interface.h"
#include "Flash.h"
//#include "self_test.h"
//#include "delay.h"
#include "string.h"
#include "basic_srv.h"
#include "soft_spi_intf.h"
#include "meter_init.h"
#ifdef TEST_FLASH
#include "display.h"
#include "EEPROM.h"
#include "FLASH_Addresses.h"
#include "SPI_Interface.h"

typedef enum
{
  FLASH_NO_ERROR                =(uint8_t)0x00,
  FLASH_ERR_ERASE_CHIP        	= (uint8_t)0x01,
  FLASH_ERR_ERASE_SECTOR      	= (uint8_t)0x02,
  FLASH_ERR_ERASE_BLOCK_32K     = (uint8_t)0x04,
  FLASH_ERR_ERASE_BLOCK_64K 	= (uint8_t)0x08,
  FLASH_ERR_READ    		= (uint8_t)0x10,
  FLASH_ERR_WRITE  		= (uint8_t)0x20 ,
  FLASH_ERR_COMPARE  		= (uint8_t)0x40 ,
  FLASH_ERR_INIT  		= (uint8_t)0x80 ,
} FLASH_ERROR_T;

uint8_t flash_error = 0;
uint8_t saved_arr[256] ;
uint32_t random_num = 0;
uint8_t cmd_act = 1;//0;
uint8_t testdataread[5000]={0} ;
uint8_t testdatawrite[5000];
uint8_t block_64_index=0;
uint8_t block_32_index=0;
uint8_t block_4_index=0;
uint16_t length_var=0;
uint8_t buff_ff[256];
uint8_t block_temp_no[4]={0,16,0,0};
#endif

#if  defined (HW_SPI)
#define FLASH_CS_LOW           	/*delay_cycles(100);*/Reset_bit_GPIO(HARD_SPI_CS_PORT, HARD_SPI_CS_PIN)/*;delay_cycles(100)*/
#define FLASH_CS_HIGH          	/*delay_cycles(100);*/Set_bit_GPIO(HARD_SPI_CS_PORT, HARD_SPI_CS_PIN)/*;delay_cycles(100)*/
#elif defined (SW_SPI)
#define FLASH_CS_LOW           	/*delay_cycles(100);*/Reset_bit_GPIO(SOFTSPI_CS_PORT, SOFTSPI_CS_PIN)/*;delay_cycles(100)*/
#define FLASH_CS_HIGH          	/*delay_cycles(100);*/Set_bit_GPIO(SOFTSPI_CS_PORT, SOFTSPI_CS_PIN)/*;delay_cycles(100)*/
#endif  //  HW_SPI

#define FLASH_TIMEOUT			(uint32_t)1600
#define FLASH_BLOCK_TIMEOUT		(uint32_t)4000
#define FAST_FLASH_TIMEOUT		6
/*============================================================================================
*         Local new types declaration
* ===========================================================================================*/
typedef enum{
  FLASH_READ_OP   =(uint8_t)0,
  FLASH_WRITE_OP   =(uint8_t)1,
}READWRITE_T;

typedef struct
{
  uint16_t datalength;
  uint16_t data_index;
  uint8_t temp_address[3];
  uint8_t*dataPtr;
}datainformation_t;
/*============================================================================================
*         Loacal Variables declaration
* ===========================================================================================*/
static uint8_t tx_instruction_frame[260];      //frame transmitted to flash
static uint8_t flash_mode = FLASH_NORMAL_MODE;
//CHIP_ID_t tenumspi_cfgport_flash = NPCS0 ;//SPI_UCB0;
/* TstSPI_cfgParameters tstspi_cfgparameters_flash;
*/
//TstSPI_cfgParameters* tstspi_cfgparameters_flash_ptr;
//tstspi_cfgparameters_flash_ptr = &tstspi_cfgparameters_flash;
/*============================================================================================
*         Local functions declaration
* ===========================================================================================*/
uint16_t FLASH_Form_Instruction(uint8_t Instruction , uint8_t * Address , uint8_t * Data_Array , uint16_t Data_length);
uint8_t FLASH_Address_Avaliable(uint8_t * Address);
uint8_t FLASH_Read_Status_Register(uint8_t Register_Number, uint8_t* status_register_value);
uint8_t FLASH_Avaliable(void);
uint8_t FLASH_Enable_Write(void);
uint8_t FLASH_Handle_ReadWriteOperantion(datainformation_t* datainformation,uint16_t write_length,READWRITE_T readorwite,uint8_t NotStartPage);
uint8_t FlASH_SelectOperation(uint8_t* Flash_Address , uint8_t * dataPtr , uint16_t datalength, READWRITE_T WR_RD);
/*============================================================================================
* Function Name  FLASH_Form_Instruction
* ===========================================================================================*/
/*!
* @brief     THis function is to form the request frame format that will be send to the flash memory
* @retval    Request frame length
* @param     Instruction set value
Address in the Flash memory
pointer to Data_Array
Data_length
*/
uint16_t FLASH_Form_Instruction(uint8_t Instruction , uint8_t * Address , uint8_t * Data_Array , uint16_t Data_length)
{
  /*could you please Add your code here with doxgen format */
  //------------------request frame format--------------------------------------
  //Instruction | Address (3byte) for write and read data | Data (for write data)
  uint16_t i = 0 ;
  tx_instruction_frame[i] = Instruction;
  i = 1 ;
  if(Address != '\0')
  {
    //tx_instruction_frame[i++] =  Address[3];
    tx_instruction_frame[i++] =  Address[2];
    tx_instruction_frame[i++] =  Address[1];
    tx_instruction_frame[i++] =  Address[0];
  }
  if(Data_length != 0)
  {
    memcpy(&tx_instruction_frame[i] , Data_Array , Data_length);
    i += Data_length;
  }
  return i ;
}
/*============================================================================================
* Function Name  FLASH_Address_Avaliable
* ===========================================================================================*/
/*!
* @brief     THis function is test if the address is avaliable in the flash memory
* @retval    the state of the address if it avaliable or not
* @param     Address in the Flash memory
*/
uint8_t FLASH_Address_Avaliable(uint8_t * Address)
{
  //block#0 start address = 0x000000h and block#127 end address=0x7fffffh
  uint8_t return_status = ADDRESS_AVALIABLE;
  /*size of ext flash consist of 32 sector ,each sector consist of 16 block of 4096Kbyte where each block consist of 16 page.
  finally page consist of 256.
  address[0] represent number of byte per page
  address[1] represent number of page per sector
  address[2] represent number of sector per flash */
  if(Address[2] > 0x3F)
  {
    return_status = ADDRESS_UNAVALIABLE;
  }
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_Read_Status_Register
* ===========================================================================================*/
/*!
* @brief     THis function is used to read status register 1 or status register 2
* @retval    uint8_t TRUE in success, FALSE other
* @param     Register_Number
* @param   The register value
*/
uint8_t FLASH_Read_Status_Register(uint8_t Register_Number, uint8_t* status_register_value)
{
  uint16_t length = 0 ;
  uint8_t return_status = FLASH_READ_ERROR;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD) || defined(EM130_BOARD) || defined(EM210_BOARD))  
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  if(Register_Number == 1)
  {
    length =  FLASH_Form_Instruction(READ_STATUS_REGISTER_1 , '\0' , '\0' , 0);
  }
  else if(Register_Number == 2)
  {
    length =  FLASH_Form_Instruction(READ_STATUS_REGISTER_2 , '\0' , '\0' , 0);
  }
  //low chip select
  FLASH_CS_LOW;
  #if defined (HW_SPI)
  if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame,  length) == NO_ERROR)
  {
    if ((uint8_t)SPI1_rx_frame(status_register_value,1) == NO_ERROR)
    {
  #elif defined (SW_SPI)
     if((uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length) == NO_ERROR)
  {
    if( (uint8_t)bSPI_InfSyncRX(status_register_value)== NO_ERROR)
    {
#endif
      FLASH_CS_HIGH;
      return_status = NO_ERROR;
    }
  }
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
    S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_Avaliable
* ===========================================================================================*/
/*!
* @brief     THis function is used to check that flash is busy or not
* @retval    status that flash is busy or not
* @param     none
*/
uint8_t FLASH_Avaliable(void)
{
  uint8_t register_data = 0 ;
  uint8_t return_status = FLASH_BUSY;
  return_status = FLASH_Read_Status_Register(STATUS_REGISTER_1 , &register_data) ;
  if(return_status == NO_ERROR)
  {
    if((register_data & 1) == 0)
    {
      return_status = FLASH_NOT_BUSY;
    }
    else
    {
      return_status = FLASH_BUSY;
    }
  }
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_Enable_Write
* ===========================================================================================*/
/*!
* @brief     THis function is used to Enable the write process in the flash
* @retval    uint8_t TRUE in success, FALSE other
* @param     none
*/
uint8_t FLASH_Enable_Write(void)
{
  uint32_t counter = 0;
  uint16_t length = 0;
  uint8_t return_status = FLASH_WRITE_ERROR;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FLASH_TIMEOUT))
  {
    // //vDelay_SW(1) ;
  }
  if(counter < FLASH_TIMEOUT)
  {
    FLASH_CS_LOW;
    length = FLASH_Form_Instruction(WRITE_ENABLE , '\0' , '\0' , 0);
    #if defined (HW_SPI)
    if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame ,  length) == NO_ERROR)
    #elif defined (SW_SPI)
    if((uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length) == NO_ERROR)
    #endif
    {
      FLASH_CS_HIGH;
      return_status = NO_ERROR;
    }
  }
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
    S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_Enable_Reset
* ===========================================================================================*/
/*!
* @brief     THis function is used to Enable the reset
* @retval    uint8_t TRUE in success, FALSE other
* @param     none
*/
uint8_t FLASH_Read_Reset(void)
{
  uint16_t length = 0;
  uint8_t return_status = FLASH_RESET_ERROR;
  uint32_t counter = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FLASH_TIMEOUT))
  {
    // //vDelay_SW(1);
  }
  if(counter < FLASH_TIMEOUT)
  {
    FLASH_CS_LOW;
    length = FLASH_Form_Instruction(RESET_CMD , '\0' , '\0' , 0);
    #if defined (HW_SPI)
    if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame ,  length) == NO_ERROR)
    #elif defined (SW_SPI)
    if((uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length) == NO_ERROR)
    #endif
    {
      FLASH_CS_HIGH;
      return_status = NO_ERROR;
    }
  }
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
    S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_Write_Status_Register
* ===========================================================================================*/
/*!
* @brief     THis function is used to write the two status register in the flash
* @retval    uint8_t TRUE in success, FALSE other
* @param     Register_1 value
Register_2 value
*/
uint8_t FLASH_Write_Status_Register(uint8_t Register_1 , uint8_t Register_2)
{
  uint8_t data[2] ;
  uint16_t length = 0;
  uint8_t return_status = FLASH_WRITE_ERROR;
  uint32_t counter = 0;
  data[0] = Register_1;
  data[1] = Register_2;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  if(FLASH_Enable_Write() == NO_ERROR)
  {
    while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FLASH_TIMEOUT))
    {
      ////vDelay_SW(1);
    }
    if(counter < FLASH_TIMEOUT)
    {
      FLASH_CS_LOW;
      length = FLASH_Form_Instruction(WRITE_STATUS_REGISTER , '\0' ,  data , 2);
    #if defined (HW_SPI)
    if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame ,  length) == NO_ERROR)
    #elif defined (SW_SPI)
    if((uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length) == NO_ERROR)
    #endif
      {
        FLASH_CS_HIGH;
        return_status = NO_ERROR;
      }
    }
  }
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
    S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}

/*============================================================================================
*         Global Functions
* ===========================================================================================*/
/*============================================================================================
* Function Name  FLASH_gInit
* ===========================================================================================*/
/*!
* @brief     This function is responsible for initializing the FLASH
* @retval    uint8_t TRUE in success, FALSE else
* @param     none
*/
uint8_t FLASH_gInit(void)
{	
  //	device_t deviceobis={NPCS0,BIT_8,LOW_ClockPhase} ;
  /*could you please Add your code here with doxgen format */
  uint8_t return_status = FLASH_INIT_ERROR;
  //spi_master_init(SPI0,&deviceobis,SPIBAUDRATE ) ;
    #if defined (SW_SPI)
    vSPI_InfInit();
    #elif defined (HW_SPI)
    S_SPI1_INIT();
    #endif
  if(FLASH_Write_Status_Register(0x00, 0x00) == NO_ERROR)
  {
    return_status = NO_ERROR;
    //eeprom_load_errors_register &= ~COMM_ERR_LOAD_FLAG;
  }
  else
  {
    //eeprom_load_errors_register |= COMM_ERR_LOAD_FLAG;
  }
  
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_gDeinit
* ===========================================================================================*/
/*!
* @brief   This function is responsible for deinitializing the FLASH
* @retval  uint8_t TRUE in success, FALSE else
* @param   none
*/
uint8_t FLASH_gDeinit(void)
{
  return 0;
}
/*============================================================================================
* Function Name  FLASH_gWritePage
* ===========================================================================================*/
/*!
* @brief     This function is used to write Data in page size in flash
* @retval    uint8_t TRUE in success, FALSE other
* @param     Flash_Address
Data array
Data_length
*/
uint8_t FLASH_gWritePage(uint8_t* Flash_Address , uint8_t * Data , uint16_t Data_length)
{
  uint16_t length;
  uint8_t return_status = FLASH_WRITE_ERROR;
  uint32_t counter = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  if(FLASH_Address_Avaliable(Flash_Address) == ADDRESS_AVALIABLE)
  {
    if(FLASH_Enable_Write() == NO_ERROR)
    {
      while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FLASH_TIMEOUT))
      {
        // //vDelay_SW(1);
      }
      if(counter < FLASH_TIMEOUT)
      {
        FLASH_CS_LOW;
        length = FLASH_Form_Instruction(WRITE_PAGE , Flash_Address , Data , Data_length);
    #if defined (HW_SPI)
    if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame ,  length) == NO_ERROR)
    #elif defined (SW_SPI)
    if((uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length) == NO_ERROR)
    #endif
        {
          FLASH_CS_HIGH;
          return_status = NO_ERROR;
        }		  
      }
    }
  }
  else
    return_status = ADDRESS_UNAVALIABLE;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
    S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}

/*============================================================================================
* Function Name  FLASH_gFastWritePage
* ===========================================================================================*/
/*!
* @brief     This function is used to write Data in page size in flash
* @retval    uint8_t TRUE in success, FALSE other
* @param     Address
Data array
Data_length
*/
uint8_t FLASH_gFastWritePage(uint8_t* Flash_Address , uint8_t * Data , uint16_t Data_length)
{
  uint16_t length;
  uint8_t return_status = FLASH_WRITE_ERROR;
  volatile uint32_t counter = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  if(FLASH_Address_Avaliable(Flash_Address) == ADDRESS_AVALIABLE)
  {
    if(FLASH_Enable_Write() == NO_ERROR)
    {
      while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FAST_FLASH_TIMEOUT))
      {
        // //vDelay_SW(1);
      }
      if(counter < FLASH_TIMEOUT)
      {
        FLASH_CS_LOW;
        length = FLASH_Form_Instruction(WRITE_PAGE , Flash_Address , Data , Data_length);
    #if defined (HW_SPI)
    if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame ,  length) == NO_ERROR)
    #elif defined (SW_SPI)
    if((uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length) == NO_ERROR)
    #endif
        {
          FLASH_CS_HIGH;
          return_status = NO_ERROR;
        }
      }
    }
  }
  else
    return_status = ADDRESS_UNAVALIABLE;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}
/*
Flash_address[0] represent number of byte per page
Flash_address[1] Bit0 --> Bit3 represent Page Index per sector  16,and Bit4-->Bit7 represent sector Number from 0 to  15
Flash_address[2] represent sector Number 16 to last sector in flash with 4096 sectors
*/
static uint8_t Address_Increment_byPage(uint8_t * Address)
{
  uint8_t status = ADDRESS_AVALIABLE;
  
  if(Address[1] == 0xff)
  {
    Address[1] = 0;
    Address[2]++;
    if(Address[2] > 0x3f)
    {
      status = ADDRESS_UNAVALIABLE;
    }
  }
  else
  {
    Address[1]++;
  }
  return status;
}
/*============================================================================================
* Function Name  FLASH_Handle_ReadWriteOperantion
* ===========================================================================================*/
/*!
* @brief     This function is used to handle a read or a write operation from the flash
* @retval    uint8_t TRUE in success, FALSE else
* @param     Flash_Address The data address
Flash_address[0] represent number of byte per page
Flash_address[1] Bit0 --> Bit3 represent Page Index per sector  16,and Bit4-->Bit7 represent sector Number from 0 to  15
Flash_address[2] represent sector Number 16 to last sector in flash with 4096 sectors
* @param     dataPtr
* @param     datalength
* @param     WR_RD is type of operation at READWRITE_T
* @param    NotStartPage is start of page or not ,if NotStartPage =0 is mean write or read operation is start from the start of page else other 
*/
uint8_t FLASH_Handle_ReadWriteOperantion(datainformation_t* datainformation,uint16_t write_length , READWRITE_T readorwite,uint8_t NotStartPage)
{
  uint8_t status_write = NO_ERROR;
  switch(readorwite)
  {
  case FLASH_WRITE_OP :
    if(FLASH_gWritePage(datainformation->temp_address , &datainformation->dataPtr[datainformation->data_index] , write_length)/*==0*/)
    {
      status_write =FLASH_WRITE_ERROR;
    }
    break;
  case FLASH_READ_OP:
    if(FLASH_gReadPage(datainformation->temp_address , &datainformation->dataPtr[datainformation->data_index] , write_length)/*==0*/)
    {
      status_write =FLASH_READ_ERROR;
    }
    break;
  default:
    status_write = FLASH_ERROR_OP;
    return status_write;
  }
  //increment by page
  datainformation->datalength -= write_length;
  datainformation->data_index += write_length;
  if(NotStartPage)
  {
    datainformation->temp_address[0] = 0;
  }
  if(Address_Increment_byPage(datainformation->temp_address) == ADDRESS_UNAVALIABLE)
  {
    status_write = ADDRESS_UNAVALIABLE;
  }  
  return status_write;
}
/*============================================================================================
* Function Name  FLASH_gWrite
* ===========================================================================================*/
/*!
* @brief     This function is used to write Data With any size in flash
* @retval    uint8_t TRUE in success, FALSE other
* @param     Flash_Address
Flash_address[0] represent number of byte per page
Flash_address[1] Bit0 --> Bit3 represent Page Index per sector  16,and Bit4-->Bit7 represent sector Number from 0 to  15
Flash_address[2] represent sector Number 16 to last sector in flash with 4096 sectors
* @param     dataPtr
* @param     datalength
*/
uint8_t FLASH_gWrite(uint8_t* Flash_Address, uint8_t* dataPtr, uint16_t datalength)
{
  uint8_t status_write =NO_ERROR;
  status_write = FlASH_SelectOperation( Flash_Address ,  dataPtr ,  datalength, FLASH_WRITE_OP);
  return status_write;
}
/*============================================================================================
* Function Name  FLASH_gWrite
* ===========================================================================================*/
/*!
* @brief   This function is used to write Data With any size in flash
* @retval  uint8_t TRUE in success, FALSE other
* @param   byte_index : 0 to 4095
start_page_index : 0 to 15
sector_number : 0 to 30
* @param     dataPtr
* @param     datalength
*/
uint8_t FLASH_gWrite_ByIndex ( uint16_t byte_index, uint16_t start_page_index, uint16_t sector_number, uint8_t* dataPtr, uint16_t datalength)
{
  TU08 addr[3]={0};
  TU16 temp_byte_index = byte_index%FLASH_PAGE_SIZE;
  TU08 status = false ;
  
  if (byte_index >=FLASH_SECTOR_SIZE || start_page_index>=NUM_PAGES_PER_SECTOR ||sector_number>=MAX_NUM_SECTORS)
    return 0;
  if (byte_index >= FLASH_PAGE_SIZE)
  {
    start_page_index = start_page_index + byte_index/FLASH_PAGE_SIZE ;
    if (start_page_index >= NUM_PAGES_PER_SECTOR)
    {
      return ADDRESS_UNAVALIABLE;
    }
  }
  
  addr[0] = (TU08)temp_byte_index;
  addr[1] = (TU08)start_page_index&0x0f |((TU08)sector_number<<4) & 0xf0;
  addr[2] = (TU08)(sector_number>>4);
  
  status = FLASH_gWrite(addr, dataPtr, (TU16)datalength); //save the new key in falsh for optical one client avalible for now
  return status;
}
/*============================================================================================
* Function Name  FLASH_gReadPage
* ===========================================================================================*/
/*!
* @brief     This function is used to read a data in page size from the flash
* @retval    uint8_t TRUE in success, FALSE else
* @param     Flash_Address The data address
Flash_address[0] represent number of byte per page
Flash_address[1] Bit0 --> Bit3 represent Page Index per sector  16,and Bit4-->Bit7 represent sector Number from 0 to  15
Flash_address[2] represent sector Number 16 to last sector in flash with 4096 sectors
* @param     dataPtr
* @param     datalength
*/
uint8_t FLASH_gReadPage (uint8_t* Flash_Address, uint8_t* dataPtr, uint16_t datalength)
{
  /*could you please Add your code here with doxgen format */
  uint16_t length = 0 ;
  uint8_t return_status = FLASH_READ_ERROR;
  uint32_t counter = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  if(FLASH_Address_Avaliable(Flash_Address) == ADDRESS_AVALIABLE)
  {
    while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FLASH_TIMEOUT))
    {
      // //vDelay_SW(1);
    }
    if(counter < FLASH_TIMEOUT)
    {
      FLASH_CS_LOW;
      length = FLASH_Form_Instruction(READ_DATA , Flash_Address , '\0' , 0);
      #if defined (HW_SPI)
      if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame, length) == NO_ERROR)
      {
        if (SPI1_rx_frame(dataPtr, datalength) == SPI_RECEIVED)
        {
      #elif defined (SW_SPI)
      if((uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length) == NO_ERROR)
      {        
        if( bSPI_InfSyncRXBuf(dataPtr ,  datalength)==SPI_RECEIVED)
        {
      #endif
          FLASH_CS_HIGH;
          return_status = NO_ERROR;
        }
      }
    }
  }
  else
    return_status = ADDRESS_UNAVALIABLE;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_gFastReadPage
* ===========================================================================================*/
/*!
* @brief     This function is used to read a data in page size from the flash
* @retval    uint8_t TRUE in success, FALSE else
* @param     Flash_Address The data address
Flash_address[0] represent number of byte per page
Flash_address[1] Bit0 --> Bit3 represent Page Index per sector  16,and Bit4-->Bit7 represent sector Number from 0 to  15
Flash_address[2] represent sector Number 16 to last sector in flash with 4096 sectors
* @param     dataPtr
* @param     datalength
*/
uint8_t FLASH_gFastReadPage(uint8_t* Flash_Address, uint8_t* dataPtr, uint16_t datalength)
{
  /*could you please Add your code here with doxgen format */
  uint16_t length = 0 ;
  uint8_t return_status = FLASH_READ_ERROR;
  uint8_t dummy_data = 0;
  uint32_t counter = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  if(FLASH_Address_Avaliable(Flash_Address) == ADDRESS_AVALIABLE)
  {
    while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FAST_FLASH_TIMEOUT))
    {
      //vDelay_SW(1);
    }
    if(counter < FLASH_TIMEOUT)
    {
      FLASH_CS_LOW;
      length = FLASH_Form_Instruction(FAST_READ_DATA , Flash_Address , &dummy_data , 1);
     #if defined (HW_SPI)
     if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame, length) == NO_ERROR)
      {
        if (SPI1_rx_frame(dataPtr, datalength) == SPI_RECEIVED)
        {
      #elif defined (SW_SPI)
      if((uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length) == NO_ERROR)
      {        
        if( bSPI_InfSyncRXBuf(dataPtr ,  datalength)==SPI_RECEIVED)
        {
      #endif
          FLASH_CS_HIGH;
          return_status = NO_ERROR;
        }
      }
    }
  }
  else
    return_status = ADDRESS_UNAVALIABLE;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}
/*============================================================================================
* Function Name  FlASH_SelectOperation
* ===========================================================================================*/
/*!
* @brief     This function is used to read or write operation from the flash
* @retval    uint8_t TRUE in success, FALSE else
* @param     Flash_Address The data address
Flash_address[0] represent number of byte per page
Flash_address[1] Bit0 --> Bit3 represent Page Index per sector  16,and Bit4-->Bit7 represent sector Number from 0 to  15
Flash_address[2] represent sector Number 16 to last sector in flash with 4096 sectors
* @param     dataPtr
* @param     datalength
* @param     WR_RD is type of operation at READWRITE_T
*/
uint8_t FlASH_SelectOperation(uint8_t* Flash_Address , uint8_t * dataPtr , uint16_t datalength, READWRITE_T WR_RD)
{
  /*could you please Add your code here with doxgen format */
  uint8_t status_write=NO_ERROR;
  static datainformation_t datainformation;
  
  memcpy(datainformation.temp_address ,Flash_Address ,sizeof(datainformation.temp_address));
  datainformation.datalength=datalength;
  datainformation.dataPtr =dataPtr;
  datainformation.data_index=0;
  uint16_t Fetch_location=0;
  uint8_t NotStartPage=0;
  //if(FLASH_gInit()== 0)
  {
    while((datainformation.datalength >= FLASH_PAGE_SIZE)  )       //if data length is greater than a page size
    {
      if(datainformation.temp_address[0] == 0)  
      {
        //if address is the start of flash sector
        Fetch_location = FLASH_PAGE_SIZE;
        NotStartPage = 0;
      }
      else
      {
        Fetch_location = FLASH_PAGE_SIZE - datainformation.temp_address[0];
        NotStartPage = 1;
      }
      status_write = FLASH_Handle_ReadWriteOperantion(&datainformation, Fetch_location , WR_RD , NotStartPage);
    }
    if(datainformation.datalength > 0)                 //if the remaining data length is less than page size
    {
      if((datainformation.datalength > (FLASH_PAGE_SIZE - datainformation.temp_address[0])) && (datainformation.temp_address[0] != 0))
      {
        //if address is not start of page and data length is greater than the remaining page size
        //write to fill the remaining of page
        NotStartPage = 1;
        status_write = FLASH_Handle_ReadWriteOperantion(&datainformation, (FLASH_PAGE_SIZE - datainformation.temp_address[0]),WR_RD,NotStartPage);
      }
      //if data length is less than current page
      if(WR_RD)
      {
        if(FLASH_gWritePage(datainformation.temp_address, &datainformation.dataPtr[datainformation.data_index], datainformation.datalength))
        {
          status_write =FLASH_WRITE_ERROR;
        }
      }
      else
      {
        if(FLASH_gReadPage (datainformation.temp_address, &datainformation.dataPtr[datainformation.data_index], datainformation.datalength))
        {
          status_write =FLASH_READ_ERROR;
        }
      }
    }
  }
  return status_write;
}
/*============================================================================================
* Function Name  FLASH_gRead
* ===========================================================================================*/
/*!
* @brief     This function is used to read a data With any Size from the flash
* @retval    uint8_t TRUE in success, FALSE else
* @param     Flash_Address The data address
Flash_address[0] represent number of byte per page
Flash_address[1] Bit0 --> Bit3 represent Page Index per sector  16,and Bit4-->Bit7 represent sector Number from 0 to  15
Flash_address[2] represent sector Number 16 to last sector in flash with 4096 sectors
* @param     dataPtr dataPtr
* @param     datalength
*/
uint8_t FLASH_gRead(uint8_t* Flash_Address , uint8_t * dataPtr , uint16_t datalength)
{
  uint8_t status_write =NO_ERROR;
  status_write = FlASH_SelectOperation( Flash_Address ,  dataPtr ,  datalength, FLASH_READ_OP);
  return status_write;
}
/*============================================================================================
* Function Name  FLASH_gWrite
* ===========================================================================================*/
/*!
* @brief   This function is used to read Data With any size in flash
* @retval  uint8_t TRUE in success, FALSE other
* @param   byte_index : 0 to 4095
start_page_index : 0 to 16
sector_number : 0 to 4095
* @param     dataPtr
* @param     datalength
*/
uint8_t FLASH_gRead_ByIndex ( uint16_t byte_index, uint8_t start_page_index, uint16_t sector_number, uint8_t* dataPtr, uint16_t datalength)
{
  TU08 addr[3]         = {0};
  TU16 temp_byte_index = byte_index%FLASH_PAGE_SIZE;
  TU08 status          = false ;
  
  if (byte_index >=FLASH_SECTOR_SIZE || start_page_index>=NUM_PAGES_PER_SECTOR ||sector_number>=MAX_NUM_SECTORS)
    return status;
  if (byte_index >= FLASH_PAGE_SIZE)
  {
    start_page_index = start_page_index + byte_index/FLASH_PAGE_SIZE ;
    if (start_page_index >= NUM_PAGES_PER_SECTOR)
    {
      start_page_index = start_page_index%NUM_PAGES_PER_SECTOR;
      sector_number ++;//check for sector num
      //todo make pointer to variable to tell that anther sector will be wrote in.
    }
  }
  
  addr[0] = (TU08)temp_byte_index;
  addr[1] = (TU08)start_page_index&0x0f |((TU08)sector_number<<4) & 0xf0;
  addr[2] = (TU08)(sector_number>>4);
  
  status = FLASH_gRead(addr, dataPtr, (TU16)datalength); //save the new key in falsh for optical one client avalible for now

  return status;
}

/*============================================================================================
* Function Name  FLASH_gWrite
* ===========================================================================================*/
/*!
* @brief   This function is used to erase sector by sector number
* @retval  uint8_t TRUE in success, FALSE other
* @param     sector_number : 0 to 4095
*/
uint8_t FLASH_gEraseSector_ByIndex (uint16_t sector_number,uint8_t delay)
{
  TU08 addr[4]         = {0};
  TU08 status          = false ;
  
  if (sector_number>=MAX_NUM_SECTORS)
    return status;
  
  addr[0] = 0;
  addr[1] = ((TU08)sector_number<<4) & 0xf0;
  addr[2] = (TU08)(sector_number>>4);
  addr[3] = 0;
  
  status = FLASH_gSectorErase(addr); //save the new key in falsh for optical one client avalible for now
  if(delay > 0)   // must put delay 80 ms according datasheet
    Delay_ms(delay);
  return status;
}

/*============================================================================================
* Function Name  FLASH_gSectorErase
* ===========================================================================================*/
/*!
* @brief     This function is used to erase the FLASH sector
* @param     Flash_Address
* @retval    uint8_t TRUE in success, FALSE else
*
*/
uint8_t FLASH_gSectorErase(uint8_t* Flash_Address)
{
  /*could you please Add your code here with doxgen format */
  uint16_t length ;
  uint8_t return_status = FLASH_SECTOR_ERASE_ERROR;
  uint32_t counter = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif

memset(tx_instruction_frame,0,sizeof(tx_instruction_frame));

  if (FLASH_Address_Avaliable(Flash_Address) == ADDRESS_AVALIABLE)
  {
    if(FLASH_Enable_Write() == NO_ERROR)
    {
      while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FLASH_TIMEOUT))
      {
        //vDelay_SW(1);
      }
      if(counter < FLASH_TIMEOUT)
      {
        FLASH_CS_LOW;
        length = FLASH_Form_Instruction(ERASE_SECTOR , Flash_Address , '\0' , 0);
        #if defined (HW_SPI)
        if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame, length) == NO_ERROR)
        #elif defined (SW_SPI)
        if( (uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length)== NO_ERROR)
        #endif
        {
          FLASH_CS_HIGH;
          return_status = NO_ERROR;
        }
      }
    }
  }
  else
    return_status = ADDRESS_UNAVALIABLE;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD)  || defined(EM210_BOARD))
  S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_gFastSectorErase
* ===========================================================================================*/
/*!
* @brief     This function is used to erase the FLASH sector
* @param     Flash_Address
* @retval    uint8_t TRUE in success, FALSE else
*
*/
uint8_t FLASH_gFastSectorErase(uint8_t* Flash_Address)
{
  /*could you please Add your code here with doxgen format */
  uint16_t length ;
  uint8_t return_status = FLASH_SECTOR_ERASE_ERROR;
  volatile uint32_t counter = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  if(FLASH_Address_Avaliable(Flash_Address) == ADDRESS_AVALIABLE)
  {
    if(FLASH_Enable_Write() == NO_ERROR)
    {
      while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FAST_FLASH_TIMEOUT))
      {
        //vDelay_SW(1);
      }
      if(counter < FLASH_TIMEOUT)
      {
        FLASH_CS_LOW;
        length = FLASH_Form_Instruction(ERASE_SECTOR , Flash_Address , '\0' , 0);
        #if defined (HW_SPI)
        if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame, length) == NO_ERROR)
        #elif defined (SW_SPI)
        if( (uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length)== NO_ERROR)
        #endif
        {
          FLASH_CS_HIGH;
          return_status = NO_ERROR;
        }
      }
    }
  }
  else
    return_status = ADDRESS_UNAVALIABLE;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}

/*============================================================================================
* Function Name  FLASH_gBlockErase
* ===========================================================================================*/
/*!
* @brief     This function is used to erase the FLASH block
* @param     Address
* @retval    uint8_t TRUE in success, FALSE else
*
*/
uint8_t FLASH_gBlockErase(uint8_t* Flash_Address)
{
  /*could you please Add your code here with doxgen format */
  uint16_t length ;
  uint8_t return_status = FLASH_BLOCK_ERASE_ERROR;
  uint32_t counter = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
#if defined(HW_SPI)
memset(tx_instruction_frame,0,sizeof(tx_instruction_frame));
#endif
  if (FLASH_Address_Avaliable(Flash_Address) == ADDRESS_AVALIABLE)
  {
    if(FLASH_Enable_Write() == NO_ERROR)
    {
      while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FLASH_BLOCK_TIMEOUT))
      {
        //vDelay_SW(1);
      }
      if(counter < FLASH_BLOCK_TIMEOUT)
      {
        FLASH_CS_LOW;
        length = FLASH_Form_Instruction(ERASE_BLOCK , Flash_Address , '\0' , 0);
        #if defined (HW_SPI)
        if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame, length) == NO_ERROR)
        #elif defined (SW_SPI)
        if( (uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length)== NO_ERROR)
        #endif
        {
          FLASH_CS_HIGH;
          return_status = NO_ERROR;
        }
      }
    }
  }
  else
    return_status = ADDRESS_UNAVALIABLE;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_gChipErase
* ===========================================================================================*/
/*!
* @brief     This function is used to erase all flash
* @param     none
* @retval    uint8_t TRUE in success, FALSE else
*
*/
//recommedation don't use this function or you should wait 100 sec after use this function to deal with flash
uint8_t FLASH_gChipErase(void)
{
  uint16_t length ;
  uint8_t return_status = FLASH_CHIP_ERASE_ERROR;
  uint32_t counter = 0;
  uint8_t i = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  if(FLASH_Enable_Write() == NO_ERROR)
  {
    while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FLASH_TIMEOUT))
    {
      //vDelay_SW(1);
    }
    if(counter < FLASH_TIMEOUT)
    {
      FLASH_CS_LOW;
      length = FLASH_Form_Instruction(ERASE_CHIP , '\0' , '\0' , 0);
      #if defined (HW_SPI)
        if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame, length) == NO_ERROR)
      #elif defined (SW_SPI)
        if( (uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length)== NO_ERROR)
      #endif
      {
        FLASH_CS_HIGH;
        for (i = 0 ; i < 30 ; i++)
        {
          vDelay_SW(1000000);
        }
        return_status = NO_ERROR;
      }
    }
  }
  else
    return_status = FLASH_CHIP_ERASE_ERROR;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}

/*============================================================================================
* Function Name  FLASH_gPunInLPM
* ===========================================================================================*/
/*!
* @brief     This function is uded to put the FLASH in a low power mode
* @retval    uint8_t TRUE in success, FALSE else
*/
uint8_t FLASH_gPunInLPM(void)
{
  /*could you please Add your code here with doxgen format */
  uint16_t length ;
  uint8_t return_status = FLASH_ENTER_LPM_ERROR;
  uint32_t counter = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FLASH_TIMEOUT))
  {
    //vDelay_SW(1);
  }
  if(counter < FLASH_TIMEOUT)
  {
    FLASH_CS_LOW;
    length = FLASH_Form_Instruction(POWER_DOWN , '\0' , '\0' , 0);
    #if defined (HW_SPI)
    if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame, length) == NO_ERROR)
    #elif defined (SW_SPI)
    if( (uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length)== NO_ERROR)
    #endif
    {
      FLASH_CS_HIGH;
      flash_mode = FLASH_LPM;
      return_status = NO_ERROR;
    }
  }
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_gReleaseLPM
* ===========================================================================================*/
/*!
* @brief     This function is used to return the FLASH status to normal mode
* @retval    uint8_t TRUE in success, FALSE else
*/
uint8_t FLASH_gReleaseLPM(void)
{
  /*could you please Add your code here with doxgen format */
  uint16_t length ;
  uint8_t return_status = FLASH_RELEASE_LPM_ERROR;
  uint32_t counter = 0;
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  while((FLASH_Avaliable() != FLASH_NOT_BUSY) && (counter++ < FLASH_TIMEOUT))
  {
    //vDelay_SW(1);
  }
  if(counter < FLASH_TIMEOUT)
  {
    FLASH_CS_LOW;
    length = FLASH_Form_Instruction(RELEASE_POWER_DOWN , '\0' , '\0' , 0);
    #if defined (HW_SPI)
    if ((uint8_t)S_SPI1_TX_FRAME(tx_instruction_frame, length) == NO_ERROR)
    #elif defined (SW_SPI)
    if( (uint8_t)bSPI_InfSyncTxBuf(tx_instruction_frame ,  length)== NO_ERROR)
    #endif
    {
      FLASH_CS_HIGH;
      flash_mode = FLASH_NORMAL_MODE;
      return_status = NO_ERROR;
    }
  }
#if (defined(EM122U_BOARD) || defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM130_BOARD) || defined(EM210_BOARD))
  S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
#endif
  return return_status;
}
/*============================================================================================
* Function Name  FLASH_gGetStatus
* ===========================================================================================*/
/*!
* @brief     This function is used to return the FLASH status, IN_POWER_MODE or in LOW_POWER_MODE
* @retval    uint8_t Get the FLASH status IN_POWER_MODE or in LOW_POWER_MODE
*/
uint8_t FLASH_gGetStatus(void)
{
  /*could you please Add your code here with doxgen format */
  return flash_mode;
}

void testcase1()
{
uint8_t testdatawrite [25];
testdatawrite[0]=10;
testdatawrite[1]=20;
testdatawrite[2]=30;
testdatawrite[3]=40;
testdatawrite[4]=50;
testdatawrite[5]=60;
testdatawrite[6]=70;
testdatawrite[7]=80;
testdatawrite[8]=90;
uint8_t testaddwrite1 [4] = {
0,0,0,0
    };
uint8_t testaddwrite2 [4] = {
250,0,0,0
      };
uint8_t testaddwrite3 [4] = {
30,0,0,0
        };
uint8_t testaddread [4] = {
250,0,0,0
      };
uint8_t testdataread [20];
if(FLASH_gInit()== 0)
{
// if(FLASH_gWritePage(testadd , testdatawrite , 10) == 0)
if(FLASH_gSectorErase(testaddwrite1)==0)
{
FLASH_gReadPage (testaddwrite1, testdataread, 25);
if(FLASH_gWritePage(testaddwrite3 , testdatawrite , 20) == 0)
{
FLASH_gReadPage (testaddwrite3, testdataread, 25);
FLASH_gWritePage(testaddwrite2 , testdatawrite , 20);
FLASH_gReadPage (testaddwrite2, testdataread, 25);
FLASH_gReadPage (testaddwrite1, testdataread, 25);
        }

      }
    }
}
/*
void testcase2()
{
uint8_t testadd [4] = {
0,0,0,0
    };
uint8_t testdataread [400];
uint8_t testdatawrite [400]={
0,0,0,0,0,0,0,0,0,0,
2,3,4,5,6,7,8,9,0,1,
3,4,5,6,7,8,9,0,1,2,
4,5,6,7,8,9,0,1,2,3,
5,6,7,8,9,0,1,2,3,4,
6,7,8,9,0,1,2,3,4,5,
7,8,9,0,1,2,3,4,5,6,
8,9,0,1,2,3,4,5,6,7,
9,0,1,2,3,4,5,6,7,8,
0,1,2,3,4,5,6,7,8,9,

0 ,11,22,33,44,55,66,77,88,99,
11,22,33,44,55,66,77,88,99,0 ,
22,33,44,55,66,77,88,99,0 ,11,
33,44,55,66,77,88,99,0 ,11,22,
44,55,66,77,88,99,0 ,11,22,33,
55,66,77,88,99,0 ,11,22,33,44,
66,77,88,99,0 ,11,22,33,44,55,
77,88,99,0 ,11,22,33,44,55,66,
88,99,0 ,11,22,33,44,55,66,77,
99,0 ,11,22,33,44,55,66,77,88,

10,11,12,13,14,15,16,17,18,19,
11,12,13,14,15,16,17,18,19,10,
12,13,14,15,16,17,18,19,10,11,
13,14,15,16,17,18,19,10,11,12,
14,15,16,17,18,19,10,11,12,13,
15,16,17,18,19,10,11,12,13,14,
16,17,18,19,10,11,12,13,14,15,
17,18,19,10,11,12,13,14,15,16,
18,19,10,11,12,13,14,15,16,17,
19,10,11,12,13,14,15,16,17,18,

20,21,22,23,24,25,26,27,28,29,
21,22,23,24,25,26,27,28,29,20,
22,23,24,25,26,27,28,29,20,21,
23,24,25,26,27,28,29,20,21,22,
24,25,26,27,28,29,20,21,22,23,
25,26,27,28,29,20,21,22,23,24,
26,27,28,29,20,21,22,23,24,25,
27,28,29,20,21,22,23,24,25,26,
28,29,20,21,22,23,24,25,26,27,
29,20,21,22,23,24,25,26,27,28

  };
if(FLASH_gInit()== 0)
{
if(FLASH_gSectorErase(testadd)==0)
{
if(FLASH_gWritePage(testadd , testdatawrite , 256) == 0)
{
FLASH_gReadPage (testadd, testdataread, 256);

      }

    }
if(FLASH_gSectorErase(testadd)==0)
{
if(FLASH_gWritePage(testadd , testdatawrite , 300) == 0)
{
FLASH_gReadPage (testadd, testdataread, 300);

      }

    }
  }
}
*/
/*void testcase3()
{
uint8_t testadd [4] = {
0,0x10,0x20,0
          };
uint8_t testadd1 [4] = {
0xff,0x10,0x20,0
      };
uint16_t i,j=0;
uint8_t flag=0;
uint8_t testaddread [256];
GPIO_gInitPin(PORT06, GPIO_PIN_3, GPIO_MODE_OUT_LOW);
GPIO_gInitPin(PORT06, GPIO_PIN_4, GPIO_MODE_OUT_LOW);
GPIO_gClrPin(PORT06, GPIO_PIN_3);
GPIO_gClrPin(PORT06, GPIO_PIN_4);
FLASH_gInit();
if(FLASH_gSectorErase(testadd)==0)
{
GPIO_gSetPin(PORT06, GPIO_PIN_3);
//FLASH_gWritePage()
for(i=0;i<16;i++)
{
if(FLASH_gReadPage (testadd, testaddread, 256)== 0)
{
for(j=0;j<256;j++)
{
if(testaddread[j]!=0xff)
{
flag=1;
break;
         }

        }
testadd[1]++;
      }
    }
  }
if(flag == 0)
GPIO_gSetPin(PORT06, GPIO_PIN_4);

}*/

void testcase2()
{
  /*
  testadd[0] represent number of byte per page
  testadd[1] represent number of page per sector
  testadd[2] represent number of sector per flash
  */
#if 0  
  static uint8_t testadd [4] = {
    0,0,0,0
  };
  uint8_t cmd_act=0;
  uint16_t i=0;
  uint8_t testdataread[256]={0} ;
  uint8_t testdatawrite[256]  ;
  uint8_t block_temp_no[4]={0,16,0,0};
  for(i=0;i<256;i++)
  {
    testdatawrite[i]=i;
  }
  if(FLASH_gInit()== 0)
  {
    while(block_temp_no[1]<32)
    {
      FLASH_gReadPage (block_temp_no, testdataread, 256);
      block_temp_no[1]++;
    }
    if(FLASH_gSectorErase(block_temp_no)==0)
    {
      while((block_temp_no[1]<32))
      {
        FLASH_gReadPage (testadd, testdataread, 256);
        FLASH_gWritePage(block_temp_no , testdataread , 256);
        block_temp_no[1]++;
        testadd[1]++;
        if(block_temp_no[1]==32)
        {
          testadd[1]=0;
          block_temp_no[1]=16;
          if(FLASH_gSectorErase(testadd)==0)
          {
            while(block_temp_no[1]<32)
            {
              FLASH_gReadPage (block_temp_no, testdataread, 256);
              FLASH_gWritePage(testadd , testdataread , 256);
              block_temp_no[1]++;
              testadd[1]++;
            }
          }
        }
      }
      
      while(cmd_act)
      {
        if(FLASH_gWritePage(testadd , testdatawrite , 256) == 0)
        {
          
          FLASH_gReadPage (testadd, testdataread, 256);
          testadd[1]++;
        }
      }
      
    }   
  }
#endif  
}

void testcase_erase(void )
{
  uint8_t testadd[4]={0,0,0,0};
  FLASH_gInit();
  FLASH_gBlockErase(testadd);
}

#ifdef TEST_FLASH
#ifdef TEST_ALL_FLASH
void flash_test_endurance(void)
{
  uint16_t i=0;
  
  for(i=0;i<256;i++)
  {
    testdatawrite[i]=i+random_num;
  }
  memset(testadd,0,sizeof(testadd));
  memset(buff_ff,0xff,sizeof(buff_ff));
  if(cmd_act)
  {
    memset((uint8_t*)&num_intrpt,0,sizeof(num_intrpt));
    if(FLASH_gInit()== 0)
    {
      // S_Timer1_CONFIG(1000000);
      if(FLASH_gChipErase()==0)
      {
        //        S_DISABLE_TIMER_1_INT;
        random_num++;
        for(block_64_index =0;block_64_index<64;block_64_index++ )
        {
          for(block_32_index =0;block_32_index<16;block_32_index++ )
          {
            for(block_4_index =0;block_4_index<16;block_4_index++ )
            {
              if(FLASH_gReadPage (testadd, testdataread, 256) == 0)
              {
                display_number_generic(random_num,flash_error);
                if(compare_2_buffer(buff_ff,testdataread,256))
                {
                  flash_error |= FLASH_ERR_ERASE_CHIP;
                  saved_arr[0] =flash_error;
                  memcpy((uint8_t*)(saved_arr+1),(uint8_t*)&random_num , sizeof(random_num));
                }
              }
              else
              {
                flash_error |= FLASH_ERR_ERASE_CHIP;
                saved_arr[0] =flash_error;
                memcpy((uint8_t*)(saved_arr),(uint8_t*)&random_num , sizeof(random_num));
              }
              testadd[1]++;
            }
            testadd[1]++;
          }
          testadd[2]++;
        }
      }
      memset(testadd,0,sizeof(testadd));
      testadd[1]=0x10;
      ///test wr,rd for all
      for(block_64_index =0;block_64_index<64;block_64_index++ )
      {
        for(block_32_index =0;block_32_index<16;block_32_index++ )
        {
          for(block_4_index =0;block_4_index<16;block_4_index++ )
          {
            if((FLASH_gWritePage(testadd , testdatawrite , 256) == 0))
            {
              if(FLASH_gReadPage (testadd, testdataread, 256) == 0)
              {
                display_number_generic(random_num,flash_error);
                if(compare_2_buffer(testdatawrite,testdataread,256))
                {
                  flash_error |= FLASH_ERR_ERASE_CHIP;
                  saved_arr[0] =flash_error;
                  memcpy((uint8_t*)(saved_arr),(uint8_t*)&random_num , sizeof(random_num));
                }
              }
              else
              {
                flash_error |= FLASH_ERR_READ;
                saved_arr[0] =flash_error;
                memcpy((uint8_t*)(saved_arr),(uint8_t*)&random_num , sizeof(random_num));
              }
            }
            else
            {
              flash_error |= FLASH_ERR_WRITE;
              saved_arr[0] =flash_error;
              memcpy((uint8_t*)(saved_arr),(uint8_t*)&random_num , sizeof(random_num));
            }
            testadd[1]++;
          }
          testadd[1]++;
        }
        testadd[2]++;
      }
    }
  }
}
#else
/*
Flash_address[0] represent number of byte per page
Flash_address[1] Bit0 --> Bit3 represent Page Index per sector  16,and Bit4-->Bit7 represent sector Number from 0 to  15
Flash_address[2] represent sector Number 16 to last sector in flash with 4096 sectors
*/
//uint16_t length_var=0;
void flash_test_endurance(void)
{
  uint16_t i=0;
  uint8_t j=0;
  uint8_t testadd [4] = {1,0x02,0x01,0x00};
  uint8_t testadd_1 [4] = {0x05,0x12,0x01,0x00};
  length_var++;
  if(length_var>5000)
  {
    length_var=240;
  }
  for(i=0;i<length_var;i++)
  {
    testdatawrite[i]=(uint8_t)(i+random_num)%107/*255*/;
  }
  if(cmd_act)
  {
    if(FLASH_gInit()== 0)
    {
      FLASH_gSectorErase(testadd_1);
      if(FLASH_gSectorErase(testadd)==0)
      {
        FLASH_gRead(testadd,testdataread , length_var );
        if(FLASH_gWrite(testadd, testdatawrite, length_var)==0)
        {
          if( FLASH_gRead( testadd,testdataread , length_var)==0)
          {
            random_num++;
            display_number_generic(random_num,flash_error);
            if(compare_2_buffer(testdatawrite,testdataread,length_var))
            {
              flash_error |= FLASH_ERR_COMPARE;
              saved_arr[0] =flash_error;
              memcpy((uint8_t*)(saved_arr),(uint8_t*)&random_num , sizeof(random_num));
            }
          }
          
          else
          {
            flash_error |= FLASH_ERR_READ;
            saved_arr[0] =flash_error;
            memcpy((uint8_t*)(saved_arr),(uint8_t*)&random_num , sizeof(random_num));
          }
        }
        else
        {
          flash_error |= FLASH_ERR_WRITE;
          saved_arr[0] =flash_error;
          memcpy((uint8_t*)(saved_arr),(uint8_t*)&random_num , sizeof(random_num));
        }
      }
      else
      {
        flash_error |= FLASH_ERR_ERASE_SECTOR;
        saved_arr[0] =flash_error;
        memcpy((uint8_t*)(saved_arr),(uint8_t*)&random_num , sizeof(random_num));
      }
    }
  }
}
#endif
uint8_t compare_2_buffer(uint8_t* src_buff,uint8_t* dest_buff,uint16_t size_buff)
{
  uint16_t index;
  uint8_t status =0;
  for(index=0;index < size_buff;index++)
  {
    if(src_buff[index]!= dest_buff[index] )
    {
      status = 1;
      break;
    }
  }
  return status;
}
#endif
uint8_t FlashRead(uint32_t u32Address, uint8_t *u8PtrData, uint16_t u16Length)
{
  uint8_t flash_addr[4];
  memset(flash_addr, 0, 4);
  ConvertUint32ToBytes(flash_addr, u32Address);
  FLASH_gReadPage(flash_addr, u8PtrData, u16Length);
  return 0;
}
uint8_t FlashWrite(uint32_t u32Address, uint8_t *u8PtrData, uint16_t u16Length)
{
  uint8_t flash_addr[4];
  memset(flash_addr, 0, 4);
  ConvertUint32ToBytes(flash_addr, u32Address);
  FLASH_gWritePage(flash_addr, u8PtrData, u16Length);
  return 0;
}
void ConvertUint32ToBytes(uint8_t *ptr_dest, uint32_t source)
{
  ptr_dest[0] = (uint8_t)source;
  ptr_dest[1] = (uint8_t)(source >> 8);
  ptr_dest[2] = (uint8_t)(source >> 16);
  ptr_dest[3] = (uint8_t)(source >> 24);
}

// use these functions for LCD

#if defined (DOT_MATRIX_LCD_ENABLE)
uint8_t page_mapping(uint8_t page)
{
  uint8_t f_page = page;
  if (f_page >= 16 && f_page <= 31)
  {
    f_page -= 16;
  }
  else if (f_page >= 32 && f_page <= 47)
  {
    f_page -= 32;
  }
  else if (f_page >= 48 && f_page <= 63)
  {
    f_page -= 48;
  }
  else if (f_page >= 64 && f_page <= 79)
  {
    f_page -= 64;
  }
  else if (f_page >= 80 && f_page <= 95)
  {
    f_page -= 80;
  }
  else if (f_page >= 96 && f_page <= 111)
  {
    f_page -= 96;
  }
  else if (f_page >= 112 && f_page <= 127)
  {
    f_page -= 112;
  }
  else if (f_page >= 128 && f_page <= 143)
  {
    f_page -= 128;
  }
  else if (f_page >= 144 && f_page <= 159)
  {
    f_page -= 144;
  }
  else if (f_page >= 160 && f_page <= 175)
  {
    f_page -= 160;
  }
  else if (f_page >= 176 && f_page <= 191)
  {
    f_page -= 176;
  }
  else if (f_page >= 192 && f_page <= 207)
  {
    f_page -= 192;
  }
  else if (f_page >= 208 && f_page <= 223)
  {
    f_page -= 208;
  }
  else if (f_page >= 224 && f_page <= 239)
  {
    f_page -= 224;
  }
  else if (f_page >= 240 )
  {
    f_page -= 240;
  }
  return f_page;
}

uint8_t FlashWrite_Pages(uint32_t u32Address, uint8_t *u8PtrData, uint16_t u16Length)
{
  uint8_t ii_Counter =0, L_GM_timeOut=0 ;
  uint8_t i=0;
  uint16_t Total_Length = u16Length,Send_Length =0;
  uint16_t Remainig_Length_inPage = 0;//Send_Length =0;
  uint32_t Temp_Address = u32Address;
  while((Temp_Address % FLASH_PAGE_SIZE) && (++L_GM_timeOut < 255))      //256 byte in page
  {    
    Remainig_Length_inPage++;
    Temp_Address++;
  }
  
  L_GM_timeOut = 0;
  
  if((Remainig_Length_inPage) && (u16Length > FLASH_PAGE_SIZE))
  {
     FlashWrite(u32Address ,u8PtrData , Remainig_Length_inPage );
     u32Address += Remainig_Length_inPage;
     u8PtrData+=Remainig_Length_inPage;
     u16Length-=Remainig_Length_inPage;
  }
  
  if(u16Length > FLASH_PAGE_SIZE)
  {
     while(!(Total_Length % FLASH_PAGE_SIZE == Total_Length) && (++L_GM_timeOut < 16))
      {    
        ii_Counter++;
        Total_Length-=FLASH_PAGE_SIZE;
       }

     
   for(i=0;i <= ii_Counter ; i++ )
    {
      
     
      if(u16Length  > FLASH_PAGE_SIZE )
      {
        Send_Length = FLASH_PAGE_SIZE;
        u16Length -= FLASH_PAGE_SIZE;
      }
      else
      {
        Send_Length  = u16Length;
      }
      
      FlashWrite(u32Address ,&u8PtrData[FLASH_PAGE_SIZE*i] , Send_Length );
      
      u32Address+=(FLASH_PAGE_SIZE);
    }
  }
  else
  {
    FlashWrite(u32Address ,u8PtrData , u16Length );
  }
   
  return 0;
}


uint8_t Flash_Write_Per_Page(uint32_t u32Address, uint8_t f_page, uint8_t *u8PtrData, uint16_t u16Length)
{
  uint8_t ret=0;
  uint8_t _page = 0;
  _page = page_mapping(f_page);
  uint8_t flash_addr[4];
  memset(flash_addr, 0, 4);
  for (uint8_t i = 0; i < _page; i++)
  {
    u32Address += FLASH_PAGE_SIZE;
  }
  ConvertUint32ToBytes(flash_addr, u32Address);
  ret =FLASH_gWritePage(flash_addr, u8PtrData, u16Length);
  return ret;
}

uint8_t Flash_Read_Per_Page(uint32_t u32Address, uint8_t f_page, uint8_t *u8PtrData, uint16_t u16Length)
{
  uint8_t ret=0;
  uint8_t _page = 0;
  _page = page_mapping(f_page);
  uint8_t flash_addr[4];
  memset(flash_addr, 0, 4);
  for (uint8_t i = 0; i < _page; i++)
  {
    u32Address += FLASH_PAGE_SIZE;
  }
  ConvertUint32ToBytes(flash_addr, u32Address);
  ret= FLASH_gReadPage(flash_addr, u8PtrData, u16Length);
  return ret;
}

uint8_t Flash_erase_sector_by_address(uint32_t address)
{
  uint8_t flash_addr[4],state=0;
  memset(flash_addr, 0, 4);
  ConvertUint32ToBytes(flash_addr, address);
  state = FLASH_gSectorErase(flash_addr);
  return state;
}

void Flash_erase_block_by_address(uint32_t address)
{
  uint8_t flash_addr[4];
  memset(flash_addr, 0, 4);
  ConvertUint32ToBytes(flash_addr, address);
  FLASH_gBlockErase(flash_addr);
}

#endif

/**
*@}
*/
/**
*@}
*/

