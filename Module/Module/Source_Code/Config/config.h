#ifndef CONFIG_H_ 
#define CONFIG_H_
/*!
 * @file config.h
 * @brief This file contains common configuration for project code.
 * @details This file contains the required definitions to select required features for
 * certain project.\n
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
 *   <td> 03/03/2013 </td>
 *   <td> mohamed.essa  </td>
 *   <td> Modified to fit with doxygen tool. </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup METER Meter
 * @brief Meter modules documentation.
 */
/*******************************************************************************************************************************/
#define false 0
#define true  1

#define FALSE 0
#define TRUE  1


#define Micro_V85XX         1
#define Micro_V94XX         2

//you must make 1 space between MicroController & Micro_V94XX for script , Note:any change in this line will affect on script
#define MicroController Micro_V94XX




#if (MicroController == Micro_V94XX)
#include "V94XX.h"
#define Reactive_PWR_V94XX_Enable   1
#define EM130_BOARD                                // New Board V94XX With Internal Measuring 
#undef SPAIN_BOARD
#undef EM122_BOARD
#undef EM110_BOARD
#else
#include "v85xx.h"
#undef SPAIN_BOARD
#undef EM122_BOARD
#define EM122U_BOARD
#undef EM110_BOARD
#endif

#include "I2C_Interface.h"
#include "UART_Interface.h"
   
#define  DEVELOPMENT 0
#define  RELEASE     1
/******************************************************************************/

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, BitStatus, BitAction;



//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define U8_MAX     (255)
#define S8_MAX     (127)
#define S8_MIN     (-128)
#define U16_MAX    (65535u)
#define S16_MAX    (32767)
#define S16_MIN    (-32768)
#define U32_MAX    (4294967295uL)
#define S32_MAX    (2147483647)
#define S32_MIN    (-2147483648uL)

/*******************************************************************************
                                Types definitions
 ******************************************************************************/
#define WRONG_ARGUMENT          0xff

#define ASHANTI_CRC  /*This feature adds CRC and backup.*/
#define __MSP430_HAS_EUSCI_Bx__
#define Dlms_NoParity    1
#define Dlms_EvenParity  0

/* Unsigned types */
typedef unsigned char      uint8;
typedef unsigned int       uint16;
typedef unsigned long int  uint32;

/* Signed types */
typedef signed char        int8;
typedef signed int         int16;
typedef signed long int    int32;
/* Declares an enumeration data type called BOOLEAN */
//
//typedef unsigned char bool_t;
//enum
//{
 // FALSE,     /* false = 0, true = 1 */
 // TRUE
//};
//enum
//{
  //false,     /* false = 0, true = 1 */
  //true
//};
//enum
//{
//  WRITE,
//  READ
//};

#undef NULL
#define NULL        (0)
/* Return functions codes */
#define iSUCCESS            (0)
#define iERROR              (-1)

typedef uint8_t   TU08;
typedef int8_t    TS08;
typedef uint16_t  TU16;
typedef int16_t   TS16;
typedef uint32_t  TU32;
typedef int32_t   TS32;
typedef uint64_t  TU64;
typedef int64_t   TS64;

typedef signed char         TBOOL;
/* Date Structure */
__packed typedef struct
{
  uint16_t uiYear;      // 0xFFFF mean that it is not specified
  uint8_t uiMonth;     // from 1 to 12; 1 is January, & 0xFD mean daylight saving end, 0xFE mean daylight saving begin, 0xFF mean that it is not specified
  uint8_t uiDay;       // from 1 to 31; & 0xFD mean 2nd last day of month, 0xFE mean last day of month, 0xFF mean that it is not specified
  uint8_t uiDayOfWeek; // from 1 to 7; 1 is Monday & 0xFF mean that it is not specified
  uint8_t uiReserved;
} TstDate;

/* Time Structure */
__packed typedef struct
{
  uint8_t uiHour;      // from 0 to 23; & 0xFF mean that it is not specified
  uint8_t uiMin;       // from 0 to 59; & 0xFF mean that it is not specified
  uint8_t uiSec;       // from 0 to 59; & 0xFF mean that it is not specified
  uint8_t uiHundredths;// from 0 to 99; & 0xFF mean that it is not specified
} TstTime;

/* Structure DateTime */
__packed typedef struct
{
  TstDate stDate;
  TstTime stTime;
  int16_t iDeviation;  // from -720 to 720 in minutes of local time to GMT; & 0x8000 mean that it is not specified
  uint8_t uiClockStatus;
  // Bit1: indicate time is invalid "may be due to power down"
  // Bit2: indicate doubful value "time is recovered but we can't guarante its value"
  // Bit3: indicate that at least on bit in the clock is invalid
  // Bit4, 5 & 6 are reserved
  // Bit6: indicate that daylight saving is active
  uint8_t uiReserved;
} TstDateTime;//14byte=size
// clock base
typedef enum
{
  NO       = 0,
  XTAL     = 1,
  FREQ_50  = 2,
  FREQ_60  = 3,
  GPS      = 4,
  RADIO    = 5
} TClockBase;
//=============================================================================
//                 MACROS DEFINITION
//=============================================================================
/*Relay number*/
enum
{
  Relay0 = 0,
  Relay1,
  Relay2
};

/*Control Actions*/
enum
{
  Connect = 0,
  Disconnect
};

#define MTR_SINGLE_PH              /*!< Single phase meter type.*/
#undef MTR_THREE_PH              /*!< Trhee phase meter type.*/
#undef RS485_ENABLE

#define LOCAL_WITH_DLMS 
#ifdef MTR_THREE_PH
#define MTR_REACTIVE
#define TRF_USE_MD_KVA
#endif
#ifdef LOCAL_WITH_DLMS

