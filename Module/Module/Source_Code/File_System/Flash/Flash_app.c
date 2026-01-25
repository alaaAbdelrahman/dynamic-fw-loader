/*
* flash_app.h
*
* Created: 29/12/2019 01:12:04 م
*  Author: ibrahem mostafa
*/ 
/*============================================================================================
*         Include Files
* ===========================================================================================*/
#include "Flash_app.h"
#include "file_sys.h"
#include "string.h"
#include "basic_srv.h"
//#include "ctrl.h"
//#include "trf_pymt.h"
#include "Timer_Interface.h"
#include "config.h"
#if (MicroController == Micro_V94XX)
#include "V94XX_ISR.h"

#else
#include "v85xx_isr.h"

#endif
//#include "display.h"
#ifdef FLASH_FM25W32_ENABLE
/*============================================================================================
*         Local new types declaration
* ===========================================================================================*/
#define U32_MAX_VALUE                   4294967295


 enum {
  GET_ID_FROM_FLASH_DONE  = (uint8_t) 1,
  ERASED_ID_WAS_HAPPEN    = (uint8_t) 2,
  CHANGED_ID_AFTER_ERASED = (uint8_t) 4,
};
/*------------------------------------------------------------------------------------------
Constants and Definitions
------------------------------------------------------------------------------------------*/
/*array of page index for id for records, every id was put in one page and the remaining bytes in the page was reserved, page index can be from 0 to 15*/
const uint8_t ID_Records_Page_Index_arr[] =  {
  BILLING_ID_PAGE_INDEX,
  EVENT_RECORD_ID_PAGE_INDEX,
  MONEY_ID_PAGE_INDEX,
  LOAD_PROFILE_ID_PAGE_INDEX,
  CONFIG_METER_ID_PAGE_INDEX,
#ifdef DLMS_ENABLED
  CONFIG_MD_PAGE_INDEX,
  DLMS_RECORDS_PAGE_INDEX,//1
  DLMS_RECORDS_PAGE_INDEX,//2
  DLMS_RECORDS_PAGE_INDEX,//3
  DLMS_RECORDS_PAGE_INDEX,//4
  DLMS_RECORDS_PAGE_INDEX,//5
  DLMS_RECORDS_PAGE_INDEX,//6
  DLMS_RECORDS_PAGE_INDEX,//7
  DLMS_RECORDS_PAGE_INDEX,//8
  DLMS_RECORDS_PAGE_INDEX,//9
  DLMS_RECORDS_PAGE_INDEX,//10
  DLMS_RECORDS_PAGE_INDEX,//11
#endif
};
/* array of start sector number for records data , sector number can be : 0 to 1023 */
const uint16_t Records_Sector_Start_arr[] =  {
  BILLING_RECORD_SECTOR_START_ADD ,
  EVENT_RECORD_SECTOR_START_ADD ,
  MONEY_RECORD_SECTOR_START_ADD ,
  PROFILE_SECTOR_START_ADD ,
  CONFIG_RECORD_SECTOR_START_ADD ,
#ifdef DLMS_ENABLED
  MD_RECORD_SECTOR_START_ADD ,
  DLMS_RECORD_SECTOR_1_START_ADD ,
  DLMS_RECORD_SECTOR_2_START_ADD ,
  DLMS_RECORD_SECTOR_3_START_ADD ,
  DLMS_RECORD_SECTOR_4_START_ADD ,
  DLMS_RECORD_SECTOR_5_START_ADD ,
  DLMS_RECORD_SECTOR_6_START_ADD ,
  DLMS_RECORD_SECTOR_7_START_ADD ,
  DLMS_RECORD_SECTOR_8_START_ADD ,
  DLMS_RECORD_SECTOR_9_START_ADD ,
  DLMS_RECORD_SECTOR_10_START_ADD ,
  DLMS_RECORD_SECTOR_11_START_ADD ,
#endif
};
/* array of the last or the end sector number for records data , sector number can be : 0 to 1023 */
const uint16_t Records_Sector_End_arr[] =  {
  BILLING_RECORD_SECTOR_END_ADD ,
  EVENT_RECORD_SECTOR_END_ADD ,
  MONEY_RECORD_SECTOR_END_ADD ,
  PROFILE_SECTOR_END_ADD ,
  CONFIG_RECORD_SECTOR_END_ADD ,
#ifdef DLMS_ENABLED
  MD_RECORD_SECTOR_END_ADD ,
  DLMS_RECORD_SECTOR_1_END_ADD ,
  DLMS_RECORD_SECTOR_2_END_ADD ,
  DLMS_RECORD_SECTOR_3_END_ADD ,
  DLMS_RECORD_SECTOR_4_END_ADD ,
  DLMS_RECORD_SECTOR_5_END_ADD ,
  DLMS_RECORD_SECTOR_6_END_ADD ,
  DLMS_RECORD_SECTOR_7_END_ADD ,
  DLMS_RECORD_SECTOR_8_END_ADD ,
  DLMS_RECORD_SECTOR_9_END_ADD ,
  DLMS_RECORD_SECTOR_10_END_ADD ,
  DLMS_RECORD_SECTOR_11_END_ADD ,
#endif
};

