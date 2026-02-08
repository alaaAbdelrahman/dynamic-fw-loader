/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                              F I L E     S Y S T E M

                              S O U R C E     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file file_sys.c
 *
 * @brief File system is responsible to manage saving and loading from storage media.
 *
 * @details File system is responsible to manage saving and loading from storage media
 * for different modules and also manage recording logs of (profile, events, money, BPH).
 *
 * <b>References</b>\n
 *   File system analysis and design documents.
 *
 * <b>Edit History For File</b>\n
 *  This section contains comments describing changes made to this file.\n
 *  Notice that changes are listed in reverse chronological order.\n
 * <table border>
 * <tr>
 *   <td><b> when </b></td>
 *   <td><b> who </b></td>
 *   <td><b> what, where, why </b></td>
 * </tr>
 * <tr>
 *   <td> 27/03/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Save control data only with power down and not with any log update.</td>
 * </tr>
 * <tr>
 *   <td> 09/12/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Solve bug of logs addresses.</td>
 * </tr>
 * <tr>
 *   <td> 21/11/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Rewrite size of BPH record.</td>
 * </tr>
 * <tr>
 *   <td> 04/11/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Integrate with control and remove publishing of EEPROM hardware error.</td>
 * </tr>
 * <tr>
 *   <td> 24/09/12 </td>
 *   <td> aibraheem </td>
 *   <td> Integrate with metering. </td>
 * </tr>
 * <tr>
 *   <td> 13/09/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Enlarge event log and add ability to publish stop of EEPROM communication error. </td>
 * </tr>
 * <tr>
 *   <td> 26/08/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add support to use internal MCU memory in file system. </td>
 * </tr>
 * <tr>
 *   <td> 03/07/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add get multiple records function and config meter log. </td>
 * </tr>
 * <tr>
 *   <td> 19/06/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Modify get record to obey FIFO rule. </td>
 * </tr>
 * <tr>
 *   <td> 11/06/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Modify after run unit test. </td>
 * </tr>
 * <tr>
 *   <td> 28/05/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Created </td>
 * </tr>
 * </table>\n
 */

/*==========================================================================================

                                 INCLUDE FILES FOR MODULE

==========================================================================================*/
//#include "display.h"
#include "file_sys.h"
#ifdef FILE_SYS_TEST
#include "file_sys_test.h"
#else
//#include "trf_pymt.h"
//#include "Meter_Interface.h"
//#include "ctrl.h"
#endif
#include "string.h"
#include "Flash.h"
#include "Flash_app.h"
#include "config.h"
#if (MicroController == Micro_V94XX)
#include "V94XX_FLASH.h"
#else
#include "v85xx_flash.h"
#endif

  
#ifdef DLMS_ENABLED
#include "..\..\Communication\Dlms\Events\EventLog\COC_EventLog_Config.h"
#endif 
/*==========================================================================================

                          DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/*------------------------------------------------------------------------------------------
                                  Constants and Definitions
------------------------------------------------------------------------------------------*/
#ifdef FILE_SYS_LOG
#ifdef TRF_BP_HISTORY
#ifdef FILE_SYS_TEST
#define FILE_SYS_GET_SIZE_BPH_REC TRF_PYMT_GET_SIZE_BPH_REC
#else
/*! Get size of a BPH record.*/
#define FILE_SYS_GET_SIZE_BPH_REC TRF_PYMT_GET_SIZE_BPH_REC
#endif
#endif
#ifdef CTRL_EVNT_LOG
/*! Get size of an event record.*/
#define FILE_SYS_GET_SIZE_EVENT_REC CTRL_READ_SIZE_EVNT_REC()
#endif
#ifdef PYMT_MONY_TRANS
/*! Get size of a money transaction record.*/
#define FILE_SYS_GET_SIZE_MONEY_REC TRF_PYMT_MNY_REC_SIZE
#endif
#ifdef MTR_LOAD_PROFILE
/*! Get size of a profile record.*/
#define FILE_SYS_GET_SIZE_PROFILE_REC MTR_GET_LD_PROF_SIZE()
#else
#define FILE_SYS_GET_SIZE_PROFILE_REC 0
#endif
#ifdef CTRL_CFG_METER_LOG
/*! Get size of a config meter record.*/
#define FILE_SYS_GET_SIZE_CFG_METER CTRL_READ_SIZE_CFG_REC()
#endif
#endif
#define FILE_SYS_GET_SIZE_MD_REC          sizeof(mtr_data.mtr_data_mem.mtr_acc_md.md_reg)