#undef CONSUMPTION_DEBUG
#undef G2_DM

// #define TRF_PYMT_POSTPAID
#endif
/* Convert Sec To Milli Sec */
#define TOOLS_mConvertSecToMillisec  1000ul
/**
    This macro builds a WORD from two bytes
    @param h This is the high significant byte
    @param l This is the low significant byte
    @return returns the WORD composed from the two bytes (h, l).
*/
#define TOOLS_mMakeWord(h, l) (TU16)((((TU16)(h)) << 8U)|((uint8_t)(l)))

/**
    This macro builds a DWORD from four bytes
    @param hh This is the high significant byte from the high significant word.
    @param hl This is the low significant byte from the high significant word.
    @param lh This is the high significant byte from the low significant word.
    @param ll This is the low significant byte from the low significant word.
    @return returns the DWORD composed from the four bytes (hh, hl, lh, ll).
*/
#define TOOLS_mMakeDoubleWord(hh, hl, lh, ll)          \
                             (TU32)((((TU32)(hh)) << 24U) | \
                             (((TU32)(hl)) << 16U) | \
                             (((TU32)(lh)) << 8U)  | \
                             (TU32)(ll))
/**
    This macro returns the low significant BYTE from a WORD
    @param w This is the word.
    @return @returns the low significant BYTE
*/
#define TOOLS_mLowByte(w) ((uint8_t)(w))
/**
    This macro returns the high significant BYTE from a WORD
    @param w This is the WORD.
    @return @returns the high significant BYTE
*/
#define TOOLS_mHighByte(w) ((uint8_t)((w)>>8U))
/**
    This macro returns the low significant WORD from a DWORD
    @param dw This is the DWORD.
    @return @returns the low significant WORD
*/
#define TOOLS_mLowWord(dw) ((uint16_t)(dw))
/**
    This macro returns the high significant WORD from a DWORD
    @param dw This is the DWORD.
    @return @returns the high significant WORD
*/
#define TOOLS_mHighWord(dw) ((uint16_t) (((uint32_t) (dw) >> 16) & 0xFFFF))

/* Some useful macros */
#define TOOLS_mGetMax(a, b) (((a) > (b)) ? (a) : (b))
#define TOOLS_mGetMin(a, b) (((a) < (b)) ? (a) : (b))

#define SET_BIT(a,b)  ((a)|=(b))
#define CLR_BIT(a,b)  ((a)&=~(b))
#define GET_BIT(a,b)  ((((a) & (b)) == (b)) ? (1): (0))


#define RET_SUCCESS         (uint8_t)0x00
#define RET_FAILD           (uint8_t)0x01
//#define Dlms_NoParity    0x00
//#define Dlms_EvenParity  0x01

/******************************************************************************/

/*******************************************************************************
                           Meter Configuration
*******************************************************************************/
#define  DATA_BUFFER_SIZE           402
#if (MicroController == Micro_V94XX)
#define DOT_MATRIX_LCD_ENABLE
#else
#undef DOT_MATRIX_LCD_ENABLE
#endif
#undef STS_ENABLE
#define HW_SPI
//#define SW_SPI
#define OPTICAL_CTRL_FEATURE
#undef CONSOL_FEATURE
//#define MTR_ACCRA_NEW
#undef MTR_ACCRA
//#define PRODUCTION
//#define SINGLE_PH_DLMS_METER
#define THREE_PH_DLMS_METER

#define SAG_ZXTO_FLAGS
#undef LITTLE_ENDIAN
#undef BIG_ENDIAN
#ifdef MTR_SINGLE_PH
#define BIG_ENDIAN
#else
#define LITTLE_ENDIAN
#endif
#define BCP_LEBANON 0
#define BCP_INDIAN  1
#define BCP_UNIVERSAL 2
#define BCP_VERSION BCP_LEBANON

#define SHARED_I2C			1
#define SEPARATE_I2C		2
#ifdef MTR_ACCRA
#define I2C_STATUS		SEPARATE_I2C
#else
#define I2C_STATUS		SEPARATE_I2C
#endif
#undef     TEST_YMODEM
//#define NEW_INITIALIZATION_FLAG  
/********************************** Headers ***********************************/


#define LOCAL_MARKET_FEATURE    // undef this line for Ashanti version
#undef GROUP_METER_FEATURE


#define _IAR_COMPILER_VERSION_

#define RECORD_NEW_CUSTOMER_DATE
#define SERIAL_NUMBER_CHANGE
/*==========================================================================================
                               LCD (WITH INDIRECT SYMBOLS)
==========================================================================================*/
#define USE_NEW_LCD    // LCD with new symbols G and M for indirect meters

/*==========================================================================================
                               Features of Metering System
==========================================================================================*/
#undef CURRNET_DEBUG 
#define CALIBRATION_CODE


#ifdef MTR_ACCRA
#define USE_NEW_LCD    // LCD with new symbols G and M for indirect meters
#define EDIT_TIME_DATE_MANUALLY
#endif
#define MTR_LOAD_PROFILE          /*!< Enable load profiling.*/
#ifdef MTR_LOAD_PROFILE
// TODO: We Minimize the record size becasue we don't have size in EEPROM for 3 phase
#define PROFILE_RECORD_NUM /*1402*/ 3360   /*!< Maximum number of load profile records to be saved.*/
#define PROFILE_RECORD_TEST_MODE_NUM    20 
#else
#define PROFILE_RECORD_NUM 0      /*!< 0 in case the feature is disabled.*/
#endif


