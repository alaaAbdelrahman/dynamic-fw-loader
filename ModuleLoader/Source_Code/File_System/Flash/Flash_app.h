/*
 * flash_app.h
 *
 * Created: 29/12/2019 01:12:04 م
 *  Author: ibrahem mostafa
 */ 
#ifndef FLASH_APP_H_
#define FLASH_APP_H_
/* ===========================================================================================
 *         Include files
 * ===========================================================================================*/
#include "config.h" 
#include "Flash.h"
#include "FLASH_Addresses.h"
#ifdef FLASH_FM25W32_ENABLE
#define FLASH_APP_SAVING_IMAGES_FEATURE
/*============================================================================================
 *         Local new types declaration
 * ===========================================================================================*/

/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
#ifdef FLASH_APP_SAVING_IMAGES_FEATURE
enum ImageTypeEnum{
  PRV_APPLICATION      = (uint8_t)0,
  CURRENT_APPLICATION  = (uint8_t)1,
  FUTURE_APPLICATION   = (uint8_t)2,
  CURRENT_BOOTLOADER   = (uint8_t)3,
  NEW_BOOTLADER        = (uint8_t)4,
  FLASH_IMAGE_MAX_TYPES
};
extern const uint8_t Info_Image_Page_Index_arr[FLASH_IMAGE_MAX_TYPES];
#endif
__packed typedef struct 
{
  uint32_t Records_number;
  uint32_t Record_index;
  uint16_t last_Sector_Num;
  uint16_t Next_Sector_Num;
  uint8_t  Next_Byte_index;//the flash address for the next (new) record address to be saved
  uint8_t  Next_Page_index;
  uint8_t  OverLapping_times;//how many times records saved reached the last place in the last sector and erased the first sector to overlapping
  uint8_t  Crc;
}Flash_Record_ID_str;

#ifdef FLASH_APP_SAVING_IMAGES_FEATURE
__packed typedef struct 
{
  uint32_t Image_Size_Bytes;
  uint8_t  Version[10];
  uint8_t  OverWrite_Times;
  uint8_t  Crc;
}Flash_Image_Info_str;
#endif
/*!
* @addtogroup FILE_SYS_EVENT
* @{
*/
typedef enum
{
  FLASH_APP_PWR_DN  = (uint8_t)0,
  FLASH_APP_NEW_HR  = (uint8_t)1,
  FLASH_APP_NEW_SEC = (uint8_t)2,
  FLASH_APP_UP_SECONDS = (uint8_t)3,

} flash_event_type_t;
/*============================================================================================
*         Global Functions Prototype
* ===========================================================================================*/
/*============================================================================================
* Function Name  FLASH_gFastRead
* ===========================================================================================*/

/*!
* @brief Flash_app_init.
*
* @par Description:
* This function is used to init. the flash driver and get the id for all records and the information about images
*
* @param [in] : None
*
* @return
* - NO_ERROR if getting data is ok ,else get one of Flash_errors_Enmt 
*/
uint8_t Flash_app_init();
/* flag to indicate any id was changed to find if it need to erase sector and save or not*/
/*!
* @brief Flash_app_test.
*
* @par Description:
* To test flash application function. Calling almost every important functions first then test save and get. There are two #define 
*    1.	TEST_FLAFH_APP_SAVING_RECORDS to test saving record in billing history, write all space 5 times and reading after each record saving.
*    2.	TEST_FLAFH_APP_READING_ONLY to test read multi record function, read all records saved.
*
* @param [in] : None
*
* @return
* None
*/
void Flash_app_test();
uint16_t flash_mapping(uint8_t record_type, uint16_t record_num);
void Flash_app_critical_data();
uint8_t Flash_app_load_backup();
uint8_t Flash_app_test_byte();
 /*!,
* @brief Flash_app_Save_Record_ID.
*
* @par Description:
* This function is used to save Id of of the required record to it's appropriate place in flash .
*
* @param [in] recordType: type of file_sys_record_type_t.
*
* @return
* - NO_ERROR if ok ,else get one of Flash_errors_Enmt 
* NOTE: 
* We didn’t make wear leveling here as it will be saved a little time, only when power down, 
* so we erased this sector before writing on it again.
*/
uint8_t Flash_app_Save_Record_ID(uint8_t recordType, uint8_t is_backup);

/*!
* @brief Flash_app_Get_Record_ID_from_flash.
*
* @par Description:
* This function is used to get Id of of the required record to it's appropriate place in flash.
*
* @param [out] ID_data_ptr : data pointer of the required record .
* @param [in] recordType: type of file_sys_record_type_t.
*
* @return
* - NO_ERROR if ok ,else get one of Flash_errors_Enmt 
*/
uint8_t Flash_app_Get_Record_ID_from_flash(uint8_t recordType, Flash_Record_ID_str* ID_data_ptr, uint8_t is_backup);
/*!
* @brief Flash_app_Set_Default_All_Records_ID.
*
* @par Description:
* This function is used to get all Ids of all records .
*
* @return
* None
*/
void Flash_app_Set_Default_All_Records_ID(void);
/*!
* @brief Get the max number of records in sectors
*
* @par Description:
* Get the max number of records can be saved in all regared sector space.
*
* @param [in] record_type Type of records in #file_sys_record_type_t
*
* @return
* - 32 bit: the max number of all records that can be used in this space either it is saved already or not.
*/
uint32_t Flash_app_get_MAX_No_Records (uint8_t record_type);