#ifdef FM24C128D_2_Wire_Serial_EEPROM

#else
#ifdef FILE_SYS_LOG

#ifdef TRF_BP_HISTORY
/*! Address of first byte in the space reserved for BPH records.*/
#define FILE_SYS_GET_BPH_FILE_ADDRESS                  EEPROM_PAGE_30
#endif

#ifdef CTRL_EVNT_LOG
/*! Address of first byte in the space reserved for events log.*/
#define FILE_SYS_GET_EVENT_LOG_FILE_ADDRESS            EEPROM_PAGE_32
#endif
#ifdef PYMT_MONY_TRANS
/*! Address of first byte in the space reserved for money transaction records.*/
#define FILE_SYS_GET_MONEY_TRANS_FILE_ADDRESS          EEPROM_PAGE_61
#endif
#ifdef CTRL_CFG_METER_LOG
/*! Address of first byte in the space reserved for profile records.*/
#define FILE_SYS_GET_CFG_LOG_FILE_ADDRESS              EEPROM_PAGE_64
#endif
//#ifdef MTR_LOAD_PROFILE
/*! Address of first byte in the space reserved for configure meter records.*/
#define FILE_SYS_GET_PROFILE_FILE_ADDRESS              EEPROM_PAGE_66
//#endif
#endif
#endif
/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
//#ifdef FILE_SYS_LOG
/*!
 * Control data of file system containg index of each log and number of saved
 * records in each log.
 *
 * @brief Structure containg file system control data.
 */

/*------------------------------------------------------------------------------------------
                                     Local Variables
------------------------------------------------------------------------------------------*/
#ifdef FILE_SYS_LOG
#ifndef FLASH_FM25W32_ENABLE
/*! Array containg address of first record in each log of (BPH, money, event, profile)*/
const uint32_t record_adrs[FILE_SYS_MAX_LOGS] =
{
#ifdef TRF_BP_HISTORY
  FILE_SYS_GET_BPH_FILE_ADDRESS,
#endif
#ifdef CTRL_EVNT_LOG
  FILE_SYS_GET_EVENT_LOG_FILE_ADDRESS,
#endif
#ifdef PYMT_MONY_TRANS
  FILE_SYS_GET_MONEY_TRANS_FILE_ADDRESS,
#endif
//#ifdef MTR_LOAD_PROFILE
  FILE_SYS_GET_PROFILE_FILE_ADDRESS,
//#endif
#ifdef CTRL_CFG_METER_LOG
  FILE_SYS_GET_CFG_LOG_FILE_ADDRESS,
#endif
};
#endif
/*! Array containg maximum number of records to be saved in each log
 * of (BPH, money, event, profile).*/
const uint16_t max_num_record[FILE_SYS_MAX_LOGS] =
{
#ifdef TRF_BP_HISTORY
  TRF_BP_HISTORY_RECORDS,
#endif
#ifdef CTRL_EVNT_LOG
  EVENT_LOG_RECORD_NUM,
#endif
#ifdef PYMT_MONY_TRANS
  PMYT_MNY_TRANS_REC,
#endif
//#ifdef MTR_LOAD_PROFILE
  PROFILE_RECORD_NUM,
//#endif
#ifdef CTRL_CFG_METER_LOG
  CFG_METER_RECORD_NUM,
#endif
};


