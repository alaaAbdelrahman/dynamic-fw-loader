/*
 * flash_intf.h
 *
 * Created: 29/04/2018 01:12:04 م
 *  Author: reda.abdelhameid
 */ 


#ifndef FLASH_ADDRESS_H
#define FLASH_ADDRESS_H
/*
=========================================================================
 *         Include files
 * ===========================================================================================*/
#include "config.h" 
#define FLASH_SECTOR_SIZE       (uint16_t)4096
#define NUM_PAGES_PER_SECTOR    (uint8_t)16
#define MAX_NUM_SECTORS         (uint16_t)1024
#define NUM_BYTES_PER_PAGE      (uint16_t)256
   
/************************************* general addressess *************************************/
#define Optical_Managment_guek_Key_ID_START_PAGE_INDEX           (uint8_t)0 
#define Optical_Managment_guek_Key_ID_END_PAGE_INDEX             (uint8_t)3 
#define Optical_Managment_guek_Key_ID_SECTOR                     297//sector  

#define ASSO_NAME_SECURITY0_ID_START_PAGE_INDEX                  (uint8_t)4
#define ASSO_NAME_SECURITY0_ID_END_PAGE_INDEX                    (uint8_t)4
#define ASSO_NAME_SECURITY0_ID_SECTOR                            297//sector

#define PW_MANAGER_CLIENT_OPTO_ID_START_PAGE_INDEX               (uint8_t)5
#define PW_MANAGER_CLIENT_OPTO_ID_END_PAGE_INDEX                 (uint8_t)5
#define PW_MANAGER_CLIENT_OPTO_ID_SECTOR                         297//sector

#define HDLC_PHYSICAL_ID_ID_BYTE_INDEX                           (uint8_t)0
#define HDLC_PHYSICAL_ID_ID_START_PAGE_INDEX                     6
#define HDLC_PHYSICAL_ID_ID_END_PAGE_INDEX                       6
#define HDLC_PHYSICAL_ID_ID_SECTOR                               297//sector

/******************************** Dot Matrix LCD Table address ***********************************/   
#if defined (DOT_MATRIX_LCD_ENABLE)

/************************************ LANGUAGE 1 ***************************************/
#define LN1_LCD_SECTOR_NUM_SCREEN_0                                         880
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_0                                     0X370000

#define LN1_LCD_SECTOR_NUM_SCREEN_1                                         881
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_1                                     0X371000

#define LN1_LCD_SECTOR_NUM_SCREEN_2                                         882
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_2                                     0X372000

#define LN1_LCD_SECTOR_NUM_SCREEN_3                                         883
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_3                                     0X373000

#define LN1_LCD_SECTOR_NUM_SCREEN_4                                         884
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_4                                     0X374000 

#define LN1_LCD_SECTOR_NUM_SCREEN_5                                         885
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_5                                     0X375000

#define LN1_LCD_SECTOR_NUM_SCREEN_6                                         886
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_6                                     0X376000

#define LN1_LCD_SECTOR_NUM_SCREEN_7                                         887
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_7                                     0X377000

#define LN1_LCD_SECTOR_NUM_SCREEN_8                                         888
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_8                                     0X378000

#define LN1_LCD_SECTOR_NUM_SCREEN_9                                         889
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_9                                     0X379000

#define LN1_LCD_SECTOR_NUM_SCREEN_10                                        890
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_10                                    0X37A000


#define LN1_LCD_SECTOR_NUM_BIG_DIGITS                                      891    //big_digits 0->9 reserved 6 page
#define LN1_LCD_SECTOR_ADDRESS_BIG_DIGITS                                  0x37B000

#define LN1_LCD_SECTOR_NUM_SMALL_DIGITS                                    892    //small_digits 0->9 reserved 6 page
#define LN1_LCD_SECTOR_ADDRESS_SMALL_DIGITS                                0x37C000

