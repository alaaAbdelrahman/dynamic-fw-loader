#ifndef FILE_SYS_H
#define FILE_SYS_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               F I L E    S Y S T E M

                              H E A D E R     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file file_sys.h
 *
 * @brief This file contains Addresses of all files in storage media.
 *
 * @details This file contains the required definitions, types for saving
 * and loading in storage media.
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
 *   <td> 24/09/12 </td>
 *   <td> aibraheem </td>
 *   <td> Integrate with metering. </td>
 * </tr>
 * <tr>
 *   <td> 13/09/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Modify comments and Doxygen groups. </td>
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
 *   <td> 28/05/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Created </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup FILE_SYS File System
 * @ingroup METER
 * File system is responsible for:
 *   -# Read and write in files of internal modules.
 *   -# Billing period history records.
 *   -# Money transaction records.
 *   -# Event log records.
 *   -# Profile records.
 *
 * \n<b>References</b>\n
 * - Analysis document of file system.
 * - Design document of file system.
 *
 * @brief File system is responsible for saving and loading in storage media for
 * different modules and all logging.
 */
/*!
 * @defgroup FILE_SYS_CORE Application Core APIs
 * @ingroup FILE_SYS
 * @brief File system initialization functions.
 */
/*!
 * @defgroup FILE_SYS_READ Read System Variables APIs
 * @ingroup FILE_SYS
 * @brief Used to read data from file system module.
 */
/*!
 * @defgroup FILE_SYS_CMD Commands APIs
 * @ingroup FILE_SYS
 * @brief File system command interface APIs.
 */
/*!
 * @defgroup FILE_SYS_EVENT Events APIs
 * @ingroup FILE_SYS
 * @brief File system event interface APIs.
 */

/*==========================================================================================

                                 INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "config.h"
#include "EEPROM.h"
#include "basic_srv.h"
#include "EEPROM_Addresses.h"
#include "FLASH_Addresses.h"
/*==========================================================================================

                          DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/*------------------------------------------------------------------------------------------
                                  Constants and Definitions
------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------
                                    External Variables
------------------------------------------------------------------------------------------*/
extern uint8_t first_init;
extern uint8_t trf_recalculation_prv_month;
extern volatile uint16_t eeprom_load_errors_register; // indicate if we load wrong data from the eeprom
extern uint8 is_Flash_work ; 
/*!
 * @addtogroup FILE_SYS_READ
 * @{
 */
#ifdef FILE_SYS_USE_INT
/*! The address of first byte in internal data EEPROM.*/
#define INTERNAL_DATA_EEPROM_ADRS FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS
/*! Address of section 0 in internal EEPROM.*/
#define FILE_SYS_GET_INTRNL_MEM_SEC0_ADRS INTERNAL_DATA_EEPROM_ADRS
#endif
/*! Address of first byte in the space reserved for tariff and payment configuration.*/
#define FILE_SYS_GET_TRF_PYMT_CFG_ADRS         EEPROM_PAGE_0
/*! Address of first byte in the space reserved for tariff and payment waiting tariff configuration.*/
#define FILE_SYS_GET_TRF_PYMT_WAITING_TRF_ADRS EEPROM_PAGE_1
/*! Address of first byte in the space reserved for tariff and payment system variables.*/
#define FILE_SYS_GET_TRF_PYMT_SYS_VAR_ADRS     EEPROM_PAGE_2
/*! Address of first byte in the space reserved for STS module.*/
#define FILE_SYS_GET_STS_ADRS                  EEPROM_PAGE_4
/*! Address of first byte in the space reserved for metering application configuration.*/
#define FILE_SYS_GET_METERING_APL_CFG_ADRS     EEPROM_PAGE_6
/*! Address of first byte in the space reserved for metering calibration configuration.*/
#define FILE_SYS_GET_METERING_CALB_CFG_ADRS    EEPROM_PAGE_7
/*! Address of first byte in the space reserved for metering system variables.*/
#define FILE_SYS_GET_METERING_SYS_VAR          EEPROM_PAGE_8
/*! Address of first byte in the space reserved for display configuration.*/
#define FILE_SYS_GET_DISPLAY_CFG               EEPROM_PAGE_57
/*! Address of first byte in the space reserved for control configuration.*/
#define FILE_SYS_GET_CONTROL_CFG_ADRS          EEPROM_PAGE_14
/*! Address of first byte in the space reserved for control system variables.*/
#define FILE_SYS_GET_CONTROL_SYS_VAR_ADRS      EEPROM_PAGE_15
/*! Address of first byte in the space reserved for GPRS communication data.*/
#define FILE_SYS_GET_COM_GPRS_DATA_ADRS        EEPROM_PAGE_17
/*! Address of first byte in the space reserved for RF communication data.*/
#define FILE_SYS_GET_COM_RF_DATA_ADRS          EEPROM_PAGE_18
/*! Address of first byte in the space reserved for RFID communication data.*/
#define FILE_SYS_GET_COM_RFID_DATA_ADRS        EEPROM_PAGE_19
/*! Address of first byte in the space reserved for RFID communication data.*/
#define FILE_SYS_GET_CTRL_VARIABLE_OPTIONAL_DATA_ADRS        EEPROM_PAGE_20
#define FILE_SYS_GET_BACKUP_CTRL_VARIABLE_OPTIONAL_DATA_ADRS        EEPROM_PAGE_21
/*! Address of first byte in the space reserved for file system control.*/
#define FILE_SYS_GET_FILE_SYS_CTRL_ADRS        EEPROM_PAGE_22
/*! Address of first byte in the space reserved for metering application configuration backup address .*/
#define FILE_SYS_GET_METERING_APL_CFG_BACKUP_ADRS      EEPROM_PAGE_23
#define FILE_SYS_GET_METERING_CALB_CFG_BACKUP_ADRS     EEPROM_PAGE_24
#define FILE_SYS_GET_TRF_PYMT_CFG_BACKUP_ADRS          EEPROM_PAGE_25 
#define FILE_SYS_GET_METERING_SYS_VAR_BACKUP_ADRS      EEPROM_PAGE_26
#define FILE_SYS_GET_CONTROL_CFG_BACKUP_ADRS           EEPROM_PAGE_29
#ifdef FM24C128D_2_Wire_Serial_EEPROM
#define FILE_SYS_GET_DISPLAY_SHOW                      EEPROM_PAGE_30  ///display take 3 Pages to save default configuration 
#define FILE_SYS_GET_TRF_PYMT_SYS_VAR_ADRS_BACKUP      EEPROM_PAGE_60  //page 33 is empty
#define FILE_SYS_CONTROL_NEW_VAR_ADRS_BACKUP           EEPROM_PAGE_34 // 519B , 3pages
#define FILE_SYS_GET_CONTROL_NEW_VAR_ADRS              EEPROM_PAGE_37 // 519B , 3pages
#define FILE_SYS_GET_CONTROL_VAR_BACKUP_ADRS           EEPROM_PAGE_40// 2pages