#ifdef FLASH_APP_SAVING_IMAGES_FEATURE
/* array of page index for id for images ,every id was put in one page and the remaining bytes in the page was reserved, page index can be from 0 to 15*/
const uint8_t Info_Image_Page_Index_arr[FLASH_IMAGE_MAX_TYPES] =  {
  PRV_IMAGE_INFO_PAGE_INDEX,
  CURRENT_IMAGE_INFO_PAGE_INDEX,
  FUTURE_IMAGE_INFO_PAGE_INDEX,
  CURRENT_BOOTLOADER_INFO_PAGE_INDEX,
  NEW_BOOTLOADER_INFO_PAGE_INDEX,
};
const uint16_t Images_Sector_Start_arr[FLASH_IMAGE_MAX_TYPES] =  {
  PRV_IMAGE_SECTOR_START_ADD ,
  CURRENT_IMAGE_SECTOR_START_ADD ,
  NEW_IMAGE_SECTOR_START_ADD ,
  BOOT_IMAGE_SECTOR_START_ADD ,
  NEW_BOOT_IMAGE_SECTOR_START_ADD ,
};
Flash_Image_Info_str Flash_Image_Info_arr[FLASH_IMAGE_MAX_TYPES];
#endif
Flash_Record_ID_str Flash_Record_ID_arr[FILE_SYS_MAX_LOGS];
uint8_t changed_ID_flag = false;
/*============================================================================================
*         Global Functions Prototype
* ===========================================================================================*/
/*!
* @brief Flash_app_test
*
* @par Description:
* This function is used to test flash_app functions.
*
* @param :None.
*
* @return
* - status.
*/
#define TEST_FLAFH_APP_SAVING_RECORDS  1
#define TEST_FLAFH_APP_READING_ONLY    0
#define TEST_FLASH_APP                 TEST_FLAFH_APP_SAVING_RECORDS
#undef TEST_FLASH_APP
#ifdef TEST_FLASH_APP
volatile uint16 from_temp = 0;
uint16_t gff=117,df=0;
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
void Flash_app_test()
{
  static uint8_t status = 0;
  static trf_pymt_bph_rec_t temp_billing ;
  static trf_pymt_bph_rec_t temp_2_ling;
#if TEST_FLASH_APP == TEST_FLAFH_APP_READING_ONLY
  static trf_pymt_bph_rec_t temp_bill_arr[10],temp_arr_backup[10] ;
#else
  static trf_pymt_bph_rec_t temp_bill_read;
#endif
#if TEST_FLASH_APP == TEST_FLAFH_APP_SAVING_RECORDS
  status = Flash_app_init();
  Flash_app_clear_Record(FILE_SYS_BPH_RECORD);
  Flash_app_event(FLASH_APP_UP_SECONDS);
#endif
  status = Flash_app_init();
  memset ((uint8_t*)&temp_billing , 0x22 , sizeof (temp_billing));
  memset ((uint8_t*)&temp_2_ling , 0x55, sizeof (temp_billing));
#if TEST_FLASH_APP == TEST_FLAFH_APP_READING_ONLY
  for (from_temp = 0; from_temp < 10 ; )
  {
    memset ((uint8_t*)&temp_arr_backup[from_temp++] , 0x22, sizeof (temp_billing));
    memset ((uint8_t*)&temp_arr_backup[from_temp++] , 0x55, sizeof (temp_billing));
  }
#endif
#if TEST_FLASH_APP == TEST_FLAFH_APP_SAVING_RECORDS
  for (from_temp = 0; from_temp < 351*5; )
#elif TEST_FLASH_APP == TEST_FLAFH_APP_READING_ONLY
    for (from_temp = 0; from_temp < 351; )
#endif
    {
#if TEST_FLASH_APP == TEST_FLAFH_APP_SAVING_RECORDS
      if ((uint16)from_temp == (uint16)(4096/sizeof(trf_pymt_bph_rec_t) * BILLING_RECORD_SECTOR_NUMBERS) )
        from_temp = 0;
      if (from_temp == gff || from_temp == gff-1)
        status = 66;
      status = Flash_app_save_record ( FILE_SYS_BPH_RECORD , (uint8_t*)&temp_billing); 
      status = Flash_app_get_record  ( FILE_SYS_BPH_RECORD , from_temp++%Flash_Record_ID_arr[FILE_SYS_BPH_RECORD].Records_number , (uint8_t*)&temp_bill_read , 1 );
      if (memcmp ((uint8_t*)&temp_billing , (uint8_t*)&temp_bill_read ,sizeof(temp_billing)) != 0 || status!=NO_ERROR)
        status =66;
      if ((uint16)from_temp == (uint16)(4096/sizeof(trf_pymt_bph_rec_t) * BILLING_RECORD_SECTOR_NUMBERS) )
        from_temp = 0;
      status = Flash_app_save_record ( FILE_SYS_BPH_RECORD , (uint8_t*)&temp_2_ling);  
      status = Flash_app_get_record  ( FILE_SYS_BPH_RECORD , from_temp++%Flash_Record_ID_arr[FILE_SYS_BPH_RECORD].Records_number , (uint8_t*)&temp_bill_read , 1 );
      if (memcmp ((uint8_t*)&temp_2_ling , (uint8_t*)&temp_bill_read ,sizeof(temp_2_ling)) != 0|| status!=NO_ERROR)
        status =34;
      if (++df > 1000)
        break;
#elif TEST_FLASH_APP == TEST_FLAFH_APP_READING_ONLY
      status = Flash_app_get_mlti_rec ( FILE_SYS_BPH_RECORD , from_temp , 10 , (uint8_t*)&temp_bill_arr  );
      from_temp = from_temp + 10;
      if (memcmp ((uint8_t*)&temp_arr_backup , (uint8_t*)&temp_bill_arr ,sizeof(temp_bill_arr)) != 0 || status !=0)
        status = 34;
#endif
      
    }
  
  Flash_app_event(FLASH_APP_PWR_DN);
}
#endif
#undef FLASH_APP_DEBUG_BREAKPOINT
#ifdef FLASH_APP_DEBUG_BREAKPOINT
uint8 Flash_app_debug_BP ()
{
  uint8_t fdf =Flash_Record_ID_arr[0].Crc  +ID_SECTOR_ADD/100; 
  return fdf;
}
#endif
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
uint8_t Flash_app_init()
{
  uint8_t i = 0 , status = 0 , temp_status = NO_ERROR ; 
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  FLASH_gInit();
  changed_ID_flag |= GET_ID_FROM_FLASH_DONE;
  for ( i = 0 ; i < FILE_SYS_MAX_LOGS  ; i ++)
  {
    status = Flash_app_Get_Record_ID_from_flash (i , &Flash_Record_ID_arr[i],0);
    if (status != NO_ERROR)
      temp_status = status;
  }
#ifdef FLASH_APP_SAVING_IMAGES_FEATURE
  /*for ( i = 0 ; i < FLASH_IMAGE_MAX_TYPES  ; i ++)
  {
    status = Flash_app_Get_Image_Info(i,&Flash_Image_Info_arr[i],0);
    if (status != NO_ERROR)
      temp_status = status;
  }*/
  FLASH_gEraseSector_ByIndex ( FLAG_APP_SECTOR_ADD ,80);
#endif
  return temp_status;
}

