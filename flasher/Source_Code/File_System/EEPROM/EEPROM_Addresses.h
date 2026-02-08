// Created with Excel Table By Mohamed Hamed			
// Date:  11/04/2015        Time:  02:14:49			
#ifndef EEPROM_ADDRESS_H			
#define EEPROM_ADDRESS_H			

#ifndef EEPROM_PAGE_256
#define EEPROM_SIZE_IN_BYTES 16384uL
#else
#define EEPROM_SIZE_IN_BYTES 131072uL
#endif
/*!
 * @addtogroup EEPROM_CMD
 * @{
 */
/*! EEPROM page size in Bytes.*/
#define EEPROM_PAGE_SIZE 256L
#define DATA_EEPROM       0
#define Data_eeprom_address  0
#define BOALOADER_EEPROM  1
/*! EEPROM page 0 address.*/
#define EEPROM_PAGE_0 (uint32_t)(0*EEPROM_PAGE_SIZE)
/*! EEPROM page 1 address.*/
#define EEPROM_PAGE_1 (uint32_t)(1*EEPROM_PAGE_SIZE)
/*! EEPROM page 2 address.*/
#define EEPROM_PAGE_2 (uint32_t)(2*EEPROM_PAGE_SIZE)
    /*! EEPROM page 4 address.*/
#define EEPROM_PAGE_4 (uint32_t)(4*EEPROM_PAGE_SIZE)
   /*! EEPROM page 5 address.*/