#define LN1_LCD_SECTOR_NUM_AR_WORDS1                                       893    //words 0->26 reserved 5 page & 3 sectors
#define LN1_LCD_SECTOR_ADDRESS_AR_WORDS1                                   0x37D000
#define LN1_LCD_SECTOR_NUM_AR_WORDS2                                       894    
#define LN1_LCD_SECTOR_ADDRESS_AR_WORDS2                                   0x37E000
#define LN1_LCD_SECTOR_NUM_AR_WORDS3                                       895    
#define LN1_LCD_SECTOR_ADDRESS_AR_WORDS3                                   0x37F000
#define LN1_LCD_SECTOR_NUM_AR_WORDS4                                       896    
#define LN1_LCD_SECTOR_ADDRESS_AR_WORDS4                                   0x380000
#define LN1_LCD_SECTOR_NUM_AR_WORDS5                                       897    
#define LN1_LCD_SECTOR_ADDRESS_AR_WORDS5                                   0x381000

#define LN1_LCD_SECTOR_NUM_SYMBOLS1                                        898    //symbols 0->20 reserved 11 page
#define LN1_LCD_SECTOR_ADDRESS_SYMBOLS1                                    0x382000
#define LN1_LCD_SECTOR_NUM_SYMBOLS2                                        899    
#define LN1_LCD_SECTOR_ADDRESS_SYMBOLS2                                    0x383000
#define LN1_LCD_SECTOR_NUM_SYMBOLS3                                        900    
#define LN1_LCD_SECTOR_ADDRESS_SYMBOLS3                                    0x384000
#define LN1_LCD_SECTOR_NUM_SYMBOLS4                                        901    
#define LN1_LCD_SECTOR_ADDRESS_SYMBOLS4                                    0x385000

#define LN1_LCD_SECTOR_NUM_UNITS                                           902    //units 0->6 reserved 6 page
#define LN1_LCD_SECTOR_ADDRESS_UNITS                                       0x386000

#define LN1_LCD_SECTOR_NUM_ALPHA1                                          903    //A -> P
#define LN1_LCD_SECTOR_ADDRESS_ALPHA1                                      0x387000
#define LN1_LCD_SECTOR_NUM_ALPHA2                                          904    //Q -> Z reserved 7 page
#define LN1_LCD_SECTOR_ADDRESS_ALPHA2                                      0x388000

/************************************ LANGUAGE 2 *****************************************/


#define LN2_LCD_SECTOR_NUM_SCREEN_0                                         905
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_0                                     0X389000

#define LN2_LCD_SECTOR_NUM_SCREEN_1                                         906
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_1                                     0X38A000

#define LN2_LCD_SECTOR_NUM_SCREEN_2                                         907
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_2                                     0X38B000

#define LN2_LCD_SECTOR_NUM_SCREEN_3                                         908
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_3                                     0X38C000

#define LN2_LCD_SECTOR_NUM_SCREEN_4                                         909
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_4                                     0X38D000 

#define LN2_LCD_SECTOR_NUM_SCREEN_5                                         910
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_5                                     0X38E000

#define LN2_LCD_SECTOR_NUM_SCREEN_6                                         911
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_6                                     0X38F000

#define LN2_LCD_SECTOR_NUM_SCREEN_7                                         912
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_7                                     0X390000

#define LN2_LCD_SECTOR_NUM_SCREEN_8                                         913
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_8                                     0X391000

#define LN2_LCD_SECTOR_NUM_SCREEN_9                                         914
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_9                                     0X392000

#define LN2_LCD_SECTOR_NUM_SCREEN_10                                        915
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_10                                    0X393000



#define LN2_LCD_SECTOR_NUM_BIG_DIGITS                                      916    //big_digits 0->9 reserved 6 page
#define LN2_LCD_SECTOR_ADDRESS_BIG_DIGITS                                  0x394000


#define LN2_LCD_SECTOR_NUM_SMALL_DIGITS                                    917    //small_digits 0->9 reserved 6 page
#define LN2_LCD_SECTOR_ADDRESS_SMALL_DIGITS                                0x395000