#define MTR_RVS_TMPR              /*!< Enable reverse tamper detection and calculations.*/
#define MTR_ERTH_TMPR             /*!< Enable earth tamper detection and calculations.*/
#define MTR_ENABLE_LMT            /*!< Enable limiter application.*/
#undef LOAD_LIMIT_PLAN          /*!< Enable load limit plan.*/
#if ((defined (MTR_SINGLE_PH)) && ( defined (MTR_THREE_PH)))
#error "Define only one type for the target." /*!< Error not defined meter type.*/

#elif (defined MTR_SINGLE_PH)

#if (MicroController == Micro_V94XX)

#undef V9203_ENABLE 
#undef ADE7953_ENABLE
#undef V9261f_ENABLE
#undef V9360_ENABLE

#define V9381_ENABLE
#define CONSTANT_3200                               

#else
#undef CONSTANT_3200                               
#undef V9203_ENABLE 
#undef ADE7953_ENABLE
#undef V9261f_ENABLE
#define V9360_ENABLE

#endif
#undef GROUP_METERING


#define MTR_DIRECT
#define MTR_NUM_OF_PHASE 1        /*!< Number of phases.*/
#ifdef MTR_ERTH_TMPR
#define ADE7953_EARTH_ENABLE      /*!< Enable ADE7953 earth detector.*/
#ifdef MTR_MANUAL_CAL
#define ADE7953_GAIN_CAL_EN       /*!< Enable power gain calibration 230/5/UPF.*/
#define ADE7953_PHASE_CAL_EN      /*!< Enable phase calibration 230/5/0.5PF.*/
#define ADE7953_OFST_CAL_EN       /*!< Enable power offset calibration 230/0.1/UPF.*/
#undef ADE7953_RMS_CAL_EN         /*!< Enable rms calibration 120/0.01/UPF.*/
#endif
#endif
#define MTR_NUM_OF_CH    2        /*!< Define the number of current channel for the currrent defined metering driver.*/
#elif (defined MTR_THREE_PH)
//#define MTR_INDIRECT
#define MISMATCH_NEUTRAL_FEATURE

#define MTR_NUM_OF_PHASE 3        /*!< Number of phases.*/
#define MTR_NUM_OF_CH    3        /*!< Define the number of current channel for the currrent defined metering driver.*/

#define  MTR_DIRECT  /*!< Enable direct calculations.*/

#define CTRL_GPRS_MODULE_CVR_SW

#define INTERNAL_METERING
#undef ADE7953_ENABLE
#undef V9261f_ENABLE
#undef V9360_ENABLE
#define V9203_ENABLE            /*!< Enable ADE7758 driver.*/  
#ifdef MTR_ERTH_TMPR
#define ADE7758_ERTH_ENABLE       /*!< Enable ADE7953 earth detection feature.*/
#endif
#ifdef V9203_ENABLE
#define V9203_PH_SEQ_TMPR       /*!< Enable ADE7953 phase sequence error detection feature.*/
#undef  ADE7758_PHASE_CAL
#undef  PHASE_CAL_PER_PHASE
#undef  ADE7758_OFST_CAL
#endif
#ifdef MTR_THREE_PH
#define MTR_MIS_POT_TMPR            /*!< Enable missing potential tamper detection and calculations.*/
#define MTR_PH_SEQ_TMPR             /*!< Enable phase sequence tamper detection and calculations.*/
#endif
#undef MTR_FREQ_CORRECTION           /*!< Enable the frequency correction recommended for class .2 only*/
#define PULSE_COUNT_ENABLE          /*!<Enable Pulse counter function*/
#else

#error "No metering type defined."
#endif
#if (defined (MTR_DIRECT) && defined (MTR_INDIRECT)) || ((!defined (MTR_DIRECT) && !defined (MTR_INDIRECT)))
#error "you must define indirect or direct."                              
#endif
#ifdef MISMATCH_NEUTRAL_FEATURE
#define NEUTRAL_FEATURE
#endif
/*==========================================================================================
                                   Features of Display
==========================================================================================*/
#undef  FOR_DIAL_TEST
#undef BACK_LIGHT_TIMEOUT_FEATURE
#define CONSTANT_SCREEN_ORDER 0
#define CHANGEABLE_SCREEN_ORDER 1
/*! Used determine screens order, it defines by CHANGEABLE_SCREEN_ORDER or CONSTANT_SCREEN_ORDER */
#ifdef ASHNTTI_PROJECT
#define DISPLAY_SCREEN_ORDER   CONSTANT_SCREEN_ORDER
#else
#define DISPLAY_SCREEN_ORDER   CHANGEABLE_SCREEN_ORDER
#endif
#define LCD_HQ30774_ENABLE 
#define LCD_CNKD0802_24SEG_8COM
#define LCD_NEW_CNKD0802_24SEG_8COM
#define ENGLISH_SCREEN
#undef ARABIC_SCREEN
#ifdef ARABIC_SCREEN
#define  CD0063_MH6530AHSP_arabic
#else
#ifdef ENGLISH_SCREEN
#define CD0066_MH6531AHSP_ENGLISH
#endif
#endif
#define DISPLAY_OBIS		0
#undef NEW_LCD_ST7033_ENABLE /*!< Uncomment for the new ashanti lcd driver*/
//#define LCD_ST7033_ENABLE /*!< Uncomment for the old Lebanon lcd driver*/
                               
#define RECORD_NEW_CUSTOMER_DATE
#define SERIAL_NUMBER_CHANGE
#define DISPLAY_MAP_SCREEN  //remap screen and subscreens
#define TRF_GRACE_48_HOURS_COMPARING_TIME     
#ifdef MTR_SINGLE_PH
#define NEW_TENDER_BLUETOOTH
#endif
/*==========================================================================================
                            Features of Tariff & Payment System
==========================================================================================*/
#define TARIFF_SYS /*!< Enable all tariff & payment system with minimum features.*/
#undef LIBYA_MONEY
   #define TRF_NKW_TAX