#define EEPROM_PAGE_5 (uint32_t)(5*EEPROM_PAGE_SIZE)
/*! EEPROM page 6 address.*/
#define EEPROM_PAGE_6 (uint32_t)(6*EEPROM_PAGE_SIZE)
/*! EEPROM page 7 address.*/
#define EEPROM_PAGE_7 (uint32_t)(7*EEPROM_PAGE_SIZE)
/*! EEPROM page 8 address.*/
#define EEPROM_PAGE_8 (uint32_t)(8*EEPROM_PAGE_SIZE)
/*! EEPROM page 11 address.*/
#define EEPROM_PAGE_11 (uint32_t)(11*EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_12 (uint32_t)(12*EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_13 (uint32_t)(13*EEPROM_PAGE_SIZE)
/*! EEPROM page 14 address.*/
#define EEPROM_PAGE_14 (uint32_t)(14*EEPROM_PAGE_SIZE)
/*! EEPROM page 15 address.*/
#define EEPROM_PAGE_15 (uint32_t)(15*EEPROM_PAGE_SIZE)
/*! EEPROM page 17 address.*/
#define EEPROM_PAGE_17 (uint32_t)(17*EEPROM_PAGE_SIZE)
/*! EEPROM page 18 address.*/
#define EEPROM_PAGE_18 (uint32_t)(18*EEPROM_PAGE_SIZE)
/*! EEPROM page 19 address.*/
#define EEPROM_PAGE_19 (uint32_t)(19*EEPROM_PAGE_SIZE)
/*! EEPROM page 20 address.*/
#define EEPROM_PAGE_20 (uint32_t)(20*EEPROM_PAGE_SIZE)
/*! EEPROM page 21 address.*/
#define EEPROM_PAGE_21 (uint32_t)(21*EEPROM_PAGE_SIZE)
/*! EEPROM page 22 address.*/
#define EEPROM_PAGE_22 (uint32_t)(22*EEPROM_PAGE_SIZE)
#ifdef FM24C128D_2_Wire_Serial_EEPROM
/*! EEPROM page 23 address.*/
#define EEPROM_PAGE_23 (uint32_t)(23*EEPROM_PAGE_SIZE)
/*! EEPROM page 24 address.*/
#define EEPROM_PAGE_24 (uint32_t)(24*EEPROM_PAGE_SIZE)
/*! EEPROM page 25 address.*/
#define EEPROM_PAGE_25 (uint32_t)(25*EEPROM_PAGE_SIZE)
/*! EEPROM page 26 address.*/
#define EEPROM_PAGE_26 (uint32_t)(26*EEPROM_PAGE_SIZE)
/*! EEPROM page 27 address.*/
#define EEPROM_PAGE_27 (uint32_t)(27*EEPROM_PAGE_SIZE)
/*! EEPROM page 28 address.*/
#define EEPROM_PAGE_28 (uint32_t)(28*EEPROM_PAGE_SIZE)
/*! EEPROM page 29 address.*/
#define EEPROM_PAGE_29 (uint32_t)(29*EEPROM_PAGE_SIZE)
/*! EEPROM page 30 address.*/
#define EEPROM_PAGE_30 (uint32_t)(30*EEPROM_PAGE_SIZE)
/*! EEPROM page 32 address.*/
#define EEPROM_PAGE_32 (uint32_t)(32*EEPROM_PAGE_SIZE)
/*! EEPROM page 33 address.*/
#define EEPROM_PAGE_33 (uint32_t)(33*EEPROM_PAGE_SIZE)
/*! EEPROM page 34 address.*/
#define EEPROM_PAGE_34 (uint32_t)(34*EEPROM_PAGE_SIZE)
/*! EEPROM page 35 address.*/
#define EEPROM_PAGE_35 (uint32_t)(35*EEPROM_PAGE_SIZE)
/*! EEPROM page 36 address.*/
#define EEPROM_PAGE_36 (uint32_t)(36*EEPROM_PAGE_SIZE)
/*! EEPROM page 37 address.*/
#define EEPROM_PAGE_37 (uint32_t)(37*EEPROM_PAGE_SIZE)
/*! EEPROM page 38 address.*/
#define EEPROM_PAGE_38 (uint32_t)(38*EEPROM_PAGE_SIZE)
/*! EEPROM page 39 address.*/
#define EEPROM_PAGE_39 (uint32_t)(39*EEPROM_PAGE_SIZE)
/*! EEPROM page 40 address.*/
#define EEPROM_PAGE_40 (uint32_t)(40*EEPROM_PAGE_SIZE)
/*! EEPROM page 41 address.*/
#define EEPROM_PAGE_41 (uint32_t)(41*EEPROM_PAGE_SIZE)
/*! EEPROM page 42 address.*/
#define EEPROM_PAGE_42 (uint32_t)(42*EEPROM_PAGE_SIZE)
/*! EEPROM page 43 address.*/
#define EEPROM_PAGE_43 (uint32_t)(43*EEPROM_PAGE_SIZE)
/*! EEPROM page 44 address.*/
#define EEPROM_PAGE_44 (uint32_t)(44*EEPROM_PAGE_SIZE)
/*! EEPROM page 45 address.*/
#define EEPROM_PAGE_45 (uint32_t)(45*EEPROM_PAGE_SIZE)
/*! EEPROM page 46 address.*/
#define EEPROM_PAGE_46 (uint32_t)(46*EEPROM_PAGE_SIZE)
/*! EEPROM page 47 address.*/
#define EEPROM_PAGE_47 (uint32_t)(47*EEPROM_PAGE_SIZE)
/*! EEPROM page 48 address.*/
#define EEPROM_PAGE_48 (uint32_t)(48*EEPROM_PAGE_SIZE)
/*! EEPROM page 49 address.*/
#define EEPROM_PAGE_49 (uint32_t)(49*EEPROM_PAGE_SIZE)
/*! EEPROM page 50 address.*/
#define EEPROM_PAGE_50 (uint32_t)(50*EEPROM_PAGE_SIZE)
/*! EEPROM page 51 address.*/
#define EEPROM_PAGE_51 (uint32_t)(51*EEPROM_PAGE_SIZE)
/*! EEPROM page 52 address.*/
#define EEPROM_PAGE_52 (uint32_t)(52*EEPROM_PAGE_SIZE)
/*! EEPROM page 53 address.*/
#define EEPROM_PAGE_53 (uint32_t)(53*EEPROM_PAGE_SIZE)
/*! EEPROM page 54 address.*/
#define EEPROM_PAGE_54 (uint32_t)(54*EEPROM_PAGE_SIZE)
/*! EEPROM page 55 address.*/
#define EEPROM_PAGE_55 (uint32_t)(55*EEPROM_PAGE_SIZE)
/*! EEPROM page 56 address.*/
#define EEPROM_PAGE_56 (uint32_t)(56*EEPROM_PAGE_SIZE)
/*! EEPROM page 57 address.*/
#define EEPROM_PAGE_57 (uint32_t)(57*EEPROM_PAGE_SIZE)
/*! EEPROM page 58 address.*/
#define EEPROM_PAGE_58 (uint32_t)(58*EEPROM_PAGE_SIZE)
/*! EEPROM page 59 address.*/
#define EEPROM_PAGE_59 (uint32_t)(59*EEPROM_PAGE_SIZE)
/*! EEPROM page 60 address.*/
#define EEPROM_PAGE_60 (uint32_t)(60*EEPROM_PAGE_SIZE)
/*! EEPROM page 61 address.*/
#define EEPROM_PAGE_61 (uint32_t)(61*EEPROM_PAGE_SIZE)
/*! EEPROM page 62 address.*/
#define EEPROM_PAGE_62 (uint32_t)(62*EEPROM_PAGE_SIZE)
/*! EEPROM page 63 address.*/
#define EEPROM_PAGE_63 (uint32_t)(63*EEPROM_PAGE_SIZE)