// these addresses are for DLMS
//reserved 5 pages for DLMS CONFIG
#define FILE_SYS_DLMS_GONFIG_ADRS		        EEPROM_PAGE_42

#define FILE_SYS_GET_FILE_SYS_DLMS_SECURITY_GONFIG_ADRS				FILE_SYS_DLMS_GONFIG_ADRS//102

#define FILE_SYS_GET_FILE_SYS_DLMS_INIT_LOCAL_PORT_ADRS				(FILE_SYS_DLMS_GONFIG_ADRS +102)//1

#define FILE_SYS_GET_FILE_SYS_DLMS_LOCAL_PORT_ADRS				(FILE_SYS_DLMS_GONFIG_ADRS+103)//35

#define FILE_SYS_GET_FILE_SYS_DLMS_TCP_UDP_ADRS					(FILE_SYS_DLMS_GONFIG_ADRS +138)//13

#define FILE_SYS_GET_FILE_SYS_DLMS_EVENT_NOTIFICATION_ADRS		        (FILE_SYS_DLMS_GONFIG_ADRS+151)//1 

#define FILE_SYS_GET_FILE_SYS_DLMS_INIT_HDLC_ADRS				(FILE_SYS_DLMS_GONFIG_ADRS + 152) //1

#define FILE_SYS_GET_FILE_SYS_DLMS_HDLC_ADRS					(FILE_SYS_DLMS_GONFIG_ADRS +153) //5

#define FILE_SYS_GET_FILE_SYS_DLMS_INIT_TCP_UDP_ADRS				(FILE_SYS_DLMS_GONFIG_ADRS +158) //1
#else
#define FILE_SYS_GET_DISPLAY_SHOW                      EEPROM_PAGE_504  ///display take 3 Pages to save default configuration 
#define FILE_SYS_GET_TRF_PYMT_SYS_VAR_ADRS_BACKUP      EEPROM_PAGE_400