#ifdef TARIFF_SYS
#define OPTIMIZATION_TARIFF
#define TRF_BP_HISTORY /*!< Enable addition of BPH records with maximum 12 records.*/
#ifdef TRF_BP_HISTORY
#define TRF_BP_HISTORY_RECORDS 12 /*!< Number of required records of billing history.*/
#else
#define TRF_BP_HISTORY_RECORDS 0  /*!< 0 in case of disabled featur.*/
#endif
#define TRF_USE_MD_KW             /*!< Record MD of active energy feature.*/
//#define TRF_USE_MD_KVA            /*!< Record MD of apparent energy feature.*/
#define TRF_USE_MD_A              /*!< Record MD of current feature.*/
#define PAYMENT_SYS               /*!< Enable money calculations.*/
#ifdef PAYMENT_SYS
#define PYMT_LOW_TWO_LVL          /*!< Enable two level detection of low credit.*/
#ifdef PYMT_LOW_TWO_LVL           /*!< Number of levels of low credit is 2.*/
#define PMYT_LVL     2
#else
#define PMYT_LVL     1            /*!< Number of levels of low credit is 1.*/
#endif
#define PYMT_MONY_TRANS           /*!< Enable money transaction logging.*/
#define PYMT_TAX                  /*!< Enable Addition of taxes.*/
#define PYMT_FRIENDLY             /*!< Enable friendly periods feature.*/
#define PYMT_VACATION_TRF         /*!< Enable special tariff at vacation feature.*/
#endif
#ifdef PYMT_MONY_TRANS
#define PMYT_MNY_TRANS_REC 20     /*!< Maximum number of money transaction records to be saved.*/
#else
#define PMYT_MNY_TRANS_REC 0      /*!< 0 in case the feature is disabled.*/
#endif
#endif

/*==========================================================================================
                              Features of Communication System
==========================================================================================*/
#undef DEBUG_LOW_POWER
//#define DEBUG_BOOTLOADER
//#define DEBUG_RTC_TIME
//#define DEBUG_HANG_LOOPING
#if (!defined MTR_ACCRA_NEW && !defined MTR_ACCRA)
#undef RF_LINK_FEATURE  /*!< Enable RF_Link.*/
#endif
#undef EF_RF_COMMUNICATION
#undef RF_CC1120
#undef RF_LINK_ENABLE
#ifdef RF_LINK_FEATURE
#define RF_LINK_COLLECTOR           1
#define RF_LINK_METER           0
#define RF_LINK_MBUS_DEVICE         RF_LINK_METER /*!< It's flag, it represents Module work as RF_LINK_COLLECTOR = 1 or RF_LINK_METER = 0.*/
#define RF_LINK_TI_CC1120_ENABLE  /*!< Enable using TI_CC1120 driver.*/
#ifdef RF_LINK_TI_CC1120_ENABLE
#define RF_LINK_TI_CC1120_PORT      USART2    /*!< To show each serial port to communicate with modem.*/
#define RF_LINK_TI_CC1120_RESET_POWER
#endif
#endif
#ifndef MTR_ACCRA_NEW 
#define RFID_FEATURE            /*!< Enable using RFID system.*/
#endif
#ifdef RFID_FEATURE
#undef CONTACT_CARD
#undef CONTACT_CONTACTLESS
#undef RFID_DEBUG_MEASUR_SPEED
#undef RFIDTEST_READ_WRITE_TOOL
#define RFID_BUFFER_LEN 1700//2000 minimum length 100
#define RFID_PAR_LIMITED
#ifdef RFID_PAR_LIMITED

#define RFID_PREPAID_PAR_NUM    100
#define RFID_PAR_PREDEFINED
#define RFID_PAR_POSTDEFINED
#ifdef RFID_PAR_POSTDEFINED
#define POST_DEFINED_WITH_1000_FEATURE
#ifdef POST_DEFINED_WITH_1000_FEATURE
#define RFID_POSTPAID_PAR_NUM    1000
#else
#define RFID_POSTPAID_PAR_NUM    100
#endif
#endif
#if (RFID_BUFFER_LEN >= 1000)
#undef RFID_SPLIT_DATA
#else
#define RFID_SPLIT_DATA
#endif
#endif
#define MFRC522_ENABLE   /*!<Determine the used driver with its configurations. */
#ifdef  MFRC522_ENABLE
#define MFRC522_CARD_1K 1             /*!< Card type = 1k card.*/
#define MFRC522_CARD_4K 4             /*!< Card type = 4k card.*/
#define MFRC522_CARD_32K 32             /*!< Card type = 4k card.*/
#define MFRC522_KEY     {0xff,0xff,0xff,0xff,0xff,0xff }  /*!< Define security key of RFID card.*/
#define MFRC522_I2C_ADDRESS      0x50         /*!< Define I2C adress of MFRC522 chip.*/
#define MFRC522_I2C_BASEADDRESS  EUSCI_B0_BASE
#define MFRC522_CARD_TYPE   MFRC522_CARD_32K    /*!< Define MFRC522 card type 1k or 4k or 32k.*/

#define CARD_32k_MASRIA_NXP_P60     0x01
#define CARD_32k_MASRIA_MTCOS_FLEX_ID   0x02
#define MFRC522_CARD_32k_TYPE    CARD_32k_MASRIA_MTCOS_FLEX_ID

