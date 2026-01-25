
/*******************************************************************************
**                                                                            **
**  PROJECT   :                                                               **
**                                                                            **
**  File Name : Basic_Srv.h                                                   **
**                                                                            **
**  File Type : header file for Services.                                     **
**                                                                            **
**  AUTHOR    : Mohamed Mahmoud Zakaria                                       **
**                                                                            **
**  PURPOSE   : Provide basic services                                        **
**                                                                            **
**  History   : May 31, 2011                                                  **
**                                                                            **
**  Version   : 1.0                                                           **
*******************************************************************************/





#ifndef BASIC_SRV_H_  /* To eliminate including the file more than one time */
#define BASIC_SRV_H_







/********************************** Headers ***********************************/

#include "config.h"      /* Header file contains the standard types used */
#include "RTC_Interface.h"
#include <string.h>

/******************************************************************************/

/**************************  Definitions  *************************************/
enum
{
  ON_BATTERY,
  ON_POWER
};
enum
{
  checksum,
  CRC
};

#define WIDTH 8
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0x131

/* @par Description:
*
*/
typedef enum
{
  BS_LITTLE_ENDIAN_TYPE = 0,
  BS_BIG_ENDIAN_TYPE = 1
} endian_type_t;

__packed typedef struct
{
	uint8_t sec;          /*!< Seconds(0-59).*/
	uint8_t min;          /*!< Minutes(0-59).*/
	uint8_t hr;           /*!< Hours, 24 hour system(0-23).*/
	uint8_t day;          /*!< Days(1-31)*/
	uint8_t month;        /*!< Months(1-31)*/
	uint16_t year;         /*!< Years(0-99) +2000*/
} G2_date_time_t;
/**************************  Definitions  *************************************/

/*******************************************************************************
                         External Global Variables
*******************************************************************************/
extern uint8_t Disp_Count;
extern uint8_t screen_sw_up;
/*------------------------------------------------------------------------------
                End External Global Variables
------------------------------------------------------------------------------*/



/*******************************************************************************
                           Functions prototypes
 ******************************************************************************/
#define SET_BIT(a,b)  ((a)|=(b))
#define CLR_BIT(a,b)  ((a)&=~(b))
#define GET_BIT(a,b)  ((((a) & (b)) == (b)) ? (1): (0))

#define SRV_CALC_CRC(COMMAND,SIZE)      calc_crc(COMMAND,SIZE)

#define ENCRYPTED_KEY_SIZE  16

#define SECONDS_IN_A_MINUTE 60
#define SECONDS_IN_AN_HOUR 3600
#define SECONDS_IN_A_DAY 86400

#define DAYS_IN_A_YEAR 365
#define DAYS_IN_A_LEAP_YEAR 366
/*!
 * @brief Return the CRC of system variables structure
 *
 * @par Description:
 * Return the CRC of system variables structure
 *
 * @return uint8_t CRC of system variables structure\n
 *
 * @note CRC is the XOR of all memory bytes of configuration structure.\n
 * To distinguish the first case of EEPROM usage from the case the all data are zeros,
 * we assign value 0x55 to the CRC when it is calculated to be zeros or ones.
 */
uint8_t calc_crc(uint8_t* data_ptr, uint16_t size,uint8_t calc_type);
void vGOTO_PWRdown(unsigned char PwrDwnType);
void vWakeup_Sys(void);
void vDelay_SW(uint32_t u32LoopTimes);
void vDelay_HW(uint8_t u8SecCount);
void vEXTI_Config(void);
uint8_t BS_check_all_zeros (uint8_t* buff , uint16_t len);
uint8_t BS_get_curr_month_days (uint8_t month , uint8_t year);