#define FILE_SYS_CONTROL_NEW_VAR_ADRS_BACKUP           EEPROM_PAGE_402 // 519B , 3pages
#define FILE_SYS_GET_CONTROL_NEW_VAR_ADRS              EEPROM_PAGE_397 // 519B , 3pages
#define FILE_SYS_GET_CONTROL_VAR_BACKUP_ADRS           EEPROM_PAGE_401
#ifdef FILE_SYS_LOG
#ifdef TRF_BP_HISTORY
/*! Address of first byte in the space reserved for BPH records.*/
#define FILE_SYS_GET_BPH_FILE_ADDRESS                       EEPROM_PAGE_30
#define EEPROM_ADDRESS_BILLING_HISTORY                      EEPROM_PAGE_30
#endif
#ifdef PYMT_MONY_TRANS
/*! Address of first byte in the space reserved for money transaction records.*/
#define FILE_SYS_GET_METER_MODE_FILE_ADDRESS                EEPROM_PAGE_61
#endif
#ifdef CTRL_CFG_METER_LOG
/*! Address of first byte in the space reserved for profile records.*/
#define FILE_SYS_GET_CFG_LOG_FILE_ADDRESS                    EEPROM_PAGE_64
#endif
//#ifdef MTR_LOAD_PROFILE
/*! Address of first byte in the space reserved for configure meter records.*/
#define FILE_SYS_GET_PROFILE_FILE_ADDRESS                    EEPROM_PAGE_66  // 3360 records * 10 byte
//#endif
#endif

#define FILE_SYS_GET_FILE_SYS_DLMS_INIT_SECURITY_GONFIG_ADRS		        EEPROM_PAGE_410

#define FILE_SYS_GET_FILE_SYS_DLMS_SECURITY_GONFIG_ADRS				EEPROM_PAGE_411

#define FILE_SYS_GET_FILE_SYS_DLMS_INIT_LOCAL_PORT_ADRS				EEPROM_PAGE_412

#define FILE_SYS_GET_FILE_SYS_DLMS_LOCAL_PORT_ADRS				EEPROM_PAGE_413

#define FILE_SYS_GET_FILE_SYS_DLMS_TCP_UDP_ADRS					EEPROM_PAGE_415

#define FILE_SYS_GET_FILE_SYS_DLMS_EVENT_NOTIFICATION_ADRS		        EEPROM_PAGE_415+20

#define FILE_SYS_GET_FILE_SYS_DLMS_INIT_HDLC_ADRS				EEPROM_PAGE_416

#define FILE_SYS_GET_FILE_SYS_DLMS_HDLC_ADRS					EEPROM_PAGE_417

#define FILE_SYS_GET_FILE_SYS_DLMS_INIT_TCP_UDP_ADRS				EEPROM_PAGE_418
#endif
/*! @} */

/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
#ifdef FILE_SYS_LOG
/*!
 * @par Description:
 *   This enumerator contains all records types saved in EEPROM.
 *
 * @ingroup FILE_SYS
 */
typedef enum
{
#ifdef TRF_BP_HISTORY
  FILE_SYS_BPH_RECORD=0,       /*!< BPH record.*/
#endif
#ifdef CTRL_EVNT_LOG
  FILE_SYS_EVENT_RECORD,     /*!< Event record.*/
#endif
#ifdef PYMT_MONY_TRANS
  FILE_SYS_MONEY_RECORD,     /*!< Money record.*/
#endif
//#ifdef MTR_LOAD_PROFILE
  FILE_SYS_PROFILE_RECORD,   /*!< Profile record.*/
//#endif
#ifdef CTRL_CFG_METER_LOG
  FILE_SYS_CFG_METER_RECORD, /*!< Configure meter log.*/
#endif
  FILE_SYS_MD_RECORD, /*!< Configure meter log.*/
#ifdef DLMS_ENABLED
  DISCONNECT_CONTROL_EVENTS_LOG_ID,
  METER_ERRORS_EVENTS_LOG_ID,
  CUSTOME_WITH_PARAMETERS_EVENTS_LOG_ID,
  FIRMWARE_SELF_CHECK_LOG_ID,
  STANDARD_EVENTS_LOG_ID, //(+ event log)
  POWER_EVENTS_LOG_COC_ID, //(+ event log)
  FILE_SYS_LP_LIST1_ID,
  FILE_SYS_LP_LIST2_ID,
  FILE_SYS_LP_LIST4_ID,
  FILE_SYS_LP_LIST5_ID,
  PROFILE_CONUMPTION_MONTHLY_ID,  
#endif
#ifdef G2_DM
  FILE_SYS_TAMPER_RECORD,              /*!< Tamper record.*/
  FILE_SYS_REMOVED_TAMPER_RECORD,     /*!< Removed Tamper record.*/
  FILE_SYS_MAX_LOGS = FILE_SYS_REMOVED_TAMPER_RECORD, /*!< Number of record types.*/
#else
    FILE_SYS_MAX_LOGS = FILE_SYS_MD_RECORD,/*!< Number of record types.*/
#endif
} file_sys_record_type_t;

/*!
 * @addtogroup FILE_SYS_EVENT
 * @{
 */
typedef enum
{
  FILE_SYS_PWR_DN = (uint8_t)0,
  FILE_SYS_NEW_HR = (uint8_t)1,
  FILE_SYS_NEW_SEC,
  FILE_SYS_NEW_DAY,
  FILE_SYS_PWR_UP,
} file_sys_event_type_t;
/*! @} */