#define LN2_LCD_SECTOR_NUM_AR_WORDS1                                       918    //words 0->26 reserved 5 page 
#define LN2_LCD_SECTOR_ADDRESS_AR_WORDS1                                   0x396000
#define LN2_LCD_SECTOR_NUM_AR_WORDS2                                       919    
#define LN2_LCD_SECTOR_ADDRESS_AR_WORDS2                                   0x397000
#define LN2_LCD_SECTOR_NUM_AR_RESERVED1                                    920    
#define LN2_LCD_SECTOR_ADDRESS_AR_RESERVED1                                0x398000
#define LN2_LCD_SECTOR_NUM_AR_RESERVED2                                    921    
#define LN2_LCD_SECTOR_ADDRESS_AR_RESERVED2                                0x399000
#define LN2_LCD_SECTOR_NUM_AR_RESERVED3                                    922    
#define LN2_LCD_SECTOR_ADDRESS_AR_RESERVED3                                0x39A000


#define LN2_LCD_SECTOR_NUM_SYMBOLS1                                        923    //symbols 0->20 reserved 11 page
#define LN2_LCD_SECTOR_ADDRESS_SYMBOLS1                                    0x39B000
#define LN2_LCD_SECTOR_NUM_SYMBOLS2                                        924    
#define LN2_LCD_SECTOR_ADDRESS_SYMBOLS2                                    0x39C000
#define LN2_LCD_SECTOR_NUM_SYMBOLS_RESERVED1                               925    
#define LN2_LCD_SECTOR_ADDRESS_SYMBOLS_RESERVED1                           0x39D000
#define LN2_LCD_SECTOR_NUM_SYMBOLS_RESERVED2                               926    
#define LN2_LCD_SECTOR_ADDRESS_SYMBOLS_RESERVED2                           0x39E000

#define LN2_LCD_SECTOR_NUM_UNITS                                           927    //units 0->6 reserved 6 page
#define LN2_LCD_SECTOR_ADDRESS_UNITS                                       0x39F000

#define LN2_LCD_SECTOR_NUM_ALPHA1                                          928    //A -> P
#define LN2_LCD_SECTOR_ADDRESS_ALPHA1                                      0x3A0000
#define LN2_LCD_SECTOR_NUM_ALPHA2                                          929    //Q -> Z reserved 7 page
#define LN2_LCD_SECTOR_ADDRESS_ALPHA2                                      0x3A1000


/************************************FRENCH LANGUAGE*****************************************/
#define LN3_LCD_SECTOR_NUM_SCREEN_0                                         930
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_0                                     0x3A2000

#define LN3_LCD_SECTOR_NUM_SCREEN_1                                         931
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_1                                     0x3A3000

#define LN3_LCD_SECTOR_NUM_SCREEN_2                                         932
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_2                                     0x3A4000

#define LN3_LCD_SECTOR_NUM_SCREEN_3                                         933
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_3                                     0x3A5000

#define LN3_LCD_SECTOR_NUM_SCREEN_4                                         934
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_4                                     0x3A6000 

#define LN3_LCD_SECTOR_NUM_SCREEN_5                                         935
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_5                                     0x3A7000

#define LN3_LCD_SECTOR_NUM_SCREEN_6                                         936
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_6                                     0X3A8000

#define LN3_LCD_SECTOR_NUM_SCREEN_7                                         937
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_7                                     0X3A9000

#define LN3_LCD_SECTOR_NUM_SCREEN_8                                         938
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_8                                     0X3AA000

#define LN3_LCD_SECTOR_NUM_SCREEN_9                                         939
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_9                                     0X3AB000

#define LN3_LCD_SECTOR_NUM_SCREEN_10                                        940
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_10                                    0X3AC000



#define LN3_LCD_SECTOR_NUM_BIG_DIGITS                                      941    //big_digits 0->9 reserved 6 page
#define LN3_LCD_SECTOR_ADDRESS_BIG_DIGITS                                  0x3AD000


#define LN3_LCD_SECTOR_NUM_SMALL_DIGITS                                    942    //small_digits 0->9 reserved 6 page
#define LN3_LCD_SECTOR_ADDRESS_SMALL_DIGITS                                0x3AE000