/*! Array containing the size of one record of different types.*/


/*! File system control structure.*/
#endif
uint8 is_Flash_work = 0 ; 
/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/
#ifdef FILE_SYS_TEST
#define FILE_SYS_PBLSH_EVENT(EVENT) fake_pblsh(EVENT)
#else
/*! Publish event.*/
#define FILE_SYS_PBLSH_EVENT(EVENT) ctrl_event(EVENT)
/*! Check Power down flag.*/
#define FILE_SYS_IS_PWR_DWN() CTRL_PWR_DN_STAT
#endif
/*!
 * initialize external EEPROM and its interface.
 */
#define FILE_SYS_INIT_EXT_MEM() EEPROM_Initialize()
/*!
 * Read data from storage media with maximum size 65535 Bytes.
 */
#define FILE_SYS_DATA_READ(data_ptr,eeprom_address, bytes_num)     EEPROM_read(Data_eeprom_address,data_ptr,eeprom_address, bytes_num)
/*!
 * Write data in storage media with maximum size 65535 Bytes.
 */
#define FILE_SYS_DATA_WRITE(data_ptr,eeprom_address, bytes_num)    EEPROM_write(Data_eeprom_address,data_ptr,eeprom_address, bytes_num)

#ifdef FILE_SYS_USE_INT
/*!
 * Used to unlock using internal MCU memory.
 */
#define FILE_SYS_INT_MEM_UNLOCK() FLASH_Unlock(FLASH_MemType_Data)
/*!
 * Used to lock using internal MCU memory.
 */
#define FILE_SYS_INT_MEM_LOCK() FLASH_Lock(FLASH_MemType_Data)
/*!
 * Used to write byte in internal MCU memory.
 */
#define FILE_SYS_INT_MEM_WRITE(ADD,BYTE_VALUE) FLASH_ProgramByte(ADD, BYTE_VALUE)
/*!
 * Used to read byte from internal MCU memory.
 */
#define FILE_SYS_INT_MEM_READ(ADD) FLASH_ReadByte(ADD)
#endif
/*!
 * Used to read byte from internal MCU memory.
 */
#define FILE_SYS_READ_HR() CTRL_READ_HR()
uint8_t trf_recalculation_prv_month=0;
/*==============================================================================

                                   FUNCTION DECLARATIONS

==============================================================================*/

/*!
 * @brief Read non volatile data.
 *
 * @par Description:
 * Read data from non voltaile storage media with max 65535 Bytes.
 *
 * @param [in] data_ptr   Address of buffer to get data in.
 * @param [in] address    Address of first byte of non volatile memory to be read.
 * @param [in] bytes_num  Number of bytes to read.
 *
 * @return
 * - TRUE if success and FALSE if error.
 */
uint8_t file_sys_read(uint8_t* data_ptr, uint32_t address, uint32_t bytes_num)
{
  /*!@par Pseudo Code:*/
  /*! - Load data.*/
  if(FILE_SYS_DATA_READ(data_ptr, address, bytes_num))
    /*! - Return true if no error*/
  {return TRUE;}
  return FALSE;
}

/*!
 * @brief Write non volatile data.
 *
 * @par Description:
 * Write data in non voltaile storage media with max 65535 Bytes.
 *
 * @param [in] data_ptr   Address of buffer to get data from.
 * @param [in] address    Address of first byte of non volatile memory to be written.
 * @param [in] bytes_num  Number of bytes to write.
 *
 * @return
 * - TRUE if success and FALSE if error.
 */
uint8_t file_sys_write(uint8_t* data_ptr, uint32_t address, uint32_t bytes_num)
{
  /*!@par Pseudo Code:*/
  /*! - Save data.*/
  if(FILE_SYS_DATA_WRITE(data_ptr, address, bytes_num))
    return TRUE;
  
#ifdef DEBUG_LOW_POWER
  uart_debug_byte ('f',0);
#endif
  return FALSE;
}