__packed typedef struct
{
  uint16_t num_saved_record[FILE_SYS_MAX_LOGS];  /*!< Number of saved records in each log.*/
  uint16_t record_index[FILE_SYS_MAX_LOGS];      /*!< Index of each log.*/
  uint8_t crc;                                   /*!< CRC of this structure*/
} record_control_t;
extern const uint16_t max_num_record[FILE_SYS_MAX_LOGS];
#endif

/*------------------------------------------------------------------------------------------
                                     External Variables
------------------------------------------------------------------------------------------*/
extern const uint8_t size_of_record[];

/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/

/*==========================================================================================

                                   FUNCTION DECLARATIONS

==========================================================================================*/
/*! @addtogroup FILE_SYS_READ
 * @{
 */
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
uint8_t file_sys_read(uint8_t* data_ptr, uint32_t address, uint32_t bytes_num);
/*!@}*/

/*! @addtogroup FILE_SYS_CMD
 * @{
 */
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
uint8_t file_sys_write(uint8_t* data_ptr, uint32_t address, uint32_t bytes_num);
/*!@}*/
void test_flash_internal();

#ifdef FILE_SYS_USE_INT
/*! @addtogroup FILE_SYS_READ
 * @{
 */
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
void file_sys_read_int(uint32_t mem_adr, uint8_t *data_ptr, uint8_t size);
/*!@}*/

/*! @addtogroup FILE_SYS_CMD
 * @{
 */
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
void file_sys_write_int(uint32_t mem_adr, uint8_t *data_ptr, uint8_t size);
/*! @} */
#endif
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
uint16_t file_sys_get_index_record(uint8_t record_type);

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
uint32_t file_sys_get_the_max_No_record(uint8_t record_type);
#ifdef FILE_SYS_LOG
/*! @addtogroup FILE_SYS_CMD
 * @{
 */
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
void file_sys_clear_log(uint8_t record_type);
/*! @} */

/*! @addtogroup FILE_SYS_READ
 * @{
 */
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
uint32_t file_sys_get_num_record(uint8_t record_type);

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
void file_sys_get_record(uint8_t record_type, uint16_t record_num, uint8_t *data_ptr);

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
uint16_t file_sys_get_mlti_rec(uint8_t record_type, uint16_t from, uint16_t num_rec, uint8_t *data_ptr);
/*! @} */

/*! @addtogroup FILE_SYS_CMD
 * @{
 */
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
void file_sys_save_record(uint8_t record_type, uint8_t *data_ptr);
/*! @} */

/*! @addtogroup FILE_SYS_EVENT
 * @{
 */
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
void file_sys_event(uint8_t event_code);
/*! @} */
#endif /* end of FILE_SYS_LOG */

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
void file_sys_init(void);



#define BILLING_PROFILE_XDR_RECORD_SIZE 51 /* "2+14+3+5 + (9 * 3)" 2 for structure formating, 14 for date & time, 3 for meter status, 5 total active energy reading and (9 * 3) for Max& Min values*/

#define BILLING_PROFILE_NUMBER_OF_CAPTURE_OBJECTS      3

#define BILLING_PROFILE_MAX_RECORDS_PER_XDR   ((uint16_t)((Dlms_mXdrMaxBufferSize - 3) / BILLING_PROFILE_XDR_RECORD_SIZE))
#ifdef DLMS_ENABLED
extern const TCapture_Object BillingProfile_CaptureObjects[BILLING_PROFILE_NUMBER_OF_CAPTURE_OBJECTS];
extern const uint8_t BillingProfile_CaptureObjects_XDR_Size[BILLING_PROFILE_NUMBER_OF_CAPTURE_OBJECTS];
#endif

#ifdef TEST_BILLING_RECORD
void test_filling_billing_records(uint8_t number_of_records);
#endif

TU08 FileSystem_gStoreItemList(TU08 recordID, TU08* ptrData);
TU08 FileSystem_gGetMaxNumRecord(TU08 recordID, TU32* max_records_num);
TU08 FileSystem_gClearList (uint8_t record_type);
TU08 FileSystem_gInitList ( TU08 recordID , TU16 record_size );
TU08 FileSystem_gGetItemList(uint8_t recordID, TU08* ptrData, TU32 index);
TU08 FileSystem_gGetEntriesInList(TU08 recordID, TU32* entries_number);
TU08 FileSystem_gGet_ListLastIndex_Inuse(TU08 recordID, TU32* index , TU32* max_num_entries, TU32* current_num_of_entries);
TU08 FileSystem_gGetLast_ActualIndex_InUse_InList(TU08 recordID, TU32* actual_index);

#endif /* end of FILE_SYS_H */