#if( MFRC522_CARD_TYPE == MFRC522_CARD_32K)
#define MFRC522_INCREASE_SPEED
#define RFID_SEQ_READ_WRITE
#elif (( MFRC522_CARD_TYPE == MFRC522_CARD_4K) ||( MFRC522_CARD_TYPE == MFRC522_CARD_1K))
#undef  RFID_SEQ_READ_WRITE
#endif
#endif
#endif
#if defined (DEBUG_LOW_POWER) || defined (DEBUG_HANG_LOOPING)|| defined (DEBUG_BOOTLOADER) || defined (DEBUG_RTC_TIME)
#undef  IEC_62056_21_SLAVE
#define IEC_BUFFER_SIZE 1056
#else
/*************************Enable HDLC OR IEC_62056*************************/
#define IEC_BUFFER_SIZE 350
#define IEC_62056_21_ISR_ENABLE
#undef HDLC_PROTOCOL
#define IEC_62056_21_SLAVE /*!< Enable of protocol of IEC 62056-21.*/
#if (defined (HDLC_PROTOCOL) && defined (IEC_62056_21_SLAVE))
//#error "select only one protocol HDLC or IEC_62056"
#elif defined (HDLC_PROTOCOL)
#undef IEC_62056_21_SLAVE
#elif defined (IEC_62056_21_SLAVE)
#undef HDLC_PROTOCOL
#define IEC_LOCAL_BUFFER_SIZE 20 /*!< Size of local buffer of IEC protocol.*/
#define IEC_SPECIFIC_BAUD_RATE '4' /*!< Baud rate of transferring data packets.*/
#define IEC_TIMEOUT_SEC 7/*!< Timeout in seconds of IEC protocol maximum 255 seconds.*/
#endif
#endif

#undef PLC_MODEM
#undef PLC_MODEM_IEC_TEST

#undef GPRS_FEATURE /*!< GPRS feature enable.*/
#ifdef GPRS_FEATURE
#define GPRS_SEND_NOTIFICATION_FEATURE  /*!< Send Charging money notification or billing period notification.*/
//#define MORE_CLIENT_IP_FEATURE  /*!< make more client to connect to meter depending on the pirority of the ip .*/
#define SCREEN_GPRS_FEATURE
#define GPRS_DATA_BUFFER_SIZE 1500
#define GPRS_SEND_NOTIFICATION_FEATURE  /*!< Send Charging money notification or billing period notification.*/
#undef GPRS_DEBUG
//#define GPRS_DEBUG
#undef GPRS_BOOTLOADER_DEBUG
//#define GPRS_BOOTLOADER_DEBUG
#define  GPRS_SRT_CARD
#define MODEM_ENABLE /*!< Enable using SIM900 GPRS driver.*/
#ifdef MODEM_ENABLE
#undef CINTERION_ENABLE
#undef SIM900_ENABLE
#undef UC15_ENABLE
#define EC200_ENABLE
#ifdef EC200_ENABLE
#define MODEM_BAUD_RATE '7' /*!< Baud rate of transfering data packets.*/
#else
#define MODEM_BAUD_RATE '5' /*!< Baud rate of transfering data packets.*/
#endif
#endif
#endif
/*===========================================================================================
                                    AES Encryption Module
 ===========================================================================================*/
#undef AES_ENCRYPTION
/*==========================================================================================
                            Features of Control System
==========================================================================================*/
typedef uint32_t ser_t;        /*!< Meter serial number type.*/
#define CTRL_RFID_BOARD_PIN_ENABLE
//#undef POWER_FAIL_PIN_FEATURE
#define POWER_FAIL_PIN_FEATURE
#define CTRL_BTRY_NON_CHRG 1 /*!< Meter has battery and battery type is non chargeable.*/
#define CTRL_BTRY_CHRG 0 /*!< Meter has battery and battery type is chargeable.*/
#define CTRL_SUPER_CAP 0 /*!< Meter hasn't battery but it uses super capacitor.*/
#if ((CTRL_BTRY_NON_CHRG + CTRL_BTRY_CHRG +  CTRL_SUPER_CAP)  != 1)
#error "Select only one option for battery"
#else
#define CTRL_BTRY
#endif
#define CTRL_MGNT_SW 0 /*!< Enable magnetic interference detection using magnetic switch.*/
#define CTRL_MGNT_SENSOR 0 /*!< Enable magnetic interference detection using ADC (magnetic sensor).*/
#if ((CTRL_MGNT_SW + CTRL_MGNT_SENSOR)  > 1)
#error "Select only one option for magnetic detection."
#endif
#ifdef ASHNTTI_PROJECT
#define CTRL_MD_SW /*!< Enable maximum demand switch control.*/
#else
#undef CTRL_MD_SW /*!< Enable maximum demand switch control.*/
#endif
//#define CTRL_GPRS_MODULE_CVR_SW /*!< Enable GPRS module cover control.*/
#define CTRL_CVR_SW /*!< Enable cover switch control.*/
#define CTRL_TRMNL_SW /*!< Enable terminal switch control.*/ 
#define CTRL_UP_SW /*!< Enable up switch control. @note If you need one scroll 
* switch, Use UP switch and remove DN switch feature.*/
#define CTRL_DN_SW /*!< Enable down switch control.*/
#define CTRL_ACTIONS /*!< Enable taking actions on coming events or tampers.*/
#undef  CTRL_AUX_LED
#undef  CTRL_SCROLL_SW

