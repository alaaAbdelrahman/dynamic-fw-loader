/*
 * flash_intf.h
 *
 * Created: 29/04/2018 01:12:04 م
 *  Author: reda.abdelhameid
 */ 


#ifndef FLASH_INTF_H_
#define FLASH_INTF_H_
/*
=========================================================================
 *         Include files
 * ===========================================================================================*/
//#include "Types.h"
//#include "spi_intf.h"
//#include "soft_spi_intf.h"
#include "config.h"
#include "FLASH_Addresses.h"
#include "SPI_Interface.h"
/*============================================================================================
 *         Local new types declaration
 * ===========================================================================================*/
#define CONDITION_TIMEOUT     (uint32_t)(250)  //3 msec
#define FLASH_TIMER_VALUE       50  /*multible of 10msec*/
#define STATUS_REGISTER_1     1
#define STATUS_REGISTER_2     2
//-------------------------------------------------------------
typedef enum {
  NO_ERROR              =(uint8_t)0,
  ADDRESS_AVALIABLE     =(uint8_t)1,
  ADDRESS_UNAVALIABLE   =(uint8_t)2,
  FLASH_READ_ERROR      =(uint8_t)3,
  FLASH_WRITE_ERROR     =(uint8_t)4,
  FLASH_INIT_ERROR      =(uint8_t)5,
  FLASH_BUSY            =(uint8_t)6,
  FLASH_NOT_BUSY        =(uint8_t)7,
  FLASH_RESET_ERROR     =(uint8_t)8,
  FLASH_SECTOR_ERASE_ERROR    =(uint8_t)9,
  FLASH_BLOCK_ERASE_ERROR     =(uint8_t)10,
  FLASH_CHIP_ERASE_ERROR      =(uint8_t)11,
  FLASH_ENTER_LPM_ERROR       =(uint8_t)12,
  FLASH_RELEASE_LPM_ERROR     =(uint8_t)13,
  FLASH_NORMAL_MODE     =(uint8_t)14,
  FLASH_LPM             =(uint8_t)15,
  FLASH_COMPARE_ERROR   =(uint8_t)16,
  UNINITILIZED_DATA     =(uint8_t)17,
  FLASH_EXCEED_TYPE     =(uint8_t)18,
  FLASH_ERROR_OP        =(uint8_t)19,
  FLASH_APP_CRC_CORRUPT =(uint8_t)20,
}Flash_errors_Enmt;
//-------------------------------------------------------------



#define FLASH_PAGE_SIZE       256
//-------------------------------------------------------------

typedef enum        //flash commands
{
  WRITE_ENABLE        = 0x06,     //to enable the following instructions: Write Disable, Page
  //Program, Sector Erase, Block Erase, Chip Erase and Write Status Register

  READ_DATA         = 0x03,     //allows one more data bytes to be sequentially read from the memory
  FAST_READ_DATA	= 0x0B,		//allows one more data bytes to be sequentially read from the memory at the highest possible frequency
  WRITE_PAGE        = 0x02,     //allows from one byte to 256 bytes (a page) of data to be programmed at
  //previously erased (FFh) memory locations.

  READ_STATUS_REGISTER_1  = 0x05,     //allow the 8-bit Status Register 1 to be read.

  READ_STATUS_REGISTER_2  = 0x35,     //allow the 8-bit Status Register 2 to be read.

  WRITE_STATUS_REGISTER   = 0x01,     //allows the Status Register to be written

  MANUFACTURE_DEVICE_ID   = 0x9F ,     //
  ERASE_BLOCK       =0xD8 ,  //Erase 64K byte
  ERASE_BLOCK_32K         = 0x52, // Erase 32K byte 
  ERASE_SECTOR         = 0x20, // Erase 4K byte
  ERASE_CHIP        = 0xC7, // there are other value = 0x60
  RESET_CMD           = 0xFF ,
  POWER_DOWN        = 0xB9,
  RELEASE_POWER_DOWN    = 0xAB,

} Instructions;