#define LN3_LCD_SECTOR_NUM_AR_WORDS1                                       943    //words 0->26 reserved 5 page 
#define LN3_LCD_SECTOR_ADDRESS_AR_WORDS1                                   0x3AF000
#define LN3_LCD_SECTOR_NUM_AR_WORDS2                                       944    
#define LN3_LCD_SECTOR_ADDRESS_AR_WORDS2                                   0x3B0000
#define LN3_LCD_SECTOR_NUM_AR_RESERVED1                                    945    
#define LN3_LCD_SECTOR_ADDRESS_AR_RESERVED1                                0x3B1000
#define LN3_LCD_SECTOR_NUM_AR_RESERVED2                                    946    
#define LN3_LCD_SECTOR_ADDRESS_AR_RESERVED2                                0x3B2000
#define LN3_LCD_SECTOR_NUM_AR_RESERVED3                                    947    
#define LN3_LCD_SECTOR_ADDRESS_AR_RESERVED3                                0x3B3000


#define LN3_LCD_SECTOR_NUM_SYMBOLS1                                        948    //symbols 0->20 reserved 11 page
#define LN3_LCD_SECTOR_ADDRESS_SYMBOLS1                                    0x3B4000
#define LN3_LCD_SECTOR_NUM_SYMBOLS2                                        949    
#define LN3_LCD_SECTOR_ADDRESS_SYMBOLS2                                    0x3B5000
#define LN3_LCD_SECTOR_NUM_SYMBOLS_RESERVED1                               950    
#define LN3_LCD_SECTOR_ADDRESS_SYMBOLS_RESERVED1                           0x3B6000
#define LN3_LCD_SECTOR_NUM_SYMBOLS_RESERVED2                               951    
#define LN3_LCD_SECTOR_ADDRESS_SYMBOLS_RESERVED2                           0x3B7000

#define LN3_LCD_SECTOR_NUM_UNITS                                           952   //units 0->6 reserved 6 page
#define LN3_LCD_SECTOR_ADDRESS_UNITS                                       0x3B8000

#define LN3_LCD_SECTOR_NUM_ALPHA1                                          953    //A -> P
#define LN3_LCD_SECTOR_ADDRESS_ALPHA1                                      0x3B9000
#define LN3_LCD_SECTOR_NUM_ALPHA2                                          954    //Q -> Z reserved 7 page
#define LN3_LCD_SECTOR_ADDRESS_ALPHA2                                      0x3BA000

/*some sectors reserved from 3BB000 ----> 3BF000*/
/****************************************BUFFER SECTOR*****************************************/
#define BUFFER_SECTOR_NUM                                                 960    
#define BUFFER_SECTOR_ADDRESS                                             0X3C0000


/****************************************END*****************************************/
#endif





/******************************** flash_app defined addressess ***********************************/
#define ID_SECTOR_ADD 	   			                  0//
#define FLAG_SECTOR_ADD 	   			          4
#define FLAG_APP_SECTOR_ADD 	   			          5
#define COUNTER_SECTOR_ADD 	   			          4
#define BACKUP_ID_SECTOR_ADD                                      5
// PageAdress = SectorADD*16 + pageIndex
/*--- Image Data Sectors ---*/
#define PRV_IMAGE_SECTOR_START_ADD		                  4
#define PRV_IMAGE_SECTOR_END_ADD		                  4

#define CURRENT_IMAGE_SECTOR_START_ADD		                  3
#define CURRENT_IMAGE_SECTOR_END_ADD		                  3

#define NEW_IMAGE_SECTOR_START_ADD		                  3
#define NEW_IMAGE_SECTOR_END_ADD		                  4

#define BOOT_IMAGE_SECTOR_START_ADD     	                  5
#define BOOT_IMAGE_SECTOR_END_ADD		                  5

#define NEW_BOOT_IMAGE_SECTOR_START_ADD		                  5
#define NEW_BOOT_IMAGE_SECTOR_END_ADD    	                  5


/*--- Id of every record or image saved in the first of each page in this sector and make the remaining reserved  ---*/
#define MONEY_ID_PAGE_INDEX		   	                 (uint8_t)0
#define BILLING_ID_PAGE_INDEX		                         (uint8_t)0
#define CONFIG_METER_ID_PAGE_INDEX		                 (uint8_t)0
#define EVENT_RECORD_ID_PAGE_INDEX	                         (uint8_t)0
#define LOAD_PROFILE_ID_PAGE_INDEX	       	                 (uint8_t)0
#define CONFIG_MD_PAGE_INDEX                                     (uint8_t)0
#define DLMS_RECORDS_PAGE_INDEX                                  (uint8_t)0
/*--- Images Information ID ---*/
#define PRV_IMAGE_INFO_PAGE_INDEX        	                 (uint8_t)0
#define CURRENT_IMAGE_INFO_PAGE_INDEX     	                 (uint8_t)0
#define FUTURE_IMAGE_INFO_PAGE_INDEX	   	   	         (uint8_t)0
#define CURRENT_BOOTLOADER_INFO_PAGE_INDEX                       (uint8_t)0
#define NEW_BOOTLOADER_INFO_PAGE_INDEX	    	                 (uint8_t)0
#define FLAG_IMAGE_INDICATION_PAGE_INDEX  	                 (uint8_t)0