uint8_t Flash_app_load_backup()
{
  uint8_t i = 0 , status = 0 , temp_status = NO_ERROR ; 
  if(eeprom_load_errors_register & FILE_SYS_ERR_LOAD_FLAG)
  {
    eeprom_load_errors_register &= ~FILE_SYS_ERR_LOAD_FLAG;
    for ( i = 0 ; i < FILE_SYS_MAX_LOGS  ; i ++)
    {
      status = Flash_app_Get_Record_ID_from_flash (i , &Flash_Record_ID_arr[i],1);
      if (status != NO_ERROR)
      {
        eeprom_load_errors_register |= FILE_SYS_ERR_LOAD_FLAG;
        temp_status = status;
      }
    }
#ifdef FLASH_APP_SAVING_IMAGES_FEATURE
    /*for ( i = 0 ; i < FLASH_IMAGE_MAX_TYPES  ; i ++)
    {
      status = Flash_app_Get_Image_Info(i,&Flash_Image_Info_arr[i],1);
      if (status != NO_ERROR)
        temp_status = status;
    }*/
#endif
  }
  return temp_status;
}
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
uint8_t Flash_app_Save_Record_ID(uint8_t recordType , uint8_t is_backup)
{
  uint8_t status = 0;
  if ( recordType >= FILE_SYS_MAX_LOGS )
    return FLASH_EXCEED_TYPE;
  uint16_t sector_num = 0 ; 
  if (is_backup == 1 )
    sector_num = BACKUP_ID_SECTOR_ADD ;
  else
    sector_num = ID_SECTOR_ADD ; 
  Flash_Record_ID_arr[recordType].Crc = calc_crc((uint8_t*)&Flash_Record_ID_arr[recordType] , sizeof(Flash_Record_ID_str) - 1 , CRC); 
  status = FLASH_gWrite_ByIndex ( 0, ID_Records_Page_Index_arr[recordType], sector_num, (uint8_t*)&Flash_Record_ID_arr[recordType], sizeof(Flash_Record_ID_str));
  
  return(status);
}
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
uint8_t Flash_app_Get_Record_ID_from_flash(uint8_t recordType, Flash_Record_ID_str* ID_data_ptr , uint8_t is_backup)
{
  uint8_t status = 0;
  if (recordType >= FILE_SYS_MAX_LOGS || ID_data_ptr == NULL)
    return FLASH_EXCEED_TYPE;
  
  uint16_t sector_num = 0 ; 
  if (is_backup == 1 )
    sector_num = BACKUP_ID_SECTOR_ADD ;
  else
    sector_num = ID_SECTOR_ADD ; 
  
  status = FLASH_gRead_ByIndex ( 0, ID_Records_Page_Index_arr[recordType] , sector_num, (uint8_t*)ID_data_ptr, sizeof(Flash_Record_ID_str));
  
  if (ID_data_ptr->Crc != calc_crc((uint8_t*)ID_data_ptr , sizeof(Flash_Record_ID_str) - 1 , CRC))
  {
      status = FLASH_APP_CRC_CORRUPT;
  }
  
  return(status);
}

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
#ifdef FLASH_APP_SAVING_IMAGES_FEATURE
uint8_t Flash_app_Save_Image_Info(uint8_t ImageType , uint8_t is_backup)
{
  uint8_t status = 0;
  if ( ImageType >= FLASH_IMAGE_MAX_TYPES )
    return FLASH_EXCEED_TYPE;
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  uint16_t sector_num = 0 ; 
  if (is_backup == 1 )
    sector_num = BACKUP_ID_SECTOR_ADD ;
  else
    sector_num = ID_SECTOR_ADD ;
  Flash_Image_Info_arr[ImageType].Crc = calc_crc((uint8_t*)&Flash_Image_Info_arr[ImageType] , sizeof(Flash_Image_Info_str) - 1 , CRC); 
  status = FLASH_gWrite_ByIndex ( 0, Info_Image_Page_Index_arr[ImageType], sector_num, (uint8_t*)&Flash_Image_Info_arr[ImageType], sizeof(Flash_Image_Info_str));
  return(status);
}
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
uint8_t Flash_app_Get_Image_Info(uint8_t ImageType, Flash_Image_Info_str* Info_data_ptr , uint8_t is_backup)
{
  uint8_t status = 0;
  if ( ImageType >= FLASH_IMAGE_MAX_TYPES || Info_data_ptr == NULL)
    return FLASH_EXCEED_TYPE;
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  uint16_t sector_num = 0 ; 
  if (is_backup == 1 )
    sector_num = BACKUP_ID_SECTOR_ADD ;
  else
    sector_num = ID_SECTOR_ADD ;
  
  status = FLASH_gRead_ByIndex ( 0, Info_Image_Page_Index_arr[ImageType] , sector_num, (uint8_t*)Info_data_ptr, sizeof(Flash_Image_Info_str));  
  if (Info_data_ptr->Crc != calc_crc((uint8_t*)Info_data_ptr , sizeof(Flash_Image_Info_str) - 1 , CRC))
  {
      status = FLASH_APP_CRC_CORRUPT;
  }
  return(status);
}
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
uint8_t Flash_app_Save_Bytes_To_Image( uint8_t ImageType, uint8_t* data_ptr, uint16_t data_size, uint32_t shift_add, uint8_t is_last_Bytes)
{
   uint16_t byte_index = 0;
   uint16_t start_page_index = 0;
   uint16_t sector_number = 0;
   uint8_t status = 0;
   if ( ImageType > FLASH_IMAGE_MAX_TYPES || data_ptr == NULL || (uint32_t)(shift_add + data_size) >= 256*1024UL)
     return false;
   
   sector_number = Images_Sector_Start_arr[ImageType] + shift_add/FLASH_SECTOR_SIZE;
   if (shift_add >= FLASH_SECTOR_SIZE)
   {
     start_page_index = (shift_add%FLASH_SECTOR_SIZE)/NUM_BYTES_PER_PAGE;
     byte_index = (shift_add%FLASH_SECTOR_SIZE)%NUM_BYTES_PER_PAGE;
   }
   else
   {
     start_page_index = (shift_add)/NUM_BYTES_PER_PAGE;
     byte_index = shift_add%NUM_BYTES_PER_PAGE;
   }

   status = FLASH_gWrite_ByIndex (  byte_index,  start_page_index,  sector_number, data_ptr , data_size);
  /* //just for test
    uint8_t read_arr[128];
    status = FLASH_gRead_ByIndex (  byte_index,  start_page_index,  sector_number, read_arr , 128>data_size? data_size: 128);
   if (memcmp(read_arr ,data_ptr ,  128>data_size? data_size: 128 ) !=0 )
   {
    return 11; //todo do it or not? to check as ramsource?
   }
*/
   if (is_last_Bytes == 1 && NO_ERROR == status)
   {
     Flash_app_Get_Image_Info( ImageType, &Flash_Image_Info_arr[ImageType],0);
     Flash_Image_Info_arr[ImageType].Image_Size_Bytes = shift_add + data_size;
     Flash_Image_Info_arr[ImageType].OverWrite_Times++;
     //memcpy(&Flash_Image_Info_arr[ImageType].Version , FIRMWARE_VERSION, sizeof(FIRMWARE_VERSION));
     eeprom_load_errors_register &= ~FILE_SYS_ERR_LOAD_FLAG;
     Flash_app_critical_data();
   }
   return status;
}
void Flash_app_critical_data()
{
     changed_ID_flag &= ~ERASED_ID_WAS_HAPPEN;
     changed_ID_flag |= CHANGED_ID_AFTER_ERASED;
     Flash_app_event (FLASH_APP_PWR_DN);
}
uint8_t Flash_app_test_byte()
{
  uint8_t write_byte = 5  ,read_byte =0  ; 
  FLASH_gEraseSector_ByIndex(999,100);
  FLASH_gWrite_ByIndex ( 0 , 0 , 999 , &write_byte , 1);
  FLASH_gRead_ByIndex ( 0 , 0 , 999 , &read_byte , 1);
  if ( read_byte != write_byte )
    return false;
  write_byte = 0xAA;
  read_byte = 0;
  FLASH_gEraseSector_ByIndex(999,100);
  FLASH_gWrite_ByIndex ( 0 , 0 , 999 , &write_byte , 1);
  FLASH_gRead_ByIndex ( 0 , 0 , 999 , &read_byte , 1);
  if ( read_byte == write_byte )
    return true;
  else
    return false;
}
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
uint8_t Flash_app_Save_Flag_Image(uint8_t ImageType , uint8_t Self_Test_Flag)
{
  uint8_t status = 1;
  uint8_t flags[3]={0};
  changed_ID_flag &= ~ERASED_ID_WAS_HAPPEN;
  Flash_app_event(FLASH_APP_PWR_DN);//to save the id and the new types for images
  FLASH_gEraseSector_ByIndex(FLAG_SECTOR_ADD,80);
  flags[0]=ImageType;
  flags[1]=Self_Test_Flag;
  flags[2]=calc_crc(flags,2,CRC);
  //status = FLASH_gWrite_ByIndex ( 0 , FLAG_IMAGE_INDICATION_PAGE_INDEX , FLAG_SECTOR_ADD , (uint8_t*)&ImageType   , 1);
 // status = FLASH_gWrite_ByIndex ( 1 , FLAG_IMAGE_INDICATION_PAGE_INDEX , FLAG_SECTOR_ADD , (uint8_t*)&Self_Test_Flag , 1);
  status = FLASH_gWrite_ByIndex ( 0 , FLAG_IMAGE_INDICATION_PAGE_INDEX , FLAG_SECTOR_ADD , flags , 3);
  
  return status;
}