// these addresses are for DLMS ,there is No place in EEPROM
#define    EEPROM_ADDRESS_BILLING_HISTORY                              
#define    EEPROM_ADDRESS_CLOCK_TIME_ZONE                              
#define    EEPROM_ADDRESS_CLOCK_DST_BEGIN                              
#define    EEPROM_ADDRESS_CLOCK_DST_END                                
#define    EEPROM_ADDRESS_CLOCK_DST_DEVIATION                          
#define    EEPROM_ADDRESS_CLOCK_DST_ENABLE                             
#define    EEPROM_ADDRESS_CLOCK_INITIALIZATION_FLAG                    

#define    EEPROM_ADDRESS_DISCONNECTOR_INITIALIZED                     
#define    EEPROM_ADDRESS_DISCONNECTOR_OUTPUT_STATE                    
#define    EEPROM_ADDRESS_DISCONNECTOR_CONTROL_STATE                   
#define    EEPROM_ADDRESS_DISCONNECTOR_CONTROL_MODE                    
#define    EEPROM_ADDRESS_LOW_POWER_MODE_TIME                          

#define FILE_SYS_GET_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS       

#define  FILE_SYS_GET_BACKUP_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS       
#define  EEPROM_RTC_INIT_FLAG_ADD                
#define RTC_SAVE_TIME_ADDR              
#define RTC_SAVE_DATE_ADDR              
#define  INT_ELAPSED_SAVE_TIME_ADDR     
   
#define  EEPROM_TEST_PAGE               1
#define   IEC_Buffer_PAGE               
#define FILE_SYS_GET_METERING_SYS_VAR_TEST_MODE_ADRS    (uint32_t)(48*EEPROM_PAGE_SIZE)
#define FILE_SYS_ALARMS_FILE_ADDRESS     (uint32_t)(49*EEPROM_PAGE_SIZE)
/*! @}*/
#define   FILE_SYS_ERR_LOAD_FLAG                0
#define   CTRL_ERR_LOAD_FLAG                    0
#define   DISPLAY_ERR_LOAD_FLAG                 0
#define   MTR_ERR_LOAD_FLAG                     0
#define   TRF_PYM_CFG_ERR_LOAD_FLAG             0
#define   COMM_ERR_LOAD_FLAG                    0
#define   CTRL_OPTIONAL_ERR_LOAD_FLAG           0
#define   CTRL_SECOND_OPTIONAL_ERR_LOAD_FLAG    0
#define   CTRL_NEW_SYS_ERR_LOAD_FLAG            0
#define   MTR_CFG_ERR_LOAD_FLAG                 0
#define   CTRL_VAR_ERR_LOAD_FLAG                0
#define   TRF_PYM_VAR_ERR_LOAD_FLAG             0
#define   ADE_ERR_CAL_LOAD_FLAG                 0
#define   STS_CFG_ERR_LOAD_FLAG                 0
#define   CTRL_LAST_TAMPERS_LOAD_FLAG           0
   