#define ADDRESS_TEST   (512*390)//200,000
#define NUM_SECTORS     90 // 90*512 = 46,000
#define VALUE_SET       0x12345678

void test_flash_internal()
{  uint32_t i;
  uint32_t w_buffer[128];/*
  uint16_t hw_buffer[256];
  uint8_t b_buffer[512];*/
  
  /* Clock initialization */
  /* Print initialization */
  for (i=0; i<512; i++)
  {
    /*
    b_buffer[i] = i % 256;
    if ((i%2) == 0)
      hw_buffer[i/2] = 0x1234 + i;*/
    if ((i%4) == 0)
      w_buffer[i/4] = VALUE_SET ;
  }
  
  FLASH_Init(FLASH_CSMODE_DISABLE);
  FLASH_CycleInit();
    
  for (i =0 ; i<NUM_SECTORS ; i++)
    FLASH_SectorErase(ADDRESS_TEST + i * 512);                     //erase ADDRESS_TEST~0x10200
  
  for (i = 0; i < 512*NUM_SECTORS; i+=4)
  {
  //  S_UART_SEND_BYTE(S_UART5 ,  *((__IO uint8_t *)(i +ADDRESS_TEST)));
    if (*(uint32_t*)(i +ADDRESS_TEST) != 0xffffffff)
    {
      S_UART_SEND_BYTE(S_UART5 ,  *((__IO uint8_t *)(i +ADDRESS_TEST)));
    }
  }
  
  S_UART_SEND_BYTE(S_UART5 ,  '\r');
  S_UART_SEND_BYTE(S_UART5 ,  '\n');   
  
  for (i =0 ; i<NUM_SECTORS ; i++)
    FLASH_ProgramWord ( ADDRESS_TEST + i*512 , w_buffer, 128 ) ;     //Word program
  
  /* Unlock flash */
  FLASH->PASS = 0x55AAAA55;
  
  for (i = 0; i < 512 * NUM_SECTORS ; i+=4)
  {    
    if (*(uint32_t*)(i +ADDRESS_TEST) != VALUE_SET)
    {
      S_UART_SEND_BYTE(S_UART5 ,  *((__IO uint8_t *)(i +ADDRESS_TEST)));
    }
  }
  S_UART_SEND_BYTE(S_UART5 ,  '\r');
  S_UART_SEND_BYTE(S_UART5 ,  '\n');   
  /*
  FLASH_ProgramWord(ADDRESS_TEST, w_buffer, 1);     //Word program
  for (i = 0; i < 128; i += 1)
  {
        if (*(uint32_t*)(i*4 +ADDRESS_TEST) != w_buffer[i])

    S_UART_SEND_BYTE(S_UART5 ,  *((__IO uint8_t *)(i + ADDRESS_TEST)));

  }*/
  /*
  S_UART_SEND_BYTE(S_UART5 ,  '\r');
  S_UART_SEND_BYTE(S_UART5 ,  '\n');  
  
  FLASH_SectorErase(ADDRESS_TEST);                    //erase ADDRESS_TEST~0x10200
  FLASH_ProgramByte(ADDRESS_TEST, b_buffer, 512);     //Byte program
  for (i = 0; i < 512; i++)
  {
            if (*(uint8_t*)(i +ADDRESS_TEST) != b_buffer[i])

    S_UART_SEND_BYTE(S_UART5 ,  *((__IO uint8_t *)(i + ADDRESS_TEST)));
    if ((i%10) == 9)
    {
    }
  }
  S_UART_SEND_BYTE(S_UART5 ,  '\r');
  S_UART_SEND_BYTE(S_UART5 ,  '\n');  
  
  FLASH_SectorErase(ADDRESS_TEST);                    //erase ADDRESS_TEST~0x10200
  FLASH_ProgramHWord(ADDRESS_TEST, hw_buffer, 256);   //Half-word program
  for (i = 0; i < 256; i += 1)
  {
    if (*(uint16_t*)(i*2 +ADDRESS_TEST) != hw_buffer[i])
    S_UART_SEND_BYTE(S_UART5 ,  *((__IO uint8_t *)(i + ADDRESS_TEST)));
    if ((i%10) == 9)
    {
    }
  }
  S_UART_SEND_BYTE(S_UART5 ,  '\r');
  S_UART_SEND_BYTE(S_UART5 ,  '\n');  */
  
}