typedef enum
{
  PAGE0 = 0,
  PAGE1,
  PAGE2,
  PAGE3,
  PAGE4,
  PAGE5,
  PAGE6,
  PAGE7,
  PAGE8,
  PAGE9,
  PAGE10,
  PAGE11,
  PAGE12,
  PAGE13,
  PAGE14,
  PAGE15,
} EN_FLASH_PAGE;



/*typedef enum
{
  ADDRESS_UNAVALIABLE = 0,          //address is not in the flash
  ADDRESS_AVALIABLE = 1           //address is in the flash
} Address_state;*/
/*============================================================================================
 *         Global Functions Prototype
 * ===========================================================================================*/
/** @addtogroup HAL
 *@{
 */
/** @addtogroup FLASH
 *@{
 */
/*============================================================================================
 * Function Name  FLASH_gInit
 * ===========================================================================================*/
/*!
* @brief   This function is responsible for initializing the FLASH
* @retval  uint8_t TRUE in success, FALSE else
* @param   none
*/
uint8_t FLASH_gInit(void);
/*============================================================================================
 * Function Name  FLASH_gDinit
 * ===========================================================================================*/
/*!
* @brief   This function is responsible for deinitializing the FLASH
* @retval  uint8_t TRUE in success, FALSE else
* @param   none
*/
uint8_t FLASH_gDeinit(void);
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
uint8_t FLASH_gReadPage (uint8_t* Flash_Address, uint8_t* dataPtr, uint16_t datalength);
/*============================================================================================
 * Function Name  FLASH_gRead
 * ===========================================================================================*/
/*!
* @brief   This function is used to read a data With any Size from the flash
* @retval  uint8_t TRUE in success, FALSE else
* @param     Flash_Address the data address
			 Flash_address[0] represent number of byte per page
  			 Flash_address[1] Bit0 --> Bit3 represent Page Index per sector  16,and Bit4-->Bit7 represent sector Number from 0 to  15
  			 Flash_address[2] represent sector Number 16 to last sector in flash with 4096 sectors
* @param     dataPtr dataPtr
* @param     datalength
*/
uint8_t FLASH_gRead(uint8_t* Flash_Address , uint8_t * dataPtr , uint16_t datalength);
/*============================================================================================
 * Function Name  FLASH_gWrite
 * ===========================================================================================*/
/*!
* @brief   This function is used to read Data With any size in flash
* @retval  uint8_t TRUE in success, FALSE other
* @param   byte_index : 0 to 4095
            start_page_index : 0 to 16
            sector_number : 0 to 4096
* @param     dataPtr
* @param     datalength
* @note : reda need to test
*/
uint8_t FLASH_gRead_ByIndex ( uint16_t byte_index, uint8_t start_page_index, uint16_t sector_number, uint8_t* dataPtr, uint16_t datalength);
/*============================================================================================
 * Function Name  FLASH_gWrite
 * ===========================================================================================*/
/*!
* @brief   This function is used to erase sector by sector number
* @retval  uint8_t TRUE in success, FALSE other
* @param     sector_number : 0 to 4095
*/
uint8_t FLASH_gEraseSector_ByIndex (uint16_t sector_number,uint8_t delay);
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
uint8_t FLASH_gFastReadPage(uint8_t* Flash_Address, uint8_t* dataPtr, uint16_t datalength);
/*============================================================================================
 * Function Name  FLASH_gWrite
 * ===========================================================================================*/
/*!
* @brief   This function is used to write Data With any size in flash
* @retval  uint8_t TRUE in success, FALSE other
* @param     Flash_Address
			 Flash_address[0] represent number of byte per page
  			 Flash_address[1] Bit0 --> Bit3 represent Page Index per sector  16,and Bit4-->Bit7 represent sector Number from 0 to  15
  			 Flash_address[2] represent sector Number 16 to last sector in flash with 4096 sectors
* @param     dataPtr
* @param     datalength
*/
uint8_t FLASH_gWrite(uint8_t* Flash_Address, uint8_t* dataPtr, uint16_t datalength);

/*============================================================================================
 * Function Name  FLASH_gWrite
 * ===========================================================================================*/