uint8_t Flash_app_Erase_Image( uint8_t ImageType, uint32_t byte_numbers)
{
  int32_t i       = byte_numbers/FLASH_SECTOR_SIZE;
  uint8_t  status = 0 ;
  
  if (byte_numbers > (uint32_t)256*1024)
    return 10;
  for ( ; i >= 0 ; i--)
  {
    status = FLASH_gEraseSector_ByIndex ( (Images_Sector_Start_arr[ImageType] + i ),50);  //TODO increase delay to 80ms in this line , if sw increase timeout (A_E)
  }
  Delay_ms(80);
  return status;
}
/*
    uint8_t reference_array [4097];
 uint8_t read_arr[4097];
void Flash_app_Image_Test( )
{
  uint8_t arr[1024] ;
  
  uint8_t status     = 0;
  uint32_t shift_add = 5,i=0,sector_number=0;
  uint8_t value = 0;
  
  if (status == 5)
  {
    Flash_app_Erase_Image(PRV_APPLICATION , 1024*51);
    status = FLASH_gRead_ByIndex (  0,  0,  Images_Sector_Start_arr[PRV_APPLICATION] , read_arr , 4096);
    
    
    for (shift_add = 0 ; shift_add < 1024*50; shift_add+= 1024 )
    {
      memset (arr , value++ , 1024);
      status = Flash_app_Save_Bytes_To_Image ( PRV_APPLICATION, arr, sizeof(arr)  , shift_add, 0);
    }
    status   = Flash_app_Save_Bytes_To_Image ( PRV_APPLICATION  , arr, sizeof(arr), shift_add , 1);
    
    
    memset (reference_array, 1 ,4096 );
    sector_number = Images_Sector_Start_arr[PRV_APPLICATION]  ;
    for (i = 0 ; i< 1024*51/4096 ; i++)
    {
      status = FLASH_gRead_ByIndex (  0,  0,  sector_number++, read_arr , 4096);
      if (memcmp(reference_array , read_arr , 4096))
        status =10;
    }
  }
}
*/
#endif
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
void Flash_app_Set_Default_Record_ID(uint8_t recordType)
{
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  memset (&Flash_Record_ID_arr[recordType] , 0 , sizeof(Flash_Record_ID_arr[recordType]));
  Flash_Record_ID_arr[recordType].Next_Sector_Num = Records_Sector_Start_arr[recordType];
  Flash_Record_ID_arr[recordType].last_Sector_Num = Flash_Record_ID_arr[recordType].Next_Sector_Num;
  Flash_Record_ID_arr[recordType].Crc = calc_crc((uint8_t*)&Flash_Record_ID_arr[recordType] , sizeof(Flash_Record_ID_arr[recordType]) - 1 , CRC);
  changed_ID_flag |= CHANGED_ID_AFTER_ERASED;
}
/*!
* @brief Flash_app_Set_Default_All_Records_ID.
*
* @par Description:
* This function is used to get all Ids of all records .
*
* @return
* None
*/
void Flash_app_Set_Default_All_Records_ID(void)
{
  uint8_t ii=0;
  for ( ii = 0 ; ii < FILE_SYS_MAX_LOGS  ; ii ++)
  {
    Flash_app_Set_Default_Record_ID(ii);
  }
  //memset ( Flash_Image_Info_arr , 0 , sizeof(Flash_Image_Info_arr) );
}
#ifdef FLASH_APP_GENERIC_FEATURE
static uint8 Flash_array[256];