/*!
 * @brief Get the index of the wanted record of certain type .
 *
 * @par Description:
 * Get the index of the wanted record of certain type like:
 * - Profile records
 * - Money transaction records
 * - Billing period history records
 * - Event records
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 *
 * @return
 * - 16 bit: the number of saved records.
 */
uint16_t file_sys_get_index_record(uint8_t record_type)
{
  /*!@par Pseudo Code:*/
  if ( record_type != FILE_SYS_BPH_RECORD)
  {
    uint16_t temp_num= flash_app_get_index_record(record_type);
    if((temp_num) > max_num_record[record_type])
    {
      temp_num =   (temp_num)% max_num_record[record_type];
    }
    return temp_num;
  }
  else
    return 1;
}
/*!
 * @brief Get the max number of records .
 *
 * @par Description:
 * Get the max number of records of certain type like:
 * - Profile records
 * - Money transaction records
 * - Billing period history records
 * - Event records
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 *
 * @return
 * - 16 bit: the number of saved records.
 */
uint32_t file_sys_get_the_max_No_record(uint8_t record_type)
{
  /*!@par Pseudo Code:*/
  /*! - Return the max number of the records which can be saved or read.*/
//#ifdef FLASH_FM25W32_ENABLE
  //return Flash_app_get_MAX_No_Records(record_type);
//#else
    return max_num_record[record_type];
//#endif
}
#ifdef FILE_SYS_USE_INT
/*!
 * @brief Read from internal EEPROM.
 *
 * @par Description:
 * This function is used to read from internal EEPROM with maximum 255 bytes.
 *
 * @param [in] mem_adr Address of internal EEPROM you want to read data from.
 * @param [out] data_ptr    Address of buffer to get the data in
 * @param [in] size Size of data in Bytes to read from internal EEPROM.
 *
 * @return
 * - None.
*/
void file_sys_read_int(uint32_t mem_adr, uint8_t *data_ptr, uint8_t size)
{
  /*!@par Pseudo Code:*/
  uint8_t index;
  /*! - Unlock internal memory.*/
  FILE_SYS_INT_MEM_UNLOCK();
  /*! - Read data.*/
  for(index = 0; index < size; index++)
    data_ptr[index] = FILE_SYS_INT_MEM_READ(mem_adr + index);
  /*! - Lock memory.*/
  FILE_SYS_INT_MEM_LOCK();
}

/*!
 * @brief Write in internal EEPROM.
 *
 * @par Description:
 * This function is used to write in internal EEPROM with maximum 255 bytes.
 *
 * @param [in] mem_adr Address of internal EEPROM you want to write data at.
 * @param [in] data_ptr    Address of buffer to get the data from.
 * @param [in] size Size of data in Bytes to write in internal EEPROM.
 *
 * @return
 * - None.
*/
void file_sys_write_int(uint32_t mem_adr, uint8_t *data_ptr, uint8_t size)
{
  /*!@par Pseudo Code:*/
  uint8_t index;
  /*! - Unlock internal memory.*/
  FILE_SYS_INT_MEM_UNLOCK();
  /*! - Write data.*/
  for(index = 0; index < size; index++)
    FILE_SYS_INT_MEM_WRITE(mem_adr + index, data_ptr[index]);
  /*! - Lock memory.*/
  FILE_SYS_INT_MEM_LOCK();
}
#endif