/*!
* @brief   This function is used to write Data With any size in flash
* @retval  uint8_t TRUE in success, FALSE other
* @param   byte_index : 0 to 4095
            start_page_index : 0 to 16
            sector_number : 0 to 4096
* @param     dataPtr
* @param     datalength
* @note : reda need to test
*/
uint8_t FLASH_gWrite_ByIndex ( uint16_t byte_index, uint16_t start_page_index, uint16_t sector_number, uint8_t* dataPtr, uint16_t datalength);
/*============================================================================================
 * Function Name  FLASH_gFastWritePage
 * ===========================================================================================*/
/*!
* @brief   This function is used to write Data in page size in flash
* @retval  uint8_t TRUE in success, FALSE other
* @param     Address
             Data array
             Data_length
*/

uint8_t FLASH_gFastWritePage(uint8_t* Flash_Address , uint8_t * Data , uint16_t Data_length);
/*============================================================================================
 * Function Name  FLASH_gSectorErase
 * ===========================================================================================*/
/*!
* @brief     This function is used to erase the FLASH sector
* @param     Flash_Address
* @retval    uint8_t TRUE in success, FALSE else
*
*/
uint8_t FLASH_gSectorErase(uint8_t* Flash_Address);
/*============================================================================================
 * Function Name  FLASH_gFastSectorErase
 * ===========================================================================================*/
/*!
* @brief     This function is used to erase the FLASH sector
* @param     Flash_Address
* @retval    uint8_t TRUE in success, FALSE else
*
*/
uint8_t FLASH_gFastSectorErase(uint8_t* Flash_Address);
/*============================================================================================
 * Function Name  FLASH_gBlockErase
 * ===========================================================================================*/
/*!
* @brief     This function is used to erase the FLASH block
* @param     Address
* @retval    uint8_t TRUE in success, FALSE else
*
*/
uint8_t FLASH_gBlockErase(uint8_t* Flash_Address);
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
uint8_t FLASH_gChipErase(void);

/*============================================================================================
 * Function Name  FLASH_gPunInLPM
 * ===========================================================================================*/
/*!
* @brief     This function is uded to put the FLASH in a low power mode
* @retval    uint8_t TRUE in success, FALSE else
*/
uint8_t FLASH_gPunInLPM(void);
/*============================================================================================
 * Function Name  FLASH_gReleaseLPM
 * ===========================================================================================*/
/*!
* @brief     This function is used to return the FLASH status to normal mode
* @retval    uint8_t TRUE in success, FALSE else
*/
uint8_t FLASH_gReleaseLPM(void);
/*============================================================================================
 * Function Name  FLASH_gGetStatus
 * ===========================================================================================*/
/*!
* @brief   This function is used to return the FLASH status, IN_POWER_MODE or in LOW_POWER_MODE
* @retval  uint8_t Get the FLASH status IN_POWER_MODE or in LOW_POWER_MODE
*/
uint8_t FLASH_gGetStatus(void);

/**
 *@}
 */
/**
 *@}
 */


uint8_t FLASH_Read_Reset(void);
uint8_t FLASH_Write_Status_Register(uint8_t Register_1 , uint8_t Register_2);
void testcase2();
uint8_t* increas_Addr_index(uint8_t index);
void testcase_erase(void );
uint8_t compare_2_buffer(uint8_t* src_buff,uint8_t* dest_buff,uint16_t size_buff);
void flash_test_endurance(void);
void ConvertUint32ToBytes(uint8_t* ptr_dest ,uint32_t source);
uint8_t FlashRead( uint32_t u32Address ,uint8_t* u8PtrData ,uint16_t u16Length);
uint8_t FlashWrite( uint32_t u32Address ,uint8_t* u8PtrData ,uint16_t u16Length);
uint8_t Flash_Write_Per_Page(uint32_t u32Address, uint8_t f_page, uint8_t *u8PtrData, uint16_t u16Length);
uint8_t Flash_Read_Per_Page(uint32_t u32Address, uint8_t f_page, uint8_t *u8PtrData, uint16_t u16Length);
uint8_t Flash_erase_sector_by_address(uint32_t address);
void Flash_erase_block_by_address(uint32_t address);
uint8_t page_mapping(uint8_t page);
uint8_t FlashWrite_Pages(uint32_t u32Address, uint8_t *u8PtrData, uint16_t u16Length);
#endif /* FLASH_INTF_H_ */