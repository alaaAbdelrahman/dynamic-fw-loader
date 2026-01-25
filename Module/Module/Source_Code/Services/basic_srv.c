
/*******************************************************************************
**                                                                            **
**  PROJECT   :                                                               **
**                                                                            **
**  File Name : Basic_Srv.c                                                   **
**                                                                            **
**  File Type : C-file for Delays.                                            **
**                                                                            **
**  AUTHOR    : Mohamed Mahmoud Zakaria                                       **
**                                                                            **
**  PURPOSE   : Provide basic services.                                       **
**                                                                            **
**  H/W       : Micro Controller: STM8L16x                                    **
**                                                                            **
**  S/W       : IAR Systems IDE & Compiler                                    **
**                                                                            **
**  History   : May 30, 2011                                                  **
**                                                                            **
**  Version   : 1.0                                                           **
*******************************************************************************/





/********************************** Headers ***********************************/
#include "basic_srv.h"/*header*/
#include "RTC_Interface.h"
/******************************************************************************/

/***************************** Data Types *************************************/
typedef union transfer32
{
  uint32_t Total;
  struct
  {
    uint8_t byte3 ;
    uint8_t byte2 ;
    uint8_t byte1 ;
    uint8_t byte0 ;
    
  } byteNo;
} unTransfer32;