#ifdef CTRL_ACTIONS
#define CTRL_RLY /*!< Enable control of relay.*/
#define CTRL_TMPR_LED /*!< Enable control of tamper LED.*/
#define CTRL_LOW_CRDT_LED /*!< Enable control of credit LED.*/
#define CTRL_BZR /*!< Enable control of buzzer.*/
#define AC_BUZZER_FEATURE
#define CTRL_ALRM_ICON /*!< Enable control of alarm icon.*/
#endif
#if (CTRL_SUPER_CAP == 0)
#define CTRL_RTC /*!< Enable usage of RTC.*/
#define CTRL_EVNT_LOG /*!< Enable events logging.*/
#ifdef CTRL_EVNT_LOG
  /*! Maximum number of records to be saved in events log.*/
#define EVENT_LOG_RECORD_NUM 402
#else
#define EVENT_LOG_RECORD_NUM 0
#endif
#define CTRL_CFG_METER_LOG /*!< Enable configure meter logging.*/
#ifdef CTRL_CFG_METER_LOG
  /*! Enable of configure meter records to be saved in configure meter log.*/
#define CFG_METER_RECORD_NUM 30
#else
#define CFG_METER_RECORD_NUM 0
#endif
#endif
#undef DEBUG_OVERLOAD
  /*==========================================================================================
                               Features of File System
  ==========================================================================================*/
#undef FILE_SYS_USE_INT /*!< Enable using internal MCU memory in file system.*/
#if (defined(CTRL_EVNT_LOG ) || defined(PYMT_MONY_TRANS) || defined(MTR_LOAD_PROFILE) || \
  defined(TRF_BP_HISTORY) || defined(CTRL_CFG_METER_LOG) )
#define FILE_SYS_LOG /*!< Enable using logging APIs. */
#endif
#define FM24C128D_2_Wire_Serial_EEPROM
#define  FLASH_FM25W32_ENABLE
////////////////////////////////////////////////////////////////////////////////////////////
            //  Keypad Configuration
///////////////////////////////////////////////////////////////////////////////////////////
#undef  KEYPAD_FEATURE
#ifdef  KEYPAD_FEATURE
#define TOUCH_KEYPAD
#undef  RUBBER_KEYPAD
#endif

//=============================================================================
//                 System Clock
//=============================================================================
/* Define system clock to be 25MHz , it can be also 8000 '8Mhz' Or '16Mhz'*/
#define GENERAL_SYSTEM_CLOCK   25000
////////////////////////////////////////////////////////////////////////////////////////////
			//	DLMS Configuration
///////////////////////////////////////////////////////////////////////////////////////////
#undef DLMS_ENABLED
#undef DLMS_UI
#ifdef DLMS_UI
#undef RS485_ENABLE
#endif
#undef G2_SECURITY
#undef HW_Crypto_Chip
#undef SECURE_ELEMENT
#if defined(DLMS_ENABLED) || defined (HDLC_PROTOCOL)
#define DLMS_GPRS_CHANNEL                1
//#define DEBUG_GET_SPECIAL
#define NEW_DESIGN_OPTIMIZE
#define COC_DATA_MODLE
#define PRECENTAGE                              1
#define   SAG_SWELL_VALUE_TYPE                  PRECENTAGE

#define Meter_3Wire               		1  			//--> Choose this for 3 Wires indirect meter 110(63.5V)
#define Meter_Direct        	  		2   		//--> Choose this for 3 phases direct meter 220V
#define Meter_Direct_Street_Light 		3   		//--> Choose this for 3 phases direct with street light meter 220V
#define Meter_Indirect        			4		  	//--> Choose this for 3 phases Indirect 220V
#define Meter_InDirect_Street_Light 	5   		//--> Choose this for 3 phases Indirect with street light meter 220V
#define TI_EMETERING        			6  			 //--> Choose this for single phase meter with internal measurment.
 /*
    Direct = 3phase
    TI     = Single phase
 */
#define TENDER  Meter_Direct // Please Take care of Internal Metering Lib.
							 // If Required to change From 1 Phase to 3 Phase
							 //	Change NUM_PHASES From 1 to  Phase "metrology-template.h" under emeter-metrology Project Include/emeter-metrology

#define TOOLS_mIsNegative(VarS32)  (((VarS32)&0x80000000) == 0x80000000)
#define TOOLS_mIsPositive(VarS32)  (((VarS32)&0x80000000) != 0x80000000)
#define TOOLS_mIsPositiveNonZero(VarS32)  ((((VarS32)&0x80000000) != 0x80000000)&&(((VarS32)&0xFFFFFFFF) != 0x00000000))
#define TOOLS_mIsZero(VarS32)             ((((VarS32)&0x80000000) != 0x80000000)&&(((VarS32)&0xFFFFFFFF) == 0x00000000))
//pouzite vrtvy v HDLC kanale
#define HDLC_DLMS   0  //HDLC + DLMS AMx60/65
#define HDLC_TLS_SML  1  //HDLC + TLS + SML MESS2020
#define LAYERS  HDLC_DLMS

#undef CORE_VERSION
#undef CORE_REVERSION

/**** ------ Tasks handler ------- ****/
#define ACTIVITY_CALENDER_ENABLE
#define CALENDER_MANAGER_ENABLE
#define REGISTER_ACTIVATION_ENABLE
#define SPECIAL_DAYS_ENABLE
//#define TARIFF_SCRIPT_TABLE_ENABLE
#define TARRIF_REGISTERS_ENABLE
#define POWER_QUALITY_ENABLE
#define NEW_LIMITER_ENABLE
#define EVENT_LOG_ENABLED
#ifdef HDLC_PROTOCOL
#undef EVENT_NOTIFICATION_ENABLE
#else
#define EVENT_NOTIFICATION_ENABLE
#endif
#define LOAD_LIMIT_PLAN_ENABLE
#define PROFILE_DLMS_ENABLE
#define TAMPER_DLMS_ENALBE
#if ( defined (ACTIVITY_CALENDER_ENABLE)|| defined(CALENDER_MANAGER_ENABLE) ||defined(REGISTER_ACTIVATION_ENABLE) ||defined(SPECIAL_DAYS_ENABLE)\
    ||defined(TARIFF_SCRIPT_TABLE_ENABLE) ||defined(TARRIF_REGISTERS_ENABLE) ||defined(POWER_QUALITY_ENABLE) ||\
    defined(NEW_LIMITER_ENABLE) ||defined(EVENT_LOG_ENABLED) ||defined(EVENT_NOTIFICATION_ENABLE) ||defined(LOAD_LIMIT_PLAN_ENABLE)\
      ||defined(PROFILE_DLMS_ENABLE) ||defined(TAMPER_DLMS_ENALBE) )