uint8_t Flash_app_Generic_Write ( uint16_t Byte_index , uint8_t Page_index , uint16 Sector_index , uint8_t* Data_Ptr , uint16_t DataSize )
{
  uint8_t status = 0 , page_temp = 0;
  uint16_t dataSize_temp = DataSize;
  
  if(Page_index > 15)
    return FLASH_EXCEED_TYPE;
  
  FLASH_gEraseSector_ByIndex ( BACK_UP_SECTOR,80 );      
  for ( ; page_temp< 16 ; page_temp++)
  {
    FLASH_gRead_ByIndex ( 0 ,page_temp , Sector_index , Flash_array , sizeof(Flash_array));//read from wanted sector
    if ( (Page_index + Byte_index/256) ==  page_temp || status == 5)
    {
      if ( DataSize > ( 256 - (Byte_index%256) ) )
      {
        dataSize_temp = 256 - (Byte_index%256);
      }
      memcpy ( &Flash_array[Byte_index] , Data_Ptr , dataSize_temp);
      dataSize_temp = DataSize - dataSize_temp;
      Byte_index = 0;
      status = 5;
    }
    else
      status = 0;
    FLASH_gWrite_ByIndex ( 0 ,page_temp , BACK_UP_SECTOR , Flash_array , sizeof(Flash_array));//write in backup sector
  }
  FLASH_gEraseSector_ByIndex ( Sector_index ,80);   //erase wanted sector to write in it what was in backup sector
  for ( page_temp = 0 ; page_temp < 16 ; page_temp++)
  {
    FLASH_gRead_ByIndex ( 0 , page_temp , BACK_UP_SECTOR , Flash_array , sizeof(Flash_array));
    status = FLASH_gWrite_ByIndex ( 0 , page_temp , Sector_index , Flash_array , sizeof(Flash_array));
  }

  return status;
}
#endif
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
uint8_t Flash_app_save_record(uint8_t record_type, uint8_t *data_ptr)
{
  uint8_t  status     = 0;
  uint32_t MAX_No_Records = 0;
  
  if ( record_type >= FILE_SYS_MAX_LOGS || (eeprom_load_errors_register & FILE_SYS_ERR_LOAD_FLAG))
    return FLASH_EXCEED_TYPE;
  
  if (Flash_Record_ID_arr[record_type].Next_Page_index == 0 &&
      Flash_Record_ID_arr[record_type].Next_Byte_index  == 0 )
  {
    FLASH_gEraseSector_ByIndex ( Flash_Record_ID_arr[record_type].Next_Sector_Num,80);
  }
  status = FLASH_gWrite_ByIndex ( Flash_Record_ID_arr[record_type].Next_Byte_index ,
                                 Flash_Record_ID_arr[record_type].Next_Page_index ,
                                 Flash_Record_ID_arr[record_type].Next_Sector_Num ,
                                 data_ptr ,
                                 size_of_record[record_type]);
  
  changed_ID_flag |= CHANGED_ID_AFTER_ERASED;

  if (status == NO_ERROR)
  {
    MAX_No_Records = Flash_app_get_MAX_No_Records(record_type);
    Flash_Record_ID_arr[record_type].last_Sector_Num = Flash_Record_ID_arr[record_type].Next_Sector_Num;
    /* increment Next_Byte_index and check if exceed the page : increment Next_Page_index , 
    and then check if exceed the sector: increment Next_Sector_Num */
    /* here 2*size_of_record[record_type] to represent it's size and the next record should has a complete space also*/
    if ( (Flash_Record_ID_arr[record_type].Next_Page_index == NUM_PAGES_PER_SECTOR -1) &&
        (Flash_Record_ID_arr[record_type].Next_Byte_index + 2*size_of_record[record_type] > FLASH_PAGE_SIZE-1))
    {
      Flash_Record_ID_arr[record_type].Next_Byte_index = 0;
      Flash_Record_ID_arr[record_type].Next_Page_index = 0;
      Flash_Record_ID_arr[record_type].Next_Sector_Num ++;
      /* check if next sector is not in this record secorts range , overlapping happen*/
      if ((uint16_t)Flash_Record_ID_arr[record_type].Next_Sector_Num > (uint16_t)Records_Sector_End_arr[record_type])
      {
        Flash_Record_ID_arr[record_type].Next_Sector_Num = Records_Sector_Start_arr[record_type];
        Flash_Record_ID_arr[record_type].OverLapping_times++;
      }
    }
    else
    {
      if (Flash_Record_ID_arr[record_type].Next_Byte_index + size_of_record[record_type] > FLASH_PAGE_SIZE-1)
        Flash_Record_ID_arr[record_type].Next_Page_index ++;
      
      Flash_Record_ID_arr[record_type].Next_Byte_index =  (Flash_Record_ID_arr[record_type].Next_Byte_index + size_of_record[record_type]) %FLASH_SECTOR_SIZE;
    }
    
    Flash_Record_ID_arr[record_type].Record_index ++;
    
    if (Flash_Record_ID_arr[record_type].Record_index > MAX_No_Records)
      Flash_Record_ID_arr[record_type].Record_index = 1;
    
    if (( Flash_Record_ID_arr[record_type].OverLapping_times == 0 ) ||
        (Flash_Record_ID_arr[record_type].last_Sector_Num == Records_Sector_End_arr[record_type]) )
      Flash_Record_ID_arr[record_type].Records_number++;
    else 
      Flash_Record_ID_arr[record_type].Records_number = (Records_Sector_End_arr[record_type] - Records_Sector_Start_arr[record_type] ) \
        * FLASH_SECTOR_SIZE/size_of_record[record_type];
  }
  // todo make record_index 32bit
  // todo make num_saved_record 32bit
  //ctrl_sys_var_optional_feature.num_saved_record[record_type] =Flash_Record_ID_arr[record_type].Records_number;

  if (status == NO_ERROR) 
    status = true;
  return( status);
}
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
uint32_t Flash_app_get_MAX_No_Records (uint8_t record_type)
{
  uint32_t MAX_No_Records = 0;
  uint32_t max_temp = (Records_Sector_End_arr[record_type] - Records_Sector_Start_arr[record_type] +1);
  
  if ( Flash_Record_ID_arr[record_type].OverLapping_times > 0 && Flash_Record_ID_arr[record_type].last_Sector_Num != Records_Sector_End_arr[record_type])
    MAX_No_Records = (Records_Sector_End_arr[record_type] - Records_Sector_Start_arr[record_type]);
  else
    MAX_No_Records = max_temp;
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  MAX_No_Records = MAX_No_Records * FLASH_SECTOR_SIZE/size_of_record[record_type];
  return MAX_No_Records;
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
* - 32 bit: the number of saved records.
*/
uint32_t flash_app_get_index_record(uint8_t record_type)
{
  if (record_type >= FILE_SYS_MAX_LOGS)
    return U32_MAX_VALUE;
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  return Flash_Record_ID_arr[record_type].Record_index;
}
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
uint8_t Flash_app_clear_Record(uint8_t record_type)
{  
  if (record_type >= FILE_SYS_MAX_LOGS)
    return FLASH_EXCEED_TYPE;
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  Flash_app_Set_Default_Record_ID(record_type);
  return NO_ERROR;
}
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
* - 32 bit: the number of saved records.
*/
uint32_t Flash_app_get_num_record(uint8_t record_type)
{
  if (record_type >= FILE_SYS_MAX_LOGS)
    return U32_MAX_VALUE;
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  return ( Flash_Record_ID_arr[record_type].Records_number );
}

uint16_t flash_mapping(uint8_t record_type, uint16_t record_num)
{
  uint16_t num;
  num = record_num;
  if (Flash_Record_ID_arr[record_type].Records_number> max_num_record[record_type])
  {
    if (record_num <= (Flash_Record_ID_arr[record_type].Records_number-1)%max_num_record[record_type])
    {
      num = ((Flash_Record_ID_arr[record_type].Records_number-1)/max_num_record[record_type])*max_num_record[record_type] +record_num; 
    }
    else
    {
      num = ((Flash_Record_ID_arr[record_type].Records_number-1)/max_num_record[record_type])*max_num_record[record_type] +record_num - max_num_record[record_type]; 
    }
  }
  return num;
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
* @param [in] WantedRecords_Num number of wanted records.
* @param [out] data_ptr   Address of buffer to get the data in
*
* @return
* - NO_ERROR if ok, else get one of Flash_errors_Enmt.
*
* @note
* If we can't access the storage media, a communication error event is published.
*/

uint8_t Flash_app_get_record ( uint8_t recordType , uint32_t record_num , uint8_t *data_ptr , uint32_t WantedRecords_Num )
{
  uint32_t Required_Shift   = 0;
  uint16_t No_Records_per_Sector = 0; 
  uint16_t Sector_Index     = 0;
  uint16_t Req_sector_num   = 0;    /* the real byte address*/
  uint8_t  Req_Byte_index   = 0;
  uint8_t  Req_Page_index   = 0;
  static uint8_t  Status    = NO_ERROR;
  
  if ( (recordType >= FILE_SYS_MAX_LOGS) || (WantedRecords_Num + record_num - 1 >= Flash_Record_ID_arr[recordType].Records_number) \
    || WantedRecords_Num == 0 || data_ptr == NULL )
    return FLASH_EXCEED_TYPE;
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  No_Records_per_Sector = FLASH_SECTOR_SIZE / size_of_record[recordType];
  /* if overlapping happens, get last record and note : the remainig place in this sector after the last record was empty because of overlapping,*/
  if ( (Flash_Record_ID_arr[recordType].OverLapping_times != 0) /*|| (Flash_Record_ID_arr[recordType].last_Sector_Num == Records_Sector_End_arr[recordType])*/ )
  {
    /*get the first sector (but the new start as overlapping happend ,so currentsector++ will be the new start ) 
    if the current sector was incremented, there are 2 probapilities, currentsector++ or overlapping if out of range*/
    Req_sector_num = Records_Sector_Start_arr[recordType];
    
    /*the new start in this new sector may not be the first address of this sector , as it depened on the shift in the current sector
    , here Required_Shift will be no_of_records */
    Required_Shift = record_num;
    while (Required_Shift >= No_Records_per_Sector)
    {
      Required_Shift -=  No_Records_per_Sector;
      Req_sector_num ++ ;
    }
    /* find the wanted sector of the wanted byte ,after RemainingRecords , the new start will be at this point in this sector*/
    
    if (Req_sector_num > Records_Sector_End_arr[recordType])
    {
      Req_sector_num = (Req_sector_num - Records_Sector_End_arr[recordType] - 1) + Records_Sector_Start_arr[recordType];
    }
    Req_Page_index = (uint8_t)(Required_Shift*size_of_record[recordType]/FLASH_PAGE_SIZE);
    if (Required_Shift > 0)
      Req_Byte_index = (uint8_t)(Required_Shift *size_of_record[recordType]- Req_Page_index *FLASH_PAGE_SIZE);
    else
      Req_Byte_index = 0;
  }
  else
  {
    /*there is no overlapping */
    Sector_Index = (uint16_t)((uint32_t)record_num / No_Records_per_Sector);
    
    if ( record_num >= No_Records_per_Sector)
      Required_Shift = record_num - No_Records_per_Sector * Sector_Index ; 
    else
      Required_Shift = record_num ;
    
    Req_sector_num =  Records_Sector_Start_arr[recordType] + Sector_Index;
    Req_Page_index = (uint8_t)(Required_Shift*size_of_record[recordType]/FLASH_PAGE_SIZE);
    if (Required_Shift > 0)
      Req_Byte_index = (uint8_t)(Required_Shift *size_of_record[recordType]- Req_Page_index *FLASH_PAGE_SIZE);
    else
      Req_Byte_index = 0;
  }
  /* read from flash , but check first the wanted number of records you need*/
  if (WantedRecords_Num == 1)
    Status = FLASH_gRead_ByIndex ( Req_Byte_index, Req_Page_index , Req_sector_num, data_ptr, size_of_record[recordType]);  
  else
  {
    if ( No_Records_per_Sector - Required_Shift >= WantedRecords_Num)
    {
      Status = FLASH_gRead_ByIndex ( Req_Byte_index, Req_Page_index , Req_sector_num, data_ptr, size_of_record[recordType] * WantedRecords_Num);  
    }
    else
    {
      Required_Shift = No_Records_per_Sector - Required_Shift;
      Status = FLASH_gRead_ByIndex ( Req_Byte_index, Req_Page_index , Req_sector_num, data_ptr, size_of_record[recordType] * Required_Shift); 
      data_ptr = data_ptr + size_of_record[recordType] * Required_Shift;
      
      WantedRecords_Num -= Required_Shift;
      
      Req_Page_index = 0;
      while ( WantedRecords_Num > 0 && Status == NO_ERROR )
      {
        if ( WantedRecords_Num > No_Records_per_Sector)
          Required_Shift = No_Records_per_Sector;
        else
          Required_Shift = WantedRecords_Num;
        
        Req_sector_num ++ ;
        if (Req_sector_num > Records_Sector_End_arr[recordType])
          Req_sector_num = Records_Sector_Start_arr[recordType];
        
        Req_Byte_index = 0;
        Status = FLASH_gRead_ByIndex ( Req_Byte_index, Req_Page_index , Req_sector_num, data_ptr, size_of_record[recordType] * Required_Shift );  
        Req_Page_index++;
        WantedRecords_Num -= Required_Shift;
      }
    }
  }
  
  return Status;
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
* - NO_ERROR if ok, else get one of Flash_errors_Enmt.
*/
uint16_t Flash_app_get_mlti_rec(uint8_t record_type, uint16_t from, uint32_t num_rec, uint8_t *data_ptr)
{
  uint8_t status = 0 ;
  
  status = Flash_app_get_record (record_type, from , data_ptr , num_rec );
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  return status ;
}
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
void Flash_app_event(uint8_t event_code)
{
  uint8_t i =0;
#ifdef FLASH_APP_DEBUG_BREAKPOINT
  Flash_app_debug_BP ();
#endif
  /*!@par Pseudo Code:*/
  switch(event_code)
  {
  case FLASH_APP_UP_SECONDS:
  case FLASH_APP_PWR_DN:
    if(!(eeprom_load_errors_register & FILE_SYS_ERR_LOAD_FLAG))
    {
    /* if flag wasn't changed , you don't need to save or erase. and if get_id wasn't called, there was unacceptable behaviour */
    if (!(changed_ID_flag & (CHANGED_ID_AFTER_ERASED|GET_ID_FROM_FLASH_DONE)))
      return;
    
    if (!(changed_ID_flag & ERASED_ID_WAS_HAPPEN))
    {
      FLASH_gEraseSector_ByIndex(ID_SECTOR_ADD,80);
      changed_ID_flag |= ERASED_ID_WAS_HAPPEN;
      changed_ID_flag &= ~CHANGED_ID_AFTER_ERASED;
    }
    if (event_code == FLASH_APP_UP_SECONDS)
    {
      FLASH_gEraseSector_ByIndex(BACKUP_ID_SECTOR_ADD,80);
     /* for (i = 0 ; i < FLASH_IMAGE_MAX_TYPES ; i++)
        Flash_app_Save_Image_Info (i,1);*/
      for (i = 0 ; i < FILE_SYS_MAX_LOGS ; i++)
        Flash_app_Save_Record_ID (i,1);
    }
    /* FLASH_APP_NEW_DAY only used to erase sector if not erased. */
    if (event_code != FLASH_APP_PWR_DN)
      return;
    /*the same previous behaviour we need to PWR_DN event but in addition to saving ID*/
#ifdef FLASH_APP_SAVING_IMAGES_FEATURE
    for (i = 0 ; i < FLASH_IMAGE_MAX_TYPES ; i++)
    {
      Flash_app_Save_Image_Info (i,0);
    }
#endif
    for (i = 0 ; i < FILE_SYS_MAX_LOGS ; i++)
    {
      Flash_app_Save_Record_ID (i,0);
    }
    changed_ID_flag &= ~ERASED_ID_WAS_HAPPEN;
    }
    break;
  case FLASH_APP_NEW_HR:
  case FLASH_APP_NEW_SEC:
    break;
  }
}
uint8_t earse_sector(uint16_t sec_nom,uint8_t index)
{
  uint8_t i;
  uint8_t status=0;
  for ( i=0; i<index; i++)
  {
    status = FLASH_gEraseSector_ByIndex ( sec_nom,80);
    sec_nom++;
  }
  return status;
}
#endif