typedef union transfer16
{
  uint16_t Total;
  struct
  {
    uint8_t byte1 ;
    uint8_t byte0 ;
    
  } byteNo;
} unTransfer16;
/***************************** Data Types *************************************/
/*!
* @par Description:
*   This structure contains all month key values .\n
*/
static const uint8_t month_key[12] = {3, 6, 6, 2, 4, 0, 2, 5, 1, 3, 6, 1};
static const uint16_t year_days[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
uint8_t bs_year , bs_month ,bs_day , bs_hour, bs_min ,bs_sec;
/***************************** Defines ****************************************/
#define LPM_Disp_Time  30
#define BSC_SRV_IS_PWR_DWN() CTRL_PWR_DN_STAT /*!< Power down flag.*/
#define CENTURY_VALUE 4
/***************************** Defines ****************************************/

/******************************************************************************/
/*******************************************************************************
Global Variables
*******************************************************************************/

uint8_t Disp_Count = 0;
uint8_t screen_sw_up;
/*------------------------------------------------------------------------------
End Global Variables
------------------------------------------------------------------------------*/



/*******************************************************************************
******************
**************************       Functions      ****************************
******************
*******************************************************************************/
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
uint8_t calc_crc(uint8_t* data_ptr, uint16_t size,uint8_t calc_type)
{
  /*!@par Pseudo Code:*/
  uint8_t remainder=0;
  switch(calc_type) 
  {  
  case checksum:
    {
      uint16_t i;
      for(i = 0 ; i < size; i++)
      {
        remainder ^= data_ptr[i];
      }
      break;
    }
  case CRC:
    {
      uint16_t byte=0;
      uint8_t bit = 8;
      /*
      * Perform modulo-2 division, a byte at a time.
      */
      for ( byte = 0; byte < size; byte++)
      {
        /*
        * Bring the next byte into the remainder.
        */
        remainder ^= (data_ptr[byte]<< (WIDTH - 8));
        
        /*
        * Perform modulo-2 division, a bit at a time.
        */
        for (bit = 8; bit > 0; bit--)
        {
          /*
          * Try to divide the current data bit.
          */
          if (remainder & TOPBIT)
          {
            remainder = (remainder << 1) ^ POLYNOMIAL;
          }
          else
          {
            remainder = (remainder << 1);
          }
        }
      }  
      
      break;  
    }
    
  }  
  if (remainder == 0)
  {
    if (BS_check_all_zeros (data_ptr, size))
      return 0x55;
  }
  return remainder; 
}

/*!
* @brief Return the CRC of system variables structure
*
* @par Description:
* Return the CRC of system variables structure
*
* @return uint8_t CRC of system variables structure\n
*
* @note CRC is the XOR of all memory bytes of configuration structure.\n
*/
////uint8_t calc_crc_only(uint8_t* data_ptr, uint16_t size)
//{
//  /*!@par Pseudo Code:*/
//  uint8_t crc = 0;
//  uint16_t i;
//  /*! - XOR byte in the system variables structure*/
//  for(i = 0; i < size; i++)
//    crc ^= data_ptr[i];
//  /*! - Return CRC*/
//  return  crc;
//}

/*------------------------------------------------------------------------------
END Function vGOTO_PWRdown
------------------------------------------------------------------------------*/

uint32_t max(uint32_t* buff,uint8_t size)
{
  uint8_t i = 1;
  uint32_t m = buff[0];
  while (i < size)
  {
    (m < buff[i]) && (m = buff[i]); //these are not conditional statements.
    i++;
  }
  return m;
}


int32_t max2(int32_t x,int32_t y)
{
  if (x>y)
    return x;
  return y;
}

/*******************************************************************************
Function Name   :  vDelay_SW

Parameters(in)  :  u32LoopTimes

Parameters(out) :  None

Return value    :  None

Sych/Asynch     :  Synchronous

Reentrancy      :  Reentrant

Description     :  used to software delay depending number of ticks of SCLK.
                  1----->0.2303 us
*******************************************************************************/
#pragma optimize=z none no_code_motion no_cse
void vDelay_SW(uint32_t u32LoopTimes)
{
 // if ((uint32_t)CLK_GetHCLKFreq() < (uint32_t)14*1000*1000)
   // u32LoopTimes = u32LoopTimes/2 +1;
  /* Decrement nCount value */
  while(u32LoopTimes != (uint32_t)0)
  {
    u32LoopTimes--;
  }
}/*end function*/

/*------------------------------------------------------------------------------
END Function vDelay_SW
------------------------------------------------------------------------------*/

/*******************************************************************************
Function Name   :  vDelay_HW

Parameters(in)  :  u8SecCount -> not more than 128

Parameters(out) :  None

Return value    :  None

Sych/Asynch     :  Synchronous

Reentrancy      :  Reentrant

Description     :  used to Hardware delay depending Timer3
*******************************************************************************/
#pragma optimize=z none no_code_motion no_cse
void vDelay_HW(uint8_t u8SecCount)
{
  uint16_t secsnow;
  uint32_t timeout = 1000000;
  secsnow = S_RTC_C_GetRTCSEC();
  while((ABS(S_RTC_C_GetRTCSEC() - secsnow) < u8SecCount)&&(--timeout!=0));  //Todo : limit while loop
}/*end function*/
/*------------------------------------------------------------------------------
END Function vDelay_HW
------------------------------------------------------------------------------*/
/*******************************************************************************
Function Name   :  u8Int_ToDecimal

Parameters(in)  :  u32IntValue
u8ptrbuf

Parameters(out) :  None

Return value    :  uint8_t  u8DigitsNumber: number of digits

Sych/Asynch     :  Synchronous

Reentrancy      :  None Reentrant

Description     :  This function is responsible of starting .
*******************************************************************************/

uint8_t u8Int_ToDecimal(uint8_t* u8ptrbuf, uint32_t u32IntValue)
{
  /*---Local Variable---*/
  uint8_t u8DigitsNumber = 0;
  uint8_t Index = 8;
  /*----***---*/
  /*to avoid LCD Overflow digits*/
  u32IntValue %= 100000000;
  do  /* generate digits in reverse order */
  {
    u8ptrbuf[--Index] = u32IntValue % 10; /* get next digit */
    u8ptrbuf[Index] += 0x30;
  }
  while((u32IntValue /= 10) > 0);        /* delete it */
  u8DigitsNumber = 8 - Index;
  return  u8DigitsNumber;
}/*end function*/

/*!
* @brief Convert an integer to ASCII.
*
* @par Description:
* Convert an integer to ASCII.
*
* @param [in] u8ptrbuf address of buffer to get ascii string in
* @param [in] num_digits number of ascii digits
* @param [out] u8int number of digits
*/
uint8_t itoa_generic (uint8_t* u8ptrbuf, uint32_t u32int_value)
{
  uint8_t index = 0,num_digits=0;
  uint32 tempVariable = u32int_value;
  if (u32int_value == 0)
  {
    num_digits = 1;
  }
  else
  {
    for(num_digits = 0; tempVariable > 0; num_digits++)
    {
      tempVariable /= 10;
    }
  }
  //u8ptrbuf[num_digits]=0;
  for(index = 1; index <= num_digits; index++)
  {
    u8ptrbuf[num_digits - index] = (u32int_value % 10) + 0x30;
    u32int_value /= 10;
  }
  return num_digits;
}
uint8_t atoi_generic (uint8_t* u8ptrbuf, uint32_t* u32int_ptr , uint8_t len)
{
  uint8_t num_digits=0;
  uint32 multiplier = 1;
  uint32_t value = 0;
  if (u32int_ptr == NULL || len == 0 || len>10 || u8ptrbuf == NULL)
  {
    return false;
  }
  for(num_digits = 0; num_digits < len; num_digits++)
  {
    value += (u8ptrbuf[len - num_digits -1] - '0') * (multiplier);
    multiplier *= 10;
  }
  *u32int_ptr = value;
  return true;
}
/*!
* @brief Convert an integer to ASCII.
*
* @par Description:
* Convert an integer to ASCII.
*
* @param [in] u8ptrbuf address of buffer to get ascii string in
* @param [in] num_digits number of ascii digits
* @param [in] u32int_value Integer value to be converted
*/
void int_to_ascii(uint8_t* u8ptrbuf, uint32_t u32int_value, uint8_t num_digits)
{
  uint8_t index;
  for(index = 1; index <= num_digits; index++)
  {
    u8ptrbuf[num_digits - index] = (u32int_value % 10) + 0x30;
    u32int_value /= 10;
  }
}/*end function*/


void vDelay_SW_RFID(uint32_t u32LoopTimes)
{
#if 0
  /* Decrement nCount value */
  while(u32LoopTimes != (uint32)0)
  {
    if (ctrl_flags & CTRL_PWR_DWN_MASK)
      break;
    u32LoopTimes--;
  }
#endif
}/*end function*/

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
int32_t comp_date(uint8_t u8day, uint8_t u8month, uint8_t u8year)
{
  return ((int32_t)((((CTRL_READ_YEAR() * (uint32_t)12) + CTRL_READ_MONTH()) * (uint32_t)31) + CTRL_READ_DAY()) - (int32_t)((((u8year * (uint32_t)12) + u8month) * (uint32_t)31) + u8day));
}
int32_t comp_time(uint8_t u8second, uint8_t u8minute, uint8_t u8hour)
{
  return ((int32_t)((((CTRL_READ_HR() * (int32_t)3600) + CTRL_READ_MIN()) * (int32_t)60) + CTRL_READ_SEC()) - (int32_t)((((u8hour * (int32_t)3600) + u8minute) * (int32_t)60) + u8second));
}
void int_to_hexString(uint8_t* res , uint16_t x)
{
  uint8_t temp = 0;
  // = TO_HEX();
  
  temp = (x & 0xF000) >> 12;
  if (temp <= 9)
    res[0] = '0' + temp;
  else 
    res[0] = 'A' - 10 + temp;
  
  temp = (x & 0x0F00) >> 8;
  if (temp <= 9)
    res[1] = '0' + temp;
  else 
    res[1] = 'A' - 10 + temp;
  
  temp = (x & 0x00F0) >> 4;
  if (temp <= 9)
    res[2] = '0' + temp;
  else 
    res[2] = 'A' - 10 + temp;
  
  temp = x & 0x000F;
  if (temp <= 9)
    res[3] = '0' + temp;
  else 
    res[3] = 'A' - 10 + temp;
  
  res[4] = '\0';
  
}                      
/*!
* @brief Convert ASCII to integer.
*
* @par Description:
* Convert ASCII to integer.
*
* @param [in] data_ptr address of ascii string
* @param [in] digit_num number of ascii digits
*
* @return
* - 32 bits Integer value.
*
* @note If the digit numbers = 0, the function will return TRUE.
*/
uint32_t ascii_to_int(uint8_t *data_ptr, uint8_t digit_num)
{
  uint32_t result = 0;
  uint8_t index;
  if(digit_num == 0)
    return TRUE;
  for(index = 0; index < digit_num; index++)
    result = (result * 10) + data_ptr[index] - 0x30;
  return result;
}
/*------------------------------------------------------------------------------
END Function u8Int_ToDecimal
------------------------------------------------------------------------------*/





/*=======================================================================================
FUNCTION Load_32BitData

DESCRIPTION:
This Function is used to concatenate 4 bytes together from 4 byte array

Inputs:
Source :pointer to start of 4 bytes to be concatenated

Return Value:
DEST: pointer to 32-bit concatenated result.
=======================================================================================*/
void Load_32BitData(uint32_t *DEST, uint8_t *Source)
{
  /*char *ee=(char *)(DEST);
  *(ee+0)=*(Source+0);//lowest
  *(ee+1)=*(Source+1);
  *(ee+2)=*(Source+2);
  *(ee+3)=*(Source+3);//highest*/
  uint32_t cpyTemp = 0;
  cpyTemp = Source[3];
  cpyTemp = (cpyTemp << 8) + Source[2];
  cpyTemp = (cpyTemp << 8) + Source[1];
  cpyTemp = (cpyTemp << 8) + Source[0];
  *DEST = cpyTemp;
}
/*=======================================================================================
FUNCTION Store_32BitData

DESCRIPTION:
This Function is not used now

Inputs:


Return Value:

=======================================================================================*/
void Store_32BitData(uint8_t *DEST, uint32_t*Source)
{
  char *ee = (char *)(Source);
  *(DEST + 0) = *(ee + 0);
  *(DEST + 1) = *(ee + 1);
  *(DEST + 2) = *(ee + 2);
  *(DEST + 3) = *(ee + 3);
}

void Store_16BitData(uint8_t *DEST, uint16_t* Source)
{
  char *ee = (char *)(Source);
  *(DEST + 0) = *(ee + 0);
  *(DEST + 1) = *(ee + 1);
  
}



/*=======================================================================================
FUNCTION ResetArr

DESCRIPTION:
This function loads zeros to any array the Start index to the End index.
Inputs:
uint8_t *u8ptrbuf  : Start the Array.
uint8_t u8Length : Number of bytes of the array to reset.
Return Value:
None.
=======================================================================================*/

void vReset_Buffer(uint8_t *u8ptrbuf, uint8_t u8Length)
{
  uint8_t u8Index;
  for(u8Index = 0; u8Index < u8Length; u8Index++)
    *(u8ptrbuf + u8Index) = 0;
}



/*************************************************
Function:       vCopy_u8Buf
Description:
Copies from the pointer "Src" to the pointer
"Dest" NCount fields.
Parameter:
unsigned char *u8ptrDest      : Destination Pointer
unsigned char *u8ptrSrc       : Source Pointer
unsigned char u8Lenght     : Number of counts
Return:
short      status of implement
**************************************************/
void vCopy_u8Buf(uint8_t *u8ptrDest, uint8_t *u8ptrSrc, uint8_t u8Lenght)
{
  uint8_t u8Index;
  for(u8Index = 0; u8Index < u8Lenght; u8Index++)
    u8ptrDest[u8Index] = u8ptrSrc[u8Index];
}

/*=======================================================================================
FUNCTION u8ComperBufs.

DESCRIPTION:
This function is used to check that the two input arrays have the same values
for certain length.
Inputs:
1- Pointer for array number 1 called SRC.
2- Pointer for array number 2 called DES.
3- Number of bytes to check called length
Return Value:
True if two arrays have the same values.
False if two arrays have any different value.
=======================================================================================*/
uint8_t u8Comper_u8Bufs(uint8_t *u8ptrSRC, uint8_t *u8ptrDES, uint8_t u8Length)
{
  uint8_t t;
  uint8_t ret = TRUE;
  for(t = 0 ; t < u8Length ; t++)
    if(*(u8ptrSRC + t) != *(u8ptrDES + t))
    {
      ret = FALSE;
      break;
    }
  return ret;
}

/*******************************************************************************************************************/
/*!
* @brief This function used
*
* @par Description:
* Used to
*
* @return
* - None.
*/
/*******************************************************************************************************************/
uint32_t SwapEndian32(uint32_t val)
{
  uint32_t data;
  data = (val << 24) | ((val << 8) & 0x00ff0000) |
    ((val >> 8) & 0x0000ff00) | (val >> 24);
  return data;
}
/*******************************************************************************************************************/
/*!
* @brief This function used to
*
* @par Description:
* Used to
*
* @return
* - None.
*/
/*******************************************************************************************************************/
uint16_t SwapEndian16(uint16_t val)
{
  uint16_t data;
  data = (val << 8) | (val >> 8);
  return data;
}
/*************************************************
Function:       bser_calc_day_of_week
Description:
Calculates the day of the week (Saterday , sunday , .... ) from the date.
pay attention that this function is used for 20th century only.
Parameter:
day   : uint8_t which represents the day of the date.
month : uint8_t which represents the month of the date
year  : uint8_t which represents the year of the date
Return: uint8_t which represents the day of the week.
**************************************************/
uint8_t bser_calc_day_of_week(uint8_t day, uint8_t month, uint8_t year)
{
  uint8_t result;
  result = (year + (year / 4) + CENTURY_VALUE + month_key[month - 1] + day) % 7;
  if(result == 0)
    result = 6;
  else
    result--;
  return result;
}

/*************************************************
Function:       bser_calc_day_of_year
Description:
Calculates the day of the year from the given year.
Parameter:
day   : uint8_t which represents the day of the date.(1-->31)
month : uint8_t which represents the month of the date (1-->12)
year  : uint8_t which represents the year of the date.
Return: uint8_t which represents the day of the year.
**************************************************/
uint16_t bser_calc_day_of_year(uint8_t day, uint8_t month, uint8_t year)
{
  uint16_t result=0;
  
  result = year_days[month-1] + day;
  if ((((year+2000)%4) == 0 )&& !(((year+2000)%100 == 0) && (year+2000)%400 != 0))
    result++;
  
  return result;
}

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
#if 0
int32_t comp_date(uint8_t day, uint8_t month, uint8_t year)
{
  return ((int32_t)((((CTRL_READ_YEAR() * (uint32_t)12) + CTRL_READ_MONTH()) * (uint32_t)31) + CTRL_READ_DAY()) - (int32_t)((((year * (uint32_t)12) + month) * (uint32_t)31) + day));
}
#endif



/*!
* @brief
*
* @par Description:
*
* @return
*
*/
uint32_t convertToLong32(uint8_t* a, uint8_t endian)
{
  uint32_t x;
  if(endian == BS_LITTLE_ENDIAN_TYPE)
    x = ((uint32_t)(a[0]) & 0xFF | ((uint32_t)a[1] << 8) & 0xFF00 | ((uint32_t)a[2] << 16) & 0xFF0000 | ((uint32_t)a[3] << 24) & 0xFF000000);
  else if(endian == BS_BIG_ENDIAN_TYPE)
    x = ((uint32_t)(a[3]) & 0xFF | ((uint32_t)a[2] << 8) & 0xFF00 | ((uint32_t)a[1] << 16) & 0xFF0000 | ((uint32_t)a[0] << 24) & 0xFF000000);
  return x;
}

/*!
* @brief
*
* @par Description:
*
* @return
*
*/
uint16_t convertToInt16(uint8_t* a, uint8_t endian)
{
  uint16_t x;
  if(endian == BS_LITTLE_ENDIAN_TYPE)
    x = ((uint16_t)(a[0]) & 0xFF | ((uint16_t)a[1] << 8) & 0xFF00);
  else if(endian == BS_BIG_ENDIAN_TYPE)
    x = ((uint16_t)(a[1]) & 0xFF | ((uint16_t)a[0] << 8) & 0xFF00);
  return x;
}



/*!
* @brief
*
* @par Description:
*
* param [in]
*
* @return
*
*/
uint16_t ABS(int16_t value)
{
  if(value >= 0)
    return value;
  else
    return -(value);
}

#if 0
//=============================================================================
// FUNCTION
//=============================================================================
// Name           : SetBootLoaderKey
// Description    : Set the Boot loader activation key if the given value
//                  equal the expected value
//-----------------------------------------------------------------------------
// Author         : HB
// Date           : 08/07/2012
//=============================================================================
void SetBootLoaderKey(void)
{    
  
  // store meter data
  
  // clear the boot loader deactivation flag
  FCTL3 = FWKEY;				// Clear Lock bit
  FCTL1 = FWKEY + ERASE;			// Set Erase bit
  __data20_write_char( 0x1900, 0 ); 		// Dummy write to erase Flash seg
  FCTL3 = FWKEY + LOCK;				// Set LOCK bit 
  // Activate the boot loader
  ((void (*)(void))0x1000)();                   // jump directly to BSL
}
#endif
uint8_t BS_check_valid_date (uint8_t day , uint8_t month , uint8_t year)
{
  uint8_t daysinmonth[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
  if (day == 0 || month == 0 || month > 12 )
    return FALSE;
  if ((((year+2000)%4) == 0 )&& !(((year+2000)%100 == 0) && (year+2000)%400 != 0))
    daysinmonth[1]=29;
  
  if( day <= daysinmonth[month-1])
    return TRUE;
  return FALSE;
} 

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : swap
// Description    : swap 2 variable from type uint32_t
//                  equal the expected value
//-----------------------------------------------------------------------------
// Author         : SR
// Date           : 27/05/2015
//=============================================================================

void swap(uint32_t *a,uint32_t *b)
{
  uint32_t temp;
  temp=*a;
  *a=*b;
  *b=temp;
}
//=============================================================================
// FUNCTION
//=============================================================================
// Name           : LeapYear
// Description    : return value > 0 - leap year
//                               = 0 - no leap year
//-----------------------------------------------------------------------------
// Author         : JM
// Date           : 28/04/2011
//=============================================================================
uint8_t LeapYear(uint8_t year)
{
  return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : Get_TotalNumberOfDays
// Description    : Get total number of days in month
//-----------------------------------------------------------------------------
// Author         : JM
// Date           : 28/04/2011
//=============================================================================
uint8_t Get_TotalNumberOfDays(uint8_t month, uint8_t year)
{
  if(month == 2)
  {
    if(LeapYear(year)) return(29);
    else return (28);
  }
  if((month >= 1) && (month <= 7))
  {
    if(month % 2 == 0) return(30);
    else return (31);
  }
  else
  {
    if(month % 2 == 0) return(31);
    else return (30);
  }
}

/**
* @brief  Delay N us SystemClock @ 26214400.
* @param  None
* @retval None
*/
#pragma optimize=z none no_code_motion no_cse
void Delay_us(volatile uint32_t n)
{
// n=  2*n;//(((uint32_t)CLK_GetHCLKFreq()/(uint32_t)13107200))*n;
  while (n--)
  {
    __NOP();
    __NOP();__NOP();
  
#if (MicroController == Micro_V94XX)    //for using 26MHZ
                             
    __NOP();
    __NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();
#endif
  }
}

/*!
* @brief generate the encryption key from the input random key.
*
* @par Description:
* This function generate the encryption key from the input random key.
*
* @param[in] key: represent the input random key as input/ and the generated encryption key as output.
*
* @return None.
*/
/*******************************************************************************************************************/
void BS_generate_key(uint8* key)
{
  uint8 inputkey[ENCRYPTED_KEY_SIZE];
  memcpy(inputkey, key, ENCRYPTED_KEY_SIZE);
  key[0]  = inputkey[2]  ^ inputkey[6];
  key[1]  = inputkey[6]  ^ inputkey[3];
  key[2]  = inputkey[3]  ^ inputkey[14];
  key[3]  = inputkey[14] ^ inputkey[11];
  key[4]  = inputkey[11] ^ inputkey[0];
  key[5]  = inputkey[0]  ^ inputkey[8];
  key[6]  = inputkey[8]  ^ inputkey[12];
  key[7]  = inputkey[12] ^ inputkey[1];
  key[8]  = inputkey[1]  ^ inputkey[9];
  key[9]  = inputkey[9]  ^ inputkey[15];
  key[10] = inputkey[15] ^ inputkey[5];
  key[11] = inputkey[5]  ^ inputkey[13];
  key[12] = inputkey[13] ^ inputkey[4];
  key[13] = inputkey[4]  ^ inputkey[7];
  key[14] = inputkey[7]  ^ inputkey[10];
  key[15] = inputkey[10] ^ inputkey[2];
}
uint8_t BS_get_curr_month_days (uint8_t month , uint8_t year)
{
  uint8_t daysinmonth[12]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  
  if ((((year+2000)%4) == 0 )&& !(((year+2000)%100 == 0) && (year+2000)%400 != 0))
    daysinmonth[1]=29;
  
  return (daysinmonth[month-1]);
} 
uint8_t BS_check_all_zeros (uint8_t* buff , uint16_t len)
{
  uint16_t i = 0;
  for (i=0;i<len;i++)
  {
    if (buff[i] != 0)
      return FALSE;
  }
  // Set_PIN(BUZZER_PORT_OUT, BUZZER_PIN);
  return TRUE;
} 
uint8_t BS_check_all_with_Value (uint8_t* buff ,uint8_t value_compared,  uint16_t len)
{
  uint16_t i = 0;
  for (i=0;i<len;i++)
  {
    if (buff[i] != value_compared)
      return FALSE;
  }
  return TRUE;
} 
/**
* @brief  Delay N ms SystemClock @ 26214400.
* @param  None
* @retval None
*/
#pragma optimize=z none no_code_motion no_cse
void Delay_ms(volatile uint32_t n)
{
  while (n--)
  {
    Delay_us(1000);
  }
}

void delay_cycles(uint32_t cycles)
{
  uint32_t i;
  if ((uint32_t)CLK_GetHCLKFreq() < (uint32_t)14*1000*1000)
    cycles=cycles/2;
  for(i = 0; i < cycles; i++)
  {
    
  }
}


//=============================================================================
// FUNCTION
//=============================================================================
// Name           : Capital_Small_Str_Convertion
// Description    : Convert letter from captial to small suitable for display
//-----------------------------------------------------------------------------
// Author         : A.Kamel
// Date           : 12/03/2018
//=============================================================================
void Capital_Small_Str_Convertion(uint8_t* buffer, uint8_t buff_size)
{
  uint8_t char_index ;
  
  for (char_index = 0 ; char_index < buff_size ; char_index++)
  {
    if ((buffer[char_index] > 64) && (buffer[char_index] < 91))
      
      buffer[char_index] =  buffer[char_index] + 32 ;
    
  }
  
}


uint8_t BCDToDecimal (uint8_t bcdByte)
{
  return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}



uint8_t DecimalToBCD (uint8_t decimalByte)
{
  return (((decimalByte / 10) << 4) | (decimalByte % 10));
}
/*!
 * @par Description:
 * This function used to transfer block of bytes based on endianness of elements inside source data and swape the endiannes.
 *
xxxx xxxx
0xxx xxxx ---represnet 1 or multiple of bytes
100x xxxx ---represnet uint16_t
101xx xxxx ---represnet uint16_t[]
110x xxxx ---represnet uint32_t
111x xxxx ---represnet uint32_t[]
 * @param[in] ptr_dest  pointer to destination
 *  @param[in] ptr_src  pointer to source
 *  @param[in] ptr_src  pointer to array of elements'size of buffer source
 *  @param[in] size_dest  length of buffer source
 * @return
 * None.
 */
void big_endian_memcpy(uint8_t* ptr_dest, uint8_t* ptr_src,uint8_t* ptr_size_ary,uint16_t size_dest)//for _16,_16[],_32,_32[]
{
  uint16_t index=0,index_size_ary=0;
  uint8_t size_loop =0,ii=0;
  uint8_t ar_temp[4];
  //  ENDIANNESS uint32_t xb;
  //  for(index=0;index<size_dest;index+=*((uint8_t*)ptr_size_ary+index1),index1++)
  while(index<size_dest)
  {
    //16 bits or 32 bits
    if(*((uint8_t*)ptr_size_ary+index_size_ary)&0x80)
    {
      size_loop=*((uint8_t*)ptr_size_ary+index_size_ary)&0x1f;
      ii=0;
      if(((*((uint8_t*)ptr_size_ary+index_size_ary)&0xE0)==0xA0)||((*((uint8_t*)ptr_size_ary+index_size_ary)&0xE0)==0x80))//array_16 or _16
      {
        do
        {
          //16 bits or array of 16 bits
          memcpy(ar_temp, (uint8_t*)(ptr_src+index),2);
          //  SwapEndian16(ptr_dest);
          *(ptr_dest+index)   = ar_temp[1];
          *(ptr_dest+index+1) = ar_temp[0];
          index+=2;
          ii++;
        }
        while(ii<size_loop);
      }
      else
      {
          do
          {
            //32 bits or array of 32 bits
            memcpy(ar_temp, (uint8_t*)(ptr_src+index),4);
            //   SwapEndian32(ptr_dest);
            *(ptr_dest+index)   =ar_temp[3];
            *(ptr_dest+index+1) =ar_temp[2];
            *(ptr_dest+index+2) =ar_temp[1];
            *(ptr_dest+index+3) =ar_temp[0];
            index+=4;
            ii++;
          }while(ii<size_loop);
      }
    }
    else
    {
      // one byte or array of bytes
      memcpy((uint8_t*)(ptr_dest+index) ,(uint8_t*)(ptr_src+index),*((uint8_t*)ptr_size_ary+index_size_ary));
      index+=*((uint8_t*)ptr_size_ary+index_size_ary);
    }
    index_size_ary++;
  }
}

uint8_t BS_valid_current_date_time(uint8_t mode)
{
  int32_t result = 0;
 
  if (mode == 0)
  {
    bs_year = S_RTC_C_GetRTCYEAR();
    bs_month = S_RTC_C_GetRTCMON();
    bs_day = S_RTC_C_GetRTCDAY();
    bs_hour = S_RTC_C_GetRTCHOUR();
    bs_min = S_RTC_C_GetRTCMIN();
    bs_sec = S_RTC_C_GetRTCSEC();
  }
  else
  {
    bs_year = CTRL_READ_YEAR();
    bs_month = CTRL_READ_MONTH();
    bs_day = CTRL_READ_DAY();
    bs_hour = CTRL_READ_HR();
    bs_min = CTRL_READ_MIN();
    bs_sec = CTRL_READ_SEC();
  }
    
  if (bs_hour < 24 && bs_min < 60 && bs_sec < 60)
  {
    result = BS_check_valid_date (bs_day, bs_month , bs_year);
  } 
  return result;
}

//void BS_check_rtc_senq_date_time(void)
//{
//  int32_t result = 0;
//  uint8_t tmp_cfg = 0;
//    if (BS_valid_current_date_time(1) == 1)
//    {
//      if (BS_valid_current_date_time(0) == 1)
//      {
//        result = comp_date( bs_day, bs_month, bs_year);
//        if (result <= 0 && result > -120 )
//        {
//        //// RTC UPtODATE
//          tmp_cfg = 0;
//        }
//        else
//        {  
//          tmp_cfg = 1;
//        }
//      }
//      else
//      {
//        tmp_cfg = 1;
//      }
//    }
//    else
//    {
//      CTRL_READ_HR() = 0;
//      CTRL_READ_MIN() = 0;
//      CTRL_READ_SEC() = 0;
//      CTRL_READ_DAY() = 1;
//      CTRL_READ_MONTH() = 1;
//      CTRL_READ_YEAR() = 13;
//     CTRL_READ_DAYOW() = 2;
//     /* - Set RTC with default date and time.*/
//     tmp_cfg = 1;
//    }  
//    if (tmp_cfg)
//    {
//      ctrl_cfg_set(CTRL_CFG_TIME, (uint8_t*)&ctrl_sys_var_optional_feature.rtc_time);//ismail it will get the same value?!
//      ctrl_cfg_set(CTRL_CFG_DATE, (uint8_t*)&ctrl_sys_var_optional_feature.rtc_date);
//    }
//}
uint8_t check_quadrant(uint8_t curr_month,uint8_t prv_month)
{
  if((prv_month>=7 && prv_month<=9)&&(curr_month>=7 && curr_month<=9))
  {
    return 1; //quadrant 1
  }
  else if((prv_month>=10 && prv_month<=12)&&(curr_month>=10 && curr_month<=12))
  {
    return 2; //quadrant 2
  }
  else if((prv_month>=1 && prv_month<=3)&&(curr_month>=1 && curr_month<=3))
  {
    return 3; //quadrant 3
  }
  else if((prv_month>=4 && prv_month<=6)&&(curr_month>=4 &&curr_month<=6))
  {
    return 4; //quadrant 4
  }
  else
  {
    return 0;
  }
}





G2_date_time_t unix_to_date_time(uint32_t unix_timestamp) 
{
    G2_date_time_t dt;

    // Calculate seconds, minutes, and hours
    dt.sec = unix_timestamp % SECONDS_IN_A_MINUTE;
    unix_timestamp /= SECONDS_IN_A_MINUTE;
    dt.min = unix_timestamp % SECONDS_IN_A_MINUTE;
    unix_timestamp /= SECONDS_IN_A_MINUTE;
    dt.hr = unix_timestamp % 24;
    unix_timestamp /= 24;

    // Calculate the year
    dt.year = 1970;
    while (unix_timestamp >= (LeapYear(dt.year) ? DAYS_IN_A_LEAP_YEAR : DAYS_IN_A_YEAR)) {
        unix_timestamp -= LeapYear(dt.year) ? DAYS_IN_A_LEAP_YEAR : DAYS_IN_A_YEAR;
        dt.year++;
    }

    // Calculate the month and day
    dt.month = 0;
    while (unix_timestamp >= Get_TotalNumberOfDays(dt.month, dt.year)) {
        unix_timestamp -= Get_TotalNumberOfDays(dt.month, dt.year);
        dt.month++;
    }
    dt.day = unix_timestamp + 1; // Day of month starts from 1

    return dt;
}
uint32_t date_time_to_unix(G2_date_time_t dt) 
{
    uint32_t unix_time = 0;
    uint16_t year=0;

    // Calculate the number of seconds from 1970 to the year before the given year
    for (year = 1970; year < dt.year; year++) {
        unix_time += LeapYear(year) ? 366 * SECONDS_IN_A_DAY : 365 * SECONDS_IN_A_DAY;
    }

    // Calculate the number of seconds from January to the month before the given month
    for (uint16_t month = 1; month < dt.month; month++) {
        unix_time += Get_TotalNumberOfDays(month, dt.year) * SECONDS_IN_A_DAY;
    }

    // Add the number of seconds for the days before the given day
    unix_time += (dt.day - 1) * SECONDS_IN_A_DAY;

    // Add the number of seconds for the given hours, minutes, and seconds
    unix_time += dt.hr * SECONDS_IN_AN_HOUR;
    unix_time += dt.min * SECONDS_IN_A_MINUTE;
    unix_time += dt.sec;

    return unix_time;
}

/*******************************************************************************
End of file
*******************************************************************************/