/*--- Records Data Sectors ---*/
#define ROFILE_SECTOR_NUMBERS			                  0
#define PROFILE_SECTOR_START_ADD                                  0
#define PROFILE_SECTOR_END_ADD                                    0
                                                                  
#define EVENT_RECORD_SECTOR_NUMBERS	                          0
#define EVENT_RECORD_SECTOR_START_ADD                             0
#define EVENT_RECORD_SECTOR_END_ADD                               0

#define CONFIG_RECORD_SECTOR_NUMBERS	 	                  5
#define CONFIG_RECORD_SECTOR_START_ADD                            0
#define CONFIG_RECORD_SECTOR_END_ADD                              0
                                                                  
#define BILLING_RECORD_SECTOR_NUMBERS		                  0
#define BILLING_RECORD_SECTOR_START_ADD                           0
#define BILLING_RECORD_SECTOR_END_ADD                             0
                                                                  
#define MONEY_RECORD_SECTOR_NUMBERS 		                  0
#define MONEY_RECORD_SECTOR_START_ADD                             0
#define MONEY_RECORD_SECTOR_END_ADD                               0
                                                                  
#define MD_RECORD_SECTOR_NUMBERS 		                  0
#define MD_RECORD_SECTOR_START_ADD                                0
#define MD_RECORD_SECTOR_END_ADD                                  0
                                                                  
#define DLMS_RECORD_SECTOR_1_START_ADD                            0
#define DLMS_RECORD_SECTOR_1_END_ADD                              0
                                                                  
#define DLMS_RECORD_SECTOR_2_START_ADD                            0
#define DLMS_RECORD_SECTOR_2_END_ADD                              0
                                                                  
#define DLMS_RECORD_SECTOR_3_START_ADD                            0
#define DLMS_RECORD_SECTOR_3_END_ADD                              0
                                                                  
#define DLMS_RECORD_SECTOR_4_START_ADD                            0
#define DLMS_RECORD_SECTOR_4_END_ADD                              0
                                                                  
#define DLMS_RECORD_SECTOR_5_START_ADD                            0
#define DLMS_RECORD_SECTOR_5_END_ADD                              0
                                                                  
#define DLMS_RECORD_SECTOR_6_START_ADD                            0
#define DLMS_RECORD_SECTOR_6_END_ADD                              0
                                                                  
#define DLMS_RECORD_SECTOR_7_START_ADD                            0
#define DLMS_RECORD_SECTOR_7_END_ADD                              0
                                                                  
#define DLMS_RECORD_SECTOR_8_START_ADD                            0
#define DLMS_RECORD_SECTOR_8_END_ADD                              0
                                                                  
#define DLMS_RECORD_SECTOR_9_START_ADD                            0
#define DLMS_RECORD_SECTOR_9_END_ADD                              0
                                                                  
#define DLMS_RECORD_SECTOR_10_START_ADD                           0
#define DLMS_RECORD_SECTOR_10_END_ADD                             0
                                                                  
#define DLMS_RECORD_SECTOR_11_START_ADD                           0
#define DLMS_RECORD_SECTOR_11_END_ADD                             0//sector

#define BACK_UP_SECTOR                                            0//sector 

////////////////////////////////Group_Metering////////////////////////////////////

#define GM_Flash_ReadOutData_StartAddress                                0 
#define GM_Flash_ReadOutData_LastAddress                                 0 
                                                           
                                                           
#define GM_Flash_TampersAndEvents_1_Sector_StartAddress                  0
#define GM_Flash_TampersAndEvents_2_Sector_StartAddress                  0
//                                                         
//                                                         
#define GM_Flash_TampersAndEventsBackUp_1_Sector_StartAddress            0
#define GM_Flash_TampersAndEventsBackUp_2_Sector_StartAddress            0
                                                                    