#define FILE_SYS_CHECK_EEPROM_FAIL_ADRS    EEPROM_PAGE_50
#ifdef GPRS_SEND_NOTIFICATION_FEATURE
//3 bytes (struct gprs_Notification_st)
#define FILE_SYS_UNSENT_GPRS_NOTIFICATION_ADRS         FILE_SYS_GET_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS
#endif
#else
/*! EEPROM page 30 address.*/
#define EEPROM_PAGE_30 (uint32_t)(30*EEPROM_PAGE_SIZE)
/*! EEPROM page 32 address.*/
#define EEPROM_PAGE_32 (uint32_t)(32*EEPROM_PAGE_SIZE)
/*! EEPROM page 52 address.*/
#define EEPROM_PAGE_61 (uint32_t)(61*EEPROM_PAGE_SIZE)
/*! EEPROM page 62 address.*/
#define EEPROM_PAGE_62 (uint32_t)(62*EEPROM_PAGE_SIZE)
/*! EEPROM page 55 address.*/
#define EEPROM_PAGE_64 (uint32_t)(64*EEPROM_PAGE_SIZE)
/*! EEPROM page 56 address.*/
#define EEPROM_PAGE_66 (uint32_t)(66*EEPROM_PAGE_SIZE)//to 409 Load profile
/*! EEPROM page 58 address.*/
#define EEPROM_PAGE_58 (uint32_t)(58*EEPROM_PAGE_SIZE)
/*! EEPROM page 60 address.*/
#define EEPROM_PAGE_60 (uint32_t)(60*EEPROM_PAGE_SIZE)
/*! EEPROM page 23 address.*/
#define EEPROM_PAGE_23 (uint32_t)(23*EEPROM_PAGE_SIZE)
/*! EEPROM page 24 address.*/
#define EEPROM_PAGE_24 (uint32_t)(24*EEPROM_PAGE_SIZE)
/*! EEPROM page 25 address.*/
#define EEPROM_PAGE_25 (uint32_t)(25*EEPROM_PAGE_SIZE)
/*! EEPROM page 26 address.*/
#define EEPROM_PAGE_26 (uint32_t)(26*EEPROM_PAGE_SIZE)
/*! EEPROM page 29 address.*/
#define EEPROM_PAGE_29 (uint32_t)(29*EEPROM_PAGE_SIZE)


#define    EEPROM_ADDRESS_InitSecCfg                                   (uint32_t)0
#define    EEPROM_ADDRESS_BeginSecCfg                                  (uint32_t)0
#define    EEPROM_ADDRESS_InitLocalPort                                (uint32_t)0
#define    EEPROM_ADDRESS_BeginLocalPort                               (uint32_t)0
#define    EEPROM_ADDRESS_InitTcpUdp                                   (uint32_t)0
#define    EEPROM_ADDRESS_BeginTcpUdp                                  (uint32_t)0
#define    EEPROM_ADDRESS_CLOCK_TIME_ZONE                              (uint32_t)0
#define    EEPROM_ADDRESS_CLOCK_DST_BEGIN                              (uint32_t)0
#define    EEPROM_ADDRESS_CLOCK_DST_END                                (uint32_t)0
#define    EEPROM_ADDRESS_CLOCK_DST_DEVIATION                          (uint32_t)0
#define    EEPROM_ADDRESS_CLOCK_DST_ENABLE                             (uint32_t)0
#define    EEPROM_ADDRESS_CLOCK_INITIALIZATION_FLAG                    (uint32_t)0

#define    EEPROM_ADDRESS_DISCONNECTOR_INITIALIZED                     (uint32_t)(144*EEPROM_PAGE_SIZE)
#define    EEPROM_ADDRESS_DISCONNECTOR_OUTPUT_STATE                    (uint32_t)(145*EEPROM_PAGE_SIZE)
#define    EEPROM_ADDRESS_DISCONNECTOR_CONTROL_STATE                   (uint32_t)(146*EEPROM_PAGE_SIZE)
#define    EEPROM_ADDRESS_DISCONNECTOR_CONTROL_MODE                    (uint32_t)(147*EEPROM_PAGE_SIZE)

/*! EEPROM page 508 address.*/
#define EEPROM_PAGE_508 (uint32_t)((uint16_t)508*(uint16_t)EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_507 (uint32_t)((uint16_t)507*(uint16_t)EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_509 (uint32_t)((uint16_t)509*(uint16_t)EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_510 (uint32_t)((uint16_t)510*(uint16_t)EEPROM_PAGE_SIZE)
/*! EEPROM page 511 address.*/
#define EEPROM_PAGE_511 (uint32_t)( (uint16_t)511* (uint16_t)EEPROM_PAGE_SIZE)