#define FLASH_APP_GENERIC_FEATURE
#endif
/******** ------------- *******/

   //******************************************************************************************
//                PLATFORMS
//*****************************************************************************************
#define PLAT_ZPA_AMX6X        0//ZPA meters AMx60/65
#define PLAT_PC           1//PC platform
#define PLAT_ESW          2//Elsewedy meters

//odvozene volby prekladu z pohledu ciloveho procesoru (AMx60 x AMx65, merici x komunikacni MCU)
#define AMX60_MC_MCU    0 //AMx60 - vsechno je v jednom procesoru
#define AMX65_COMM_MCU    1 //AMx65 - Komunikacni procesor
#define AMX65_MC_MCU    2 //AMx65 - Merici procesor

//******************************************************************************************
//choose the platform
//*****************************************************************************************
#define PLAT_PLATFORM       PLAT_ESW

//#define uiIxHdlc          1

#define TARGET    AMX60_MC_MCU
//=============================================================================
//                 Software version
//=============================================================================
#define Dlms_mSoftwareVersion             "0108"

//=============================================================================
//                 Meter type
//=============================================================================
//#define _MONO_    /* Monophase meter */
#undef _MONO_       /* Triphase meter */

//=============================================================================
//                 IP profile
//=============================================================================
#if (BCP_VERSION == BCP_LEBANON)
#define _IP_		/* manage TCP/IP, UDP */
#endif

//=============================================================================
//                 Platform
//=============================================================================
//#define _WIN32_     /* windows platform */
#undef _WIN32_    /* embedded platform */

//=============================================================================
//                 Cyphering
//=============================================================================
//#define _GCM_     /* windows platform */
#undef _GCM_    /* embedded platform */

//=============================================================================
//                 TYPES DEFINTION
//=============================================================================
#define RET_SUCCESS         (uint8_t)0x00
#define RET_FAILD           (uint8_t)0x01

/* Date Structure */

__packed typedef struct {
  uint16_t Class_ID;
  uint8_t Obis[6];
  int8_t Attribute_Index;
  uint16_t Data_Index;
} TCapture_Object;


//control state for disconnector
typedef enum {
  DISCONNECTED = 0,
  CONNECTED = 1,
  READY_FOR_CONNECTION = 2
} TControlState;

//control mode for disconnector
typedef enum {
  CONTROL_MODE_0 = 0,
  CONTROL_MODE_1 = 1,
  CONTROL_MODE_2 = 2,
  CONTROL_MODE_3 = 3,
  CONTROL_MODE_4 = 4,
  CONTROL_MODE_5 = 5,
  CONTROL_MODE_6 = 6,
} TControlMode;

typedef enum
{
    SYM_OFF = 0,
    SYM_BLINK,
    SYM_ON,
} TSymState;
/* Baude Rate for Infra red Communication */
typedef enum
{
  Dlms_eBaud300 = 0,
  Dlms_eBaud600,
  Dlms_eBaud1200,
  Dlms_eBaud2402,
  Dlms_eBaud4800,
  Dlms_eBaud9600,
  Dlms_eBaud19200,
  Dlms_eBaud38402,
  Dlms_eBaud57600,
  Dlms_eBaud115200,
  Dlms_eMax_Baud
}TDlms_eBaudRate;

/* Infra-red communication mode */
typedef enum
{
  Dlms_eModeIec = 0,
  Dlms_eModeDlmsUA
}TDlms_eModeLocalPort;

#ifdef _IP_
/* Ip profile */
typedef enum
{ /* TCP */
  Dlms_eIpTcp = 0,
  /* UDP */
  Dlms_eIpUdp,
}TDlms_eIpCommunicationProfile;
#endif /* _IP_ */

//=============================================================================
//                 MACROS DEFINTION
//=============================================================================
/* nb databits */
#define Dlms_m8DataBits					8
#define Dlms_m7DataBits					7
/* Parity */
#ifdef _WIN32_
#define Dlms_NoParity					NOPARITY
#define Dlms_EvenParity					EVENPARITY
#else
#define Dlms_NoParity					1
#define Dlms_EvenParity					0
#endif

#define  RS485_FEATURE
/* Id String "SWE" */
#define Dlms_mIecIdNameChar1			'S'
#define Dlms_mIecIdNameChar2			'E'
#define Dlms_mIecIdNameChar3			'E'
/* Char for Identification */
#define Dlms_mIecIdChar					'2'

/* Maximum number of attempts to enter the password */
#define Dlms_mPasswordMaxRetry          3