#define GM_Flash_Orders_StartAddress                                     0
#define GM_Flash_OrdersBackUp_StartAddress                               0
        
#define GM_Flash_CMD_Respons_StartAddress                                0
#define GM_Flash_CMD_ResponsBackUp_StartAddress                          0
        
        
#define GM_Flash_SerialNumber_StartAddress                                0
#define GM_Flash_SerialNumberBackUp_StartAddress                          0

//////////////////////////////////////////////////////////////////////////////////
   /**************************************** Flash app generic address *********************************************************/
/*----------- tampers store --------*/
#define TAMPERS_COUNTER_ID_BYTE_INDEX                           0
#define TAMPERS_COUNTER_ID_PAGE                                 0

#define TAMPERS_COUNTER_REDUNDANT_ID_BYTE_INDEX                 1
#define TAMPERS_COUNTER_REDUNDANT_ID_PAGE                       2

#define LAST_TAMPER_ID_BYTE_INDEX                               1
#define LAST_TAMPER_ID_PAGE                                     4
   
#define EVENTLOG_STORE_PERMISSION_BYTE_INDEX                    0
#define EVENTLOG_STORE_PERMISSION_PAGE                          5

#define EVENT_LPM_0_ID_BYTE_INDEX                               0
#define EVENT_LPM_1_ID_BYTE_INDEX                               1
#define EVENT_LPM_2_ID_BYTE_INDEX                               3
#define EVENT_LPM_ID_PAGE                                       7 

#define INSTANTENOUS_ERROR_STATUS_ID_BYTE_INDEX                 7         
#define ERROR_STATUS_ID_BYTE_INDEX                              7
#define ERROR_STATUS_ID_PAGE                                    7

#define LIMITER_LOADLIMIT_CFG_ID_BYTE_INDEX                     8
#define LIMITER_LOADLIMIT_CFG_ID_PAGE                           7

#define LIMITER_LOADLIMIT_NUM_OF_PLANS_ID_BYTE                  9
#define LIMITER_LOADLIMIT_NUM_OF_PLANS_ID_PAGE                  7


#define LIMITER_THRESH_CUR1_CFG_PARAMS_ID_INDEX                 9
#define LIMITER_THRESH_CUR1_CFG_PARAMS_ID_PAGE                  7


#define LIMITER_MON_VAL_CUR1_CFG_PARAMS_ID_BYTE                 1
#define LIMITER_MON_VAL_CUR1_CFG_PARAMS_ID_PAGE                 7

#define EVENTLOGSTOREPERMISSION_POWER_COC_ID_BYTE_INDEX         0
#define EVENTLOGSTOREPERMISSION_POWER_COC_ID_PAGE               8


#define LIMITER_EMERGENCY_CUR1_CFG_PARAMS_BYTE                  0
#define LIMITER_EMERGENCY_CUR1_CFG_PARAMS_PAGE                  1

#define LIMITER_LOADLIMIT_PLAN1_INDEX                           0
#define LIMITER_LOADLIMIT_PLAN1_PAGE                            1

#define POWER_FAILURE_DURATION_OFFSET_ID                        1
#define POWER_FAILURE_DURATION_OFFSET_ID_PAGE                   1         
                                                                
#define SHORT_POWER_FAILURE_DURATION_OFFSET_ID                  1
#define SHORT_POWER_FAILURE_DURATION_OFFSET_ID_PAGE             1
                                                                
#define LONG_POWER_FAILURE_DURATION_OFFSET_ID                   1
#define LONG_POWER_FAILURE_DURATION_OFFSET_ID_PAGE              1
                                                                
#define LONG_POWER_FAILURE_THRESHOLD_OFFSET_ID            1     
#define LONG_POWER_FAILURE_THRESHOLD_OFFSET_ID_PAGE             1
                                                                
#define SAG_STATES_DATA_OFFSET_ID                         1     5
#define SAG_STATES_DATA_OFFSET_ID_PAGE                          1
                                                                