#define EEPROM_PAGE_396 (uint32_t)((uint16_t)396* (uint16_t)EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_397 (uint32_t)((uint16_t)397* (uint16_t)EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_400 (uint32_t)((uint16_t)400* (uint16_t)EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_401 (uint32_t)((uint16_t)401* (uint16_t)EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_402 (uint32_t)((uint16_t)402* (uint16_t)EEPROM_PAGE_SIZE)
/*! EEPROM page 199 address.*/
#define EEPROM_PAGE_410 (uint32_t)(410*EEPROM_PAGE_SIZE)
/*! EEPROM page 200 address.*/
#define EEPROM_PAGE_411 (uint32_t)(411*EEPROM_PAGE_SIZE)
/*! EEPROM page 201 address.*/
#define EEPROM_PAGE_412 (uint32_t)(412*EEPROM_PAGE_SIZE)
/*! EEPROM page 202 address.*/
#define EEPROM_PAGE_413 (uint32_t)(413*EEPROM_PAGE_SIZE)
/*! EEPROM page 203 address.*/
#define EEPROM_PAGE_414 (uint32_t)(414*EEPROM_PAGE_SIZE)
/*! EEPROM page 204 address.*/
#define EEPROM_PAGE_415 (uint32_t)(415*EEPROM_PAGE_SIZE)
/*! EEPROM page 205 address.*/
#define EEPROM_PAGE_416 (uint32_t)(416*EEPROM_PAGE_SIZE)
/*! EEPROM page 206 address.*/
#define EEPROM_PAGE_417 (uint32_t)(417*EEPROM_PAGE_SIZE)
/*! EEPROM page 206 address.*/
#define EEPROM_PAGE_418 (uint32_t)(418*EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_425 (uint32_t)(425*EEPROM_PAGE_SIZE)//new page 

#define FILE_SYS_GET_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS        EEPROM_PAGE_12

#define  FILE_SYS_GET_BACKUP_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS        EEPROM_PAGE_13
#define  EEPROM_RTC_INIT_FLAG_ADD                  EEPROM_PAGE_425
#define RTC_SAVE_TIME_ADDR                        (EEPROM_RTC_INIT_FLAG_ADD+2)
#define RTC_SAVE_DATE_ADDR                        (RTC_SAVE_TIME_ADDR+4)
#define  INT_ELAPSED_SAVE_TIME_ADDR               (RTC_SAVE_DATE_ADDR+4)
   
#define  EEPROM_TEST_PAGE               (uint32_t)(426*EEPROM_PAGE_SIZE)
#define   IEC_Buffer_PAGE               (uint32_t)(427*EEPROM_PAGE_SIZE)
#define FILE_SYS_GET_METERING_SYS_VAR_TEST_MODE_ADRS    (uint32_t)(427*EEPROM_PAGE_SIZE)
#define FILE_SYS_ALARMS_FILE_ADDRESS     (uint32_t)(428*EEPROM_PAGE_SIZE)
/*! @}*/
#define   FILE_SYS_ERR_LOAD_FLAG                0
#define   CTRL_ERR_LOAD_FLAG                    0
#define   DISPLAY_ERR_LOAD_FLAG                 0
#define   MTR_ERR_LOAD_FLAG                     0
#define   TRF_PYM_CFG_ERR_LOAD_FLAG             0
#define   COMM_ERR_LOAD_FLAG                    0
#define   CTRL_OPTIONAL_ERR_LOAD_FLAG           0
#define   CTRL_SECOND_OPTIONAL_ERR_LOAD_FLAG    0
#define   CTRL_NEW_SYS_ERR_LOAD_FLAG            0
#define   MTR_CFG_ERR_LOAD_FLAG                 0
#define   CTRL_VAR_ERR_LOAD_FLAG                0
#define   TRF_PYM_VAR_ERR_LOAD_FLAG             0
#define   ADE_ERR_CAL_LOAD_FLAG                 0
#define   CTRL_CFG_ERR_LOAD_FLAG                0
#define   CTRL_LAST_TAMPERS_LOAD_FLAG           0
   
#define EEPROM_PAGE_80 (uint32_t)(80ul* EEPROM_PAGE_SIZE)
#define FILE_SYS_CHECK_EEPROM_FAIL_ADRS    EEPROM_PAGE_80
#ifdef GPRS_SEND_NOTIFICATION_FEATURE
//3 bytes (struct gprs_Notification_st)
#define FILE_SYS_UNSENT_GPRS_NOTIFICATION_ADRS         FILE_SYS_GET_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS
#endif
#endif
#endif	// end of file