/* Wait 5 hours (5 * 60 * 60 seconds) after  Dlms_mPasswordMaxRetry  */
/* attempts to enter the password */
#define Dlms_mPasswordTimeOut			18000
/* Max Size of HDLC Buffer */
#ifdef HDLC_PROTOCOL
#define Dlms_mHdlcMaxFrameSize			2048
#else
#define Dlms_mHdlcMaxFrameSize			140
#endif
/* Max APDU */
#define Dlms_mAppMaxApduSize			512 // TODO: if you changed this value you must change "Dlms_Asso_mMaxPduSize" in Dlms_Itf.h also
/* Buffer XDR */
#define Dlms_mXdrMaxBufferSize			1024//2048
/* Max phy buffer */
#define Dlms_mIpLinkMaxFrameSize		Dlms_mAppMaxApduSize + 8
/* Time Slice : every 10ms */
#define Dlms_mOsTimeSlice				10
/* Inactivity timer 120 seconds */
#define Dlms_mHdlcInactivityTime		120ul

/* Configure the start index of the Block in Block transfer */
#define START_BLOCK_INDEX                        1   /* Start block index 0 or 1*/

/*---------------------------------------------------*/
/*                IR communication                   */
/*---------------------------------------------------*/
/* Com Port */
#define Dlms_mIrCom						13
/* Default Br */
#define Dlms_mIrDefaultBaudrate			Dlms_eBaud300
/* Default Mode : For CTT Hdlc tests we must put it equal to Dlms_eModeDlmsUA */
#define Dlms_mIrCommunicationMode		Dlms_eModeIec //Dlms_eModeDlmsUA
/* Normal BR */
#define Dlms_mIrNormalBaudrate			Dlms_eBaud9600
/* Frame timout (in 10 milliseconds) for IR communication */
#define Dlms_mIrItb						20

/*---------------------------------------------------*/
/*                RS communication                   */
/*---------------------------------------------------*/
/* Com Port */

#define Dlms_mRsCom						1
/* Normal baudrate */
#define Dlms_mRsBaudrate                Dlms_eBaud9600
/* Comm Params : Data bit */
#define Dlms_mRsDatabits				Dlms_m8DataBits
/* Comm Params Parity */
#define Dlms_mRsParity					Dlms_NoParity
/* Time Out RS in 10*ms */
#ifdef RS485_NETWORK
#define Dlms_mRsItf						2
#else
#define Dlms_mRsItf						2
#endif

#ifdef _IP_
/*---------------------------------------------------*/
/*                IP communication                   */
/*---------------------------------------------------*/
/* Timer Inactivity = 180 seconds = 3 min */
#define Dlms_IpWrap_mInactivityTime     55
/* Port for IP/cosem */
#define Dlms_IpWrap_mPort               4059
/* Mode Comm Profil TCP or UDP */
#define Dlms_IpWrap_mDefaultCommProfile Dlms_eIpTcp
/* Time Out IP in 10*ms */
#define Dlms_IpWrap_mItf                 1
#endif /* _IP_ */

/*---------------------------------------------------*/
/*                                DLMS PASSWORD                               */
/*---------------------------------------------------*/
#define DLMS_PASSWORD "S07%_HAI&MH_WA&S" // TODO: make it configurable
#endif //_DLMS_
/* The paswwords between the software and the meter*/
#define PASSWORD_SIZE	32
#undef RETRIEVAL_TRICK
//#define RETRIEVAL_TRICK


#define FIRMWARE_VERSION_FOR_COMMUNICATION   25
#define EDIT_TIME_DATE_MANUALLY
#undef REMOVE_MANUALLY_SET_DATE // TODO
#define LOCAL_NEW_MODIFICATION

/*-------------- hardware board version -----------*/
#ifdef MTR_THREE_PH
#ifdef MTR_DIRECT
#define  HARDWAREWARE_VERSION     "SWD-PPM70-V2.20V-0319"
#else//indirect
#endif//MTR_DIRECT
#else//Single
#define  HARDWAREWARE_VERSION     "SWD-GHA01-V1-00-0719"
#endif//MTR_THREE_PH
////////////////////////////////////////////////////////////////////////////////////////  //
  /*==========================================================================================
                               End of Features Configuration
  ==========================================================================================*/
  /*! \mainpage
  * \section Introduction
  * The objective of this Meter Core Library is to house the various forms of
  * firmware modules used to configure and compile a fully working meter.
  * This is library contains mostly all market needed features in metering industry
  * , which reduces the development process of meter firmware to only selecting and
  * configuring different library modules.\n
  * \section SECTION1 Current Supported Modules
  * -# Metering and limiters application.
  * -# ADE7953 Driver.
  * -# Control and tampers application.
  * -# Tariff and payment application.
  * -# File system application.
  * -# Display application.
  * -# LCD_HQ30774 Driver.
  * -# Communication apllication.
  * -# IEC62056-21 application used with optical and RS485 interfaces.
  * -# RFID application.
  * -# MFRC522 Driver.
  * -# GPRS application.
  * -# MODEM driver.
  * \section SECTION2 Core Team
  * - <a href="http://www.linkedin.com/profile/view?id=27968199">Waleed Kadry </a>
  * - <a href="http://www.linkedin.com/in/mohamedzakaria">Mohamed Zakaria </a>
  * - <a href="http://www.linkedin.com/in/ahmadibraheem">Ahmad Ibraheem </a>
  * - <a href="mohamed.essa@sewedy.com.eg">Mohamed Ismail Essa </a>
  * - <a href="http://eg.linkedin.com/pub/islam-elshahat/45/3b8/760">Islam Elshahat </a>
  */
#define STATUS_SUCCESS  0x01
#define STATUS_FAIL     0x00


#define HWREG32(x)                                                              \
    (*((volatile uint32_t *)((uint16_t)x)))
#define HWREG16(x)                                                             \
    (*((volatile uint16_t *)((uint16_t)x)))
#define HWREG8(x)                                                             \
    (*((volatile uint8_t *)((uint16_t)x)))

#endif /*end 0f #ifndef METER_CONFIG_H_ defined */