#define SWELL_STATES_DATA_OFFSET_ID                             7
#define SWELL_STATES_DATA_OFFSET_ID_PAGE                        1

#define SHORT_POWER_FAILURE_COUNTER_ID                         2 
#define SHORT_POWER_FAILURE_COUNTER_ID_PAGE                     1
                                                               
#define LONG_POWER_FAILURE_COUNTER_ID                          1
#define LONG_POWER_FAILURE_COUNTER_ID_PAGE                      1
                                                               
#define POWER_CUT_START_OFFSET2_ID                              2
#define POWER_CUT_START_OFFSET2_ID_PAGE                         1
                                                               
#define POWER_CUT_START_OFFSET2_ID_INDEX                        2
#define POWER_CUT_START_OFFSET2_ID_PAGE                         1
                                                               
#define LIMITER_LOADLIMIT_TYPE_ID_BYTE                          0
#define LIMITER_LOADLIMIT_TYPE_ID_PAGE                          1
                                                               
#define TAMPERS_SECTOR                                          5

/*---- load profile store -------*/
//Profile_ID_Cfg_Idx
#define LP_CONFIG1_ID                                           0
#define LP_CONFIG2_ID                                           0
#define LP_CONFIG4_ID                                           0
#define LP_CONFIG5_ID                                           0
#define LP_CONFIG6_ID                                           0

#define LP_1_PAGE                                               0//page

#define Battery_TamperEvent_Timestamp_ID                        0//todo it's size in future if you need to use all tampers ,now i use one only.
#define Battery_TamperEvent_Timestamp_ID_PAGE                   0

#define Battery_TamperEvent_Duration_ID                         0
#define Battery_TamperEvent_Duration_ID_PAGE                    0

#define Battery_TamperEvent_Cumulative_ID                       0
#define Battery_TamperEvent_Cumulative_ID_PAGE                  0

/////////////////
//Capture_Object_list_Idx
#define LP_OBJECTLIST_PAGE                                      1//page

#define LP_OBJECTLIST_IDX1_ID                                   0
#define LP_OBJECTLIST_IDX2_ID                                   0
#define LP_OBJECTLIST_IDX4_ID                                   0
#define LP_OBJECTLIST_IDX5_ID                                   0
#define LP_OBJECTLIST_IDX6_ID                                   0
//Capture_Object_list                                           0
#define LP_OBJECTLIST_CFG1_ID                                   0
#define LP_OBJECTLIST_CFG2_ID                                   0
#define LP_OBJECTLIST_CFG3_ID                                   0
#define LP_OBJECTLIST_CFG4_ID                                   0
#define LP_OBJECTLIST_CFG5_ID                                   0
#define LP_OBJECTLIST_CFG6_ID                                   0
                                                                
////////////                                                    0
#define LAST_BILLING_MONTH_ID_BYTE                              0
#define LAST_BILLING_MONTH_ID_PAGE                              0
                                                                
#define Time_Change_Flag_ID_BYTE                                0
#define Time_Change_Flag_ID_PAGE                                0
                                                                
#define AMR_PROFILE_STATUS_CODE_ID                              0
#define AMR_PROFILE_STATUS_CODE_ID_PAGE                         0
////////////

#define LP_SECTOR                                               502//sector


///////////////////////////
#define EVENT_NOT_SECTOR                                        LP_SECTOR

#define LASTHANDLEEVENT_ID                                      0//64bytes
#define LASTHANDLEEVENT_ID_PAGE                                 0

//note : form 64 till 255 in page 3 is empty
#define EVENTLOGSENDEPERMISSION_POWER_COC_ID                    0//512bytes //not jumping to anther page (checked ok ✔)
#define EVENTLOGSENDEPERMISSION_POWER_COC_ID_PAGE               0 //2pages (5,6)

/////////////
#define CALENDER_SECTOR                                         LP_SECTOR
   
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_ACTIVE_PASSIVE_TIME_ID  0//70bytes
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_ACTIVE_PASSIVE_TIME_ID_PAGE 7//page

#define PassiveToActiveCoping_Flag_ID                           0
#define PassiveToActiveCoping_Flag_ID_PAGE                     0