uint8_t u8Int_ToDecimal(uint8_t* u8ptrbuf, uint32_t u32IntValue);
void int_to_ascii(uint8_t* u8ptrbuf, uint32_t u32int_value, uint8_t num_digits);
uint32_t ascii_to_int(uint8_t *data_ptr, uint8_t digit_num);
void Load_32BitData(uint32_t *DEST, uint8_t *Source);
void Store_32BitData(uint8_t *DEST, uint32_t*Source);
void vReset_Buffer(uint8_t *u8ptrbuf, uint8_t u8Length);
void vCopy_u8Buf(uint8_t *u8ptrDest, uint8_t *u8ptrSrc, uint8_t u8Lenght);
uint8_t u8Comper_u8Bufs(uint8_t *u8ptrSRC, uint8_t *u8ptrDES, uint8_t u8Length);
void vSave16bIn8bArr(uint8_t *Buffer, uint16_t *variable);
void vSave32bIn8bArr(uint8_t *Buffer, uint32_t *variable);
//uint8_t calc_crc_only(uint8_t* data_ptr, uint16_t size);
uint8_t bser_calc_day_of_week(uint8_t day, uint8_t month, uint8_t year);
uint32_t convertToLong32(uint8_t* a, uint8_t endian);
uint16_t convertToInt16(uint8_t* a, uint8_t endian);
uint16_t bser_calc_day_of_year(uint8_t day, uint8_t month, uint8_t year);
uint8_t BS_check_valid_date (uint8_t day , uint8_t month , uint8_t year);
void Store_16BitData(uint8_t *DEST, uint16_t* Source);
void vDelay_SW_RFID(uint32_t u32LoopTimes);
uint8_t itoa_generic (uint8_t* u8ptrbuf, uint32_t u32int_value);
uint8_t atoi_generic (uint8_t* u8ptrbuf, uint32_t* u32int_value , uint8_t len);

uint8_t BS_check_all_with_Value (uint8_t* buff ,uint8_t value_compared,  uint16_t len);

/*
 * brief Compare a date with today.
 *
 * par Description:
 * Compare given date with current time to check if given date is elapsed or not.
 *
 * param [in] day day to compare with.
 * param [in] month month to compare with.
 * param [in] year year to compare with.
 *
 * return
 * int32_t difference between today and date entered in days, all monthes are
 * considered 31 days.
 */
int32_t comp_date(uint8_t u8day, uint8_t u8month, uint8_t u8year);

int32_t comp_time(uint8_t u8second, uint8_t u8minute, uint8_t u8hour);
/*
 * brief Compare a date with today.
 *
 * par Description:
 * Compare given date with current time to check if given date is elapsed or not.
 *
 * param [in] day day to compare with.
 * param [in] month month to compare with.
 * param [in] year year to compare with.
 *
 * return
 * int32_t difference between date entered and today in days, all monthes are
 * considered 31 days.
 */
int32_t comp_date(uint8_t day, uint8_t month, uint8_t year);
uint8_t check_card_date(uint8_t day, uint8_t month, uint8_t year);

uint16_t SwapEndian16(uint16_t val);
uint32_t SwapEndian32(uint32_t val);

uint16_t ABS(int16_t value);
uint32_t max(uint32_t* buff,uint8_t size);
int32_t max2(int32_t x,int32_t y);
void Capital_Small_Str_Convertion(uint8_t* buffer, uint8_t buff_size);

uint8_t DecimalToBCD (uint8_t decimalByte);
uint8_t BCDToDecimal (uint8_t bcdByte);


void SetBootLoaderKey(void);
void swap(uint32_t *a,uint32_t *b);
uint8_t Get_TotalNumberOfDays(uint8_t month, uint8_t year);
void int_to_hexString(uint8_t* res , uint16_t x);
void Delay_ms(volatile uint32_t n);
void Delay_us(volatile uint32_t n);
void delay_cycles(volatile uint32_t cycles);
void BS_generate_key(uint8* key);
/*!
 * @par Description:
 * This function used to transfer block of bytes based on endianness of elements inside source data and swape the endiannes.
 *
xxxx xxxx
0xxx xxxx ---represnet 1 or multiple of bytes
100x xxxx ---represnet uint16_t
101xx xxxx ---represnet uint16_t[]
110x xxxx ---represnet uint32_t
111xx xxxx ---represnet uint32_t[]
 * @param[in] ptr_dest  pointer to destination
 *  @param[in] ptr_src  pointer to source
 *  @param[in] ptr_src  pointer to array of elements'size of buffer source
 *  @param[in] size_dest  length of buffer source
 * @return
 * None.
 */
void big_endian_memcpy(uint8_t* ptr_dest, uint8_t* ptr_src,uint8_t* ptr_size_ary,uint16_t size_dest);//for _16,_16[],_32,_32[]

uint8_t BS_valid_current_date_time(uint8_t mode);

void BS_check_rtc_senq_date_time(void);
uint8_t check_quadrant(uint8_t curr_month,uint8_t prv_month);
G2_date_time_t unix_to_date_time(uint32_t unix_timestamp);
uint32_t date_time_to_unix(G2_date_time_t dt) ;
/*------------------------------------------------------------------------------
                        End function prototypes
 -----------------------------------------------------------------------------*/






#endif  /*end 0f if BASIC_SRV_H_ defined */
/*******************************************************************************
                             End of file
*******************************************************************************/