#ifdef FILE_SYS_LOG
/*!
 * @brief Clear records of certain type.
 *
 * @par Description:
 * Clear all record saved in storage media of certain type like:
 * - Profile records
 * - Money transaction records
 * - Billing period history records
 * - Event records
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t that will be cleared
 *
 * @return
 * - None.
 *
 * @note
 * - Clearing storage media is not done by reset pages in storage media but, only by reset index
 *   and number of saved records to be overwritten by coming records.
 */
void file_sys_clear_log(uint8_t record_type)
{
#ifdef FLASH_FM25W32_ENABLE
  if ( record_type != FILE_SYS_BPH_RECORD)
  Flash_app_clear_Record(record_type);
#endif
  /*!@par Pseudo Code:*/
  /*! - Clear index of the log.*/
  //ctrl_sys_var_optional_feature.record_index[record_type] = 0;
  /*! - Clear number of saved records in the log.*/
}

/*!
 * @brief Get number of saved records of certain type.
 *
 * @par Description:
 * Get number of saved in storage media of certain type like:
 * - Profile records
 * - Money transaction records
 * - Billing period history records
 * - Event records
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 *
 * @return
 * - 16 bit: the number of saved records.
 */
uint32_t file_sys_get_num_record(uint8_t record_type)
{
  uint32_t temp_num;
  if ( record_type != FILE_SYS_BPH_RECORD)
  { 
    temp_num = Flash_app_get_num_record(record_type);
    if((temp_num) > max_num_record[record_type])
    {
      temp_num =    max_num_record[record_type];
    }
    return temp_num;
  }
  else
  {
    /*!@par Pseudo Code:*/
  /*! - Return number of saved records in this log.*/
    //temp_num = 1;
    if((temp_num) > max_num_record[record_type])
    {
      temp_num =  max_num_record[record_type];
    }
  return temp_num;
  }
}

/*!
 * @brief Get a certain record from records saved in storage media.
 *
 * @par Description:
 * Get a record data saved in storage media of certain type like:
 * - Profile record
 * - Money transaction record
 * - Billing period history record
 * - Event record
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 * @param [in] record_num  Number of record to be read (0 -> max-1)
 * @param [out] data_ptr   Address of buffer to get the data in
 *
 * @return
 * - None.
 *
 * @note
 * If we can't access the storage media, a communication error event is published.
 */
void file_sys_get_record(uint8_t record_type, uint16_t record_num, uint8_t *data_ptr)
{
  if ( record_type != FILE_SYS_BPH_RECORD)
  {
    if (!trf_recalculation_prv_month)
    {
      if((Flash_app_get_num_record(record_type)) >= max_num_record[record_type])
      {
        record_num = (record_num + Flash_app_get_num_record(record_type)) % max_num_record[record_type];
      }  
    }
    record_num = flash_mapping(record_type, record_num);
    Flash_app_get_record (  record_type ,  record_num , data_ptr ,  1 );
  }
  else
  {
    /*!@par Pseudo Code:*/
    /*! - Check if the number of record required is valid.*/
    if(record_num < 1)
    {
      if (!trf_recalculation_prv_month)
      {
        /*! - Calculate record number to be read. */
      }    
      /*! - Read this record from storage media. */
      file_sys_read(data_ptr, EEPROM_ADDRESS_BILLING_HISTORY + (uint32_t)size_of_record[record_type] * record_num, size_of_record[record_type]);
    }
  }
}

/*!
 * @brief Get number of records saved in storage media.
 *
 * @par Description:
 * Get certain number of records saved in storage media of certain type like:
 * - Profile record
 * - Money transaction record
 * - Billing period history record
 * - Event record
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 * @param [in] from        First record number you want to read (0 -> max-1)
 * @param [in] num_rec  Number of record to be read (1 -> max)
 * @param [out] data_ptr   Address of buffer to get the data in
 *
 * @return
 * - Size of data written in buffer if read is done successfully, 0 if no data read.
 */