#define ACTIVITY_CALENDER_ACTIVE_IMPORT_CALENDER_NAME_ACTIVE_ID 0
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_CALENDER_NAME_ACTIVE_ID_PAGE 0
   
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_CALENDER_NAME_PASSIVE_ID 0
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_CALENDER_NAME_PASSIVE_ID_PAGE 0

//note : use this :: from 150 to 192 in page 7 is empty 

#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_SEASON_ACTIVE_ID         0//520bytes, every one : 104bytes //not jumping to anther page (checked ok ✔)
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_SEASON_ACTIVE_ID_PAGE    0//page


#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_SEASON_PASSIVE_ID        0          
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_SEASON_PASSIVE_ID_PAGE   0//page

#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_WEEK_ACTIVE_ID           0//
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_WEEK_ACTIVE_ID_PAGE      0//page

#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_WEEK_PASSIVE_ID          0//
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_WEEK_PASSIVE_ID_PAGE     0//page

#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_DAY_TABLE_ACTIVE_ID      0//  
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_DAY_TABLE_ACTIVE_ID_PAGE 0//  page

#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_DAY_TABLE_PASSIVE_ID     0// 
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_DAY_TABLE_PASSIVE_ID_PAGE 0// page

/////////////
#define CALENDER_2_SECTOR                                         0//sector/****-----** last sector till now ****------*******/

#define ACTIVITY_CALENDER_ACTIVE_IMPORT_ACTIVE_INDEX_ID           0//
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_ACTIVE_INDEX_ID_PAGE      0//page

#define ACTIVITY_CALENDER_ACTIVE_IMPORT_PASSIVE_INDEX_ID         0//
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_PASSIVE_INDEX_ID_PAGE    0//page

#define SPECIAL_DAYS_RECORDS_ID                                  0
#define SPECIAL_DAYS_RECORDS_ID_PAGE                             0//page

#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ASSIGMENT_NUM_ID 0//5bytes
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ASSIGMENT_NUM_ID_PAGE 0
                                                                         
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTE_ACTIVATION_NUM_ID      0
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTE_ACTIVATION_NUM_ID_PAGE 0
                                                                         
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ACTIVE_ID             0
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ACTIVE_ID_PAGE        0
                                                                         
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ASSIGMENT_ID          0
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ASSIGMENT_ID_PAGE     0
                                                                         
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTE_ACTIVATION_ID          0
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTE_ACTIVATION_ID_PAGE     0
                                                                         
#define SPECIAL_DAYS_TABLE_ID                                            0
#define SPECIAL_DAYS_TABLE_ID_PAGE                                       0
                                                                         
//////                                                                   
#define TRF_REG_SECTOR                                                   0
                                                                         
#define ACTIVE_TARRIF_BILLING_PERIOD_ID                                  0
#define ACTIVE_TARRIF_BILLING_PERIOD_ID_PAGE                             0

#define REACTIVE_TARRIF_BILLING_PERIOD_ID                               0
#define REACTIVE_TARRIF_BILLING_PERIOD_ID_PAGE                          0
                                                                        
#define ACTIVE_GEN_TARRIF_BILLING_PERIOD_ID                             0
#define ACTIVE_GEN_TARRIF_BILLING_PERIOD_ID_PAGE                        0
                                                                        
#define REACTIVE_GEN_TARRIF_BILLING_PERIOD_ID                           0
#define REACTIVE_GEN_TARRIF_BILLING_PERIOD_ID_PAGE                      0
                                                                        
#define PRE_Register_Period1_ID                                         0
#define PRE_Register_Period1_ID_PAGE                                    0
                                                                        
#define ACTIVE_TARRIF_PERIOD1_ID                                        0
#define ACTIVE_TARRIF_PERIOD1_ID_PAGE                                   0
                                                                        
#define LAST_PERIODIC_MIN_OFFSET_ID                                     0
#define LAST_PERIODIC_MIN_OFFSET_ID_PAGE                                0
                                                                        
#define LAST_PERIODIC_MAX_OFFSET_ID                                     0
#define LAST_PERIODIC_MAX_OFFSET_ID_PAGE                                0
                                                                        
#define LAST_PERIODIC_AVG_OFFSET_ID                                     0
#define LAST_PERIODIC_AVG_OFFSET_ID_PAGE                                0
#endif	// end of file