#ifdef FLASH_APP_SAVING_IMAGES_FEATURE
/*!
* @brief .
*
* @par Description:
* This function is used to save flags as indicator to images detals
*
* @param [in] ImageType       : type of ImageTypeEnum to define the new place to save the comming data via serial .
* @param [in] Self_Test_Flag  : flag to define the status of self test if 0 not used yet, if 1 self test done and ok , if 2 self test done and has problems.
*
* @return
* - NO_ERROR if ok, else get one of Flash_errors_Enmt.
*/
uint8_t Flash_app_Save_Flag_Image(uint8_t ImageType , uint8_t Self_Test_Flag);

/*!
* @brief Flash_app_Save_Image_Info.
*
* @par Description:
* This function is used to save Image Information of of the required record to it's appropriate place in flash .
*
* @param [in] ImageType: type of ImageTypeEnum.
*
* NOTE:
* The address of images are fixed and didn’t need to be handled by wear leveling concept.
* @return
* - NO_ERROR if ok ,else get one of Flash_errors_Enmt 
*/
uint8_t Flash_app_Save_Image_Info(uint8_t ImageType, uint8_t is_backup);
/*!
* @brief Flash_app_Get_Image_Info.
*
* @par Description:
* This function is used to get Image Information of of the required record to it's appropriate place in flash.
*
* @param [out] ID_data_ptr : data pointer of the required image information .
* @param [in] ImageType: type of ImageTypeEnum.
*
* @return
* - NO_ERROR if ok, else get one of Flash_errors_Enmt.
*/
uint8_t Flash_app_Get_Image_Info(uint8_t ImageType, Flash_Image_Info_str* Info_data_ptr , uint8_t is_backup);

void Flash_app_Image_Test();
uint8_t Flash_app_Erase_Image( uint8_t ImageType, uint32_t byte_numbers);

/*!
* @brief .
*
* @par Description:
* This function is used to save bytes to wanted image place in external flash.
*
* @param [in] data_ptr       : data pointer of the data sent to be saved in wanted image place .
* @param [in] ImageType      : type of ImageTypeEnum.
* @param [in] data_size      : data size to be saved with this length.
* @param [in] shift_add      : shift address.
* @param [in] is_last_Bytes  : flag to indicate it is a last sending bytes to update Flash_Image_Info_str .
*
* @return
* - NO_ERROR if ok, else get one of Flash_errors_Enmt.
*/
uint8_t Flash_app_Save_Bytes_To_Image( uint8_t ImageType, uint8_t* data_ptr, uint16_t data_size, uint32_t shift_add, uint8_t is_last_Bytes);
#endif
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
* - NO_ERROR if ok, else get one of Flash_errors_Enmt.
*
* @note
* - Clearing storage media is not done by reset pages in storage media but, only by reset index
*   and number of saved records to be overwritten by coming records.
*/
uint8_t Flash_app_clear_Record(uint8_t record_type);
/*!
* @brief Flash_app_Set_Default_Record_ID.
*
* @par Description:
* This function is used to set the default of Flash_Record_ID_arr.
*
* @param [in] recordType: type of file_sys_record_type_t.
* @return
* - None.
*/
void Flash_app_Set_Default_Record_ID(uint8_t recordType);

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
* - 32 bit: the number of saved records.
*/
uint32_t flash_app_get_index_record(uint8_t record_type);

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
void Flash_app_clear_log(uint8_t record_type);
#endif
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
* - 23 bit: the number of saved records.
*/
uint32_t Flash_app_get_num_record(uint8_t record_type);

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
* - TRUE if function works correctly.
*
* @note
* If we can't access the storage media, a communication error event is published.
*/
uint8_t Flash_app_get_record ( uint8_t record_type , uint32_t record_num , uint8_t *data_ptr , uint32_t WantedRecords_Num );

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
uint16_t Flash_app_get_mlti_rec(uint8_t record_type, uint16_t from, uint32_t num_rec, uint8_t *data_ptr);

uint8_t Flash_app_Generic_Write ( uint16_t Byte_index , uint8_t Page_index , uint16 Sector_index , uint8_t* Data_Ptr , uint16_t DataSize );
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
* - NO_ERROR if ok, else get one of Flash_errors_Enmt.
*
* @note
* If we can't access the storage media, a communication error event is published.
*/
uint8_t Flash_app_save_record(uint8_t record_type, uint8_t *data_ptr);
/*! @} */

/*! @addtogroup FILE_SYS_EVENT
* @{
*/
/*!
* @brief Tell flash module with a new event.
*
* @par Description:
* Tell flash with new event.
*
* @param [in] event_code Code of event #file_sys_event_type_t
*
* @return
* - None.
*/
void Flash_app_event(uint8_t event_code);
#endif
uint8_t earse_sector(uint16_t sec_nom,uint8_t index);
#endif /* FLASH_APP_H_ */