uint16_t file_sys_get_mlti_rec(uint8_t record_type, uint16_t from, uint16_t num_rec, uint8_t *data_ptr)
{
  if ( record_type != FILE_SYS_BPH_RECORD)
  {
    uint8_t status =0 ;
    uint16_t mapped_from,ii;
    for(ii=0;ii<num_rec;ii++)
    {
      mapped_from = flash_mapping(record_type, from);
      status = Flash_app_get_record (  record_type ,  mapped_from , data_ptr ,  1 );
      if (status != NO_ERROR)
        return 0;
      from++;
      data_ptr+=size_of_record[record_type];
    }
  }
  else
  {
  /*!@par Pseudo Code:*/
  uint16_t rest_rec_num = num_rec;
  uint8_t i = 1;
  if (from+num_rec > 1)
    return 0;
  /*! - Check if the number of saved records in the log equals the maximum number.*/
  if(1 == max_num_record[record_type])
  {
    from = from % max_num_record[record_type];
    rest_rec_num = max_num_record[record_type] - from;
  }
    else
    {
      rest_rec_num = num_rec;
    }
  
  /*! - Start reading from non-volatile area.*/
  for(; i > 0; i--)
  {
    file_sys_read(data_ptr, EEPROM_ADDRESS_BILLING_HISTORY + ((uint32_t)from * size_of_record[record_type]), \
      (uint32_t)rest_rec_num * size_of_record[record_type]);
    data_ptr += (uint32_t)rest_rec_num * size_of_record[record_type];
    from = 0;
    rest_rec_num = num_rec - rest_rec_num;
  }
  }
  /*! - Return size of read data in Bytes.*/
  return (num_rec * size_of_record[record_type]);
}

/*!
 * @brief Save a certain record in storage media.
 *
 * @par Description:
 * Save a record data in storage media of certain type like:
 * - Profile record
 * - Money transaction record
 * - Billing period history record
 * - Event record
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 * @param [in] data_ptr    Address of buffer to get the data in
 *
 * @return
 * - None.
 *
 * @note
 * If we can't access the storage media, a communication error event is published.
 */
void file_sys_save_record(uint8_t record_type, uint8_t *data_ptr)
{
  if ( record_type != FILE_SYS_BPH_RECORD)
    Flash_app_save_record( record_type, data_ptr);
  else  /*!@par Pseudo Code:*/
  {
    /*! - Write record in storage media.*/
    if(file_sys_write(data_ptr, EEPROM_ADDRESS_BILLING_HISTORY + (uint32_t)size_of_record[record_type] * 1, size_of_record[record_type]))
    {
      if (trf_recalculation_prv_month)
        return;
      /*! - Inrement index and number of saved records.*/
      //ctrl_sys_var_optional_feature.record_index[record_type]++;
      //ctrl_sys_var_optional_feature.record_index[record_type] %= max_num_record[record_type];
      //if(1 != max_num_record[record_type])
    }
  }
}

/*!
 * @brief Tell file system module with a new event.
 *
 * @par Description:
 * Tell file system with new event.
 *
 * @param [in] event_code Code of event #file_sys_event_type_t
 *
 * @return
 * - None.
 */
void file_sys_event(uint8_t event_code)
{
  /*!@par Pseudo Code:*/
  switch(event_code)
  {
  case FILE_SYS_PWR_DN:
#ifdef FLASH_FM25W32_ENABLE
    Flash_app_event (FLASH_APP_PWR_DN);
#endif
    break;
  case FILE_SYS_NEW_SEC:
    //if(eeprom_load_errors_register&FILE_SYS_ERR_LOAD_FLAG)
    //file_sys_init();
    break;
  case FILE_SYS_PWR_UP:
#ifdef FLASH_FM25W32_ENABLE
    Flash_app_event (FLASH_APP_UP_SECONDS);
#endif 
    break;
    
  }
}
#endif /* endof FILE_SYS_LOG */

/*!
 * @ingroup FILE_SYS_CORE
 *
 * @brief Initialize logs containing records of different types.
 *
 * @par Description:
 * Initialize all logs containing records of different types like:
 * - Profile records
 * - Money transaction records
 * - Billing period history records
 * - Event records
 * - Configure meter records
 *
 * @return
 * - None.
 *
 * @note
 * If storage media can't be read, a communication error event is published.
 * If the CRC check failed, a data corrupt event is published.
 */
void file_sys_init(void)
{
#ifdef FLASH_FM25W32_ENABLE
  uint8_t status = 0;
#endif
  /*!@par Pseudo Code:*/
  eeprom_load_errors_register &= ~FILE_SYS_ERR_LOAD_FLAG;
  FILE_SYS_INIT_EXT_MEM();
#ifdef FLASH_FM25W32_ENABLE
  FLASH_gInit();
  status = Flash_app_init();
  if (status != NO_ERROR)
    eeprom_load_errors_register |= FILE_SYS_ERR_LOAD_FLAG;
  if ( Flash_app_test_byte() == true)
    is_Flash_work = true;
  else
    is_Flash_work = false;
#endif
}

#ifdef TEST_BILLING_RECORD
void test_filling_billing_records(uint8_t number_of_records)
{
  trf_pymt_bph_rec_t bph_record_obj;
  uint8_t indexer = 1;
  for(indexer = 1;indexer<number_of_records;indexer++)
  {
    bph_record_obj.consumption_wh = indexer*100;
    bph_record_obj.mda = indexer*100;
    bph_record_obj.bill = indexer*1000;
    bph_record_obj.diff_debit_recalc  = indexer*100;
    bph_record_obj.tax_customer_serv = indexer*100;
    bph_record_obj.money_balance = indexer*10000;
    bph_record_obj.mdw = indexer*100;
    bph_record_obj.day =1;
    if(indexer >12)
    {
      bph_record_obj.month = indexer%12;
      bph_record_obj.year =20;
    }
    else
    {
      bph_record_obj.month = indexer;
      bph_record_obj.year =19;
    }
    file_sys_save_record(FILE_SYS_BPH_RECORD, (uint8_t *)&bph_record_obj );
  }
}
#endif
TU08 FileSystem_gStoreItemList(TU08 recordID, TU08* ptrData)
{
  file_sys_save_record(recordID, ptrData);
  return true;
}

TU08 FileSystem_gGetMaxNumRecord(TU08 recordID, TU32* max_records_num)
{
    *max_records_num = file_sys_get_the_max_No_record(recordID);
    return true;
}

TU08 FileSystem_gClearList (uint8_t record_type)
{
  file_sys_clear_log( record_type);
  return NO_ERROR;
}

TU08 FileSystem_gInitList ( TU08 recordID , TU16 record_size )
{
  return NO_ERROR;
}

TU08 FileSystem_gGetItemList(uint8_t recordID, TU08* ptrData, TU32 index)
{
  file_sys_get_record( recordID, index, ptrData);
  return NO_ERROR;
}

TU08 FileSystem_gGetEntriesInList(TU08 recordID, TU32* entries_number)
{
  *entries_number = file_sys_get_num_record(recordID);
  return NO_ERROR;
}
TU08 FileSystem_gGet_ListLastIndex_Inuse(TU08 recordID, TU32* index , TU32* max_num_entries, TU32* current_num_of_entries)
{
  *index = file_sys_get_index_record(recordID);
  *max_num_entries = file_sys_get_the_max_No_record(recordID);
  *current_num_of_entries = file_sys_get_num_record(recordID);
  return NO_ERROR;
}
TU08 FileSystem_gGetLast_ActualIndex_InUse_InList(TU08 recordID, TU32* actual_index)
{
  *actual_index = file_sys_get_index_record(recordID);
  return NO_ERROR;
}
