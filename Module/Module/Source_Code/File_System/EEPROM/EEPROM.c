//=============================================================================
//
//                       FILE NAME : EEPROM_AT24C1024.c
//
//                       MODULE    : H/W Drivers
//
//                       PROJECT   : 3P_Meter
//
//=============================================================================
//
//    This is a SEWEDY's document. Reproduction and Disclosure are forbidden
//
//=============================================================================

//=============================================================================
//                 IMPORT
//=============================================================================
#include "EEPROM.h"
//#include "ctrl.h"
#include "core_cm0.h"
#include "intrinsics.h"
#include <string.h>
//#include "display.h"
#include "file_sys.h"
#include "config.h"
#include "basic_srv.h"
#include "I2C_Interface.h"
#include "GPIO_Interface.h"
#include "UART_Interface.h"
//#include "Dot_Matrix_Interfacing.h"
//=============================================================================
//              DEFINITIONS
//=============================================================================

#if (MicroController == Micro_V94XX)
#define EEPROM_AT24C256_I2C_ADDRESS   0xa4    //0x50
#define EEPROM_M24M02_I2C_ADDRESS     0xa4    //0x54
#elif (defined(EM110_BOARD) || defined(EM130_BOARD) || defined(EM210_BOARD) || defined(EM122U_BOARD)|| defined(EM330_BOARD))
#define EEPROM_AT24C256_I2C_ADDRESS   0xa4    //0x50
#define EEPROM_M24M02_I2C_ADDRESS     0xa4    //0x54
#else
#define EEPROM_AT24C256_I2C_ADDRESS   0xa0    //0x50
#define EEPROM_M24M02_I2C_ADDRESS     0xa0    //0x54
#endif

#ifndef EEPROM_PAGE_256
#define EEPROM_PAGE_SIZE_DRV    64L
#else
#define EEPROM_PAGE_SIZE_DRV    256L
#endif

#define EEPROM_WRITE(slave_add , Address,TxData,numBytes)  S_I2C_Write( slave_add , Address, TxData, numBytes, EEPROM_PAGE_SIZE_DRV,S_EEPROM_I2C )
#define EEPROM_READ(slave_add , Address,RxData,numBytes)   S_I2C_Read( slave_add , Address, RxData, numBytes, EEPROM_PAGE_SIZE_DRV,S_EEPROM_I2C )
#define EEPROM_PUBLISH_EVENT(event)                         ctrl_event(event)   



#define INDICATE_EEPROM_FAIL()   ctrl_event(CTRL_DATA_CRPT_FILE_SYS)
#define MAX_EEPROM_DRIV_INIT    10
#define CLR_EEPROM_FAIL_FLAG()     ctrl_sys_var.basic.meter_status[0] &= (~(1 << (CTRL_DATA_CRPT_FILE_SYS_MASK-1)));
/*! - Publish metering event.*/
#define MTR_PUBLISH_EVENT(event) ctrl_event(event)

#define Dot_matrix_handling_disp_message(a,b)
//=============================================================================
//                 GLOBAL FUNCTIONS
//=============================================================================
static uint8_t Failure_time = 0;

//extern 
uint8_t first_init;
//=============================================================================
// FUNCTION
//=============================================================================
// Name           : EEPROM_AT24C1024_Initialize
// Description    : Initialize EEPROM AT24C256 I2C link
//-----------------------------------------------------------------------------
// Author         : HB
// Date           : 22/03/2011
//=============================================================================
void EEPROM_Initialize(void)
{
  /* Enable EEPROM WP to disable writing */
 S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
}

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : EEPROM_AT24C1024_PowerDown
// Description    : Disable EEPROM AT24C256 I2C link
//-----------------------------------------------------------------------------
// Author         : HB
// Date           : 10/01/2012
//=============================================================================
void EEPROM_PowerDown(void)
{
  //* Enable EEPROM WP to disable writing */
   S_GPIO_Set_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;
}


//=============================================================================
// FUNCTION
//=============================================================================
// Name           : WriteInEEPROM
// Description    : Write data to EEPROM using I2C
//-----------------------------------------------------------------------------
// Author         : HB
// Modified       : Mohamed H.Elsaidy
// Date           : 22/03/2011
//=============================================================================
uint8_t EEPROM_write  (uint8_t EEPROM_Add ,  uint8_t* TxData,uint32_t Address, uint32_t numBytes)
{
  uint16_t page_size ;

  uint8_t u8Page_Number;
  uint32_t start_loc, lengthPartially;
  uint8_t u8Page_Counter;
  uint8_t EEprom_STATUS;
  if (0)
  {
    /*in case test accuracy don't save */
    return FALSE;
  }
  if(EEPROM_Add == DATA_EEPROM)
  {
	  page_size = EEPROM_PAGE_SIZE_DRV;
	  EEPROM_Add = EEPROM_AT24C256_I2C_ADDRESS;
  }
  else if(EEPROM_Add == BOALOADER_EEPROM)
  {
	  page_size = EEPROM_PAGE_SIZE;
	  EEPROM_Add = EEPROM_M24M02_I2C_ADDRESS;
  }

  /* Reset EEPROM WP to enable writing */
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;

  start_loc = ((uint32_t)Address) % page_size;
  u8Page_Number = (numBytes + start_loc) / page_size;  
  for(u8Page_Counter = 0; u8Page_Counter <= u8Page_Number; u8Page_Counter++)
  {
    if((u8Page_Counter == 0) && (u8Page_Number == 0))
      lengthPartially = numBytes;
    else if(u8Page_Counter == 0)
      lengthPartially = page_size - start_loc;
    else if(u8Page_Counter == u8Page_Number)
      lengthPartially = (numBytes + start_loc) % EEPROM_PAGE_SIZE_DRV;
    else
      lengthPartially = page_size;
	
    EEprom_STATUS = EEPROM_WRITE(EEPROM_Add,Address, TxData, lengthPartially);
    if(EEprom_STATUS == FALSE)
    {
     
      //  EEPROM_Failure_Handler();
	  return FALSE;
    }
    else
      Failure_time = 0;
    TxData += lengthPartially;
    Address += lengthPartially;
    Delay_ms(6); // 6 milli seconds delay 
   // if(uC_speed_high == 1)
    //  Delay_ms(6);
  }

 return TRUE;
}

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : ReadFromEEPROM
// Description    : Read data from EEPROM using I2C
//-----------------------------------------------------------------------------
// Author         : HB
// Modified       : Mohamed H.Elsaidy
// Date           : 22/03/2011
//=============================================================================
uint8_t EEPROM_read(uint8_t EEPROM_Add ,uint8_t* RxData, uint32_t Address ,  uint32_t numBytes)
{
  TBOOL Status;
 ///////// uint8_t SVMStatus;
  if(EEPROM_Add == DATA_EEPROM)
  {
	  EEPROM_Add = EEPROM_AT24C256_I2C_ADDRESS;
  }
  else if(EEPROM_Add == BOALOADER_EEPROM)
  {
	  EEPROM_Add = EEPROM_M24M02_I2C_ADDRESS;
  }
  
 ////////// SVMStatus = Get_SVM_Interrupt_Status();
//#ifndef POWER_FAIL_PIN_FEATURE
//  Disable_SVM_Interrupt();
//#endif
  
  if(numBytes==0)
    return FALSE;
    
  
 /* Disable EEPROM WP to enable writing */
  
  
  S_GPIO_Clear_Bits(EEPROM_WP_PORT , EEPROM_WP_PIN) ;

  Status = EEPROM_READ(EEPROM_Add , Address ,RxData,numBytes);
  if(Status != STATUS_SUCCESS)
    {
	 /* Enable EEPROM WP to disable writing */ 
       /*RmvCodCmntA_K*/
	  EEPROM_Failure_Handler();
      return FALSE;
    }
    else
      Failure_time = 0;
  /* Enable EEPROM WP to disable writing */ 
     
     /*RmvCodCmntA_K*/
  
  return TRUE;
}





//=============================================================================
// FUNCTION
//=============================================================================
// Name            : EEPROM_Failure_Handler
// Description    : Make An Action when the EEPROM is Failed and we cannot read/write to it.
//-----------------------------------------------------------------------------
// Author         : Moustafa Saied.
// Date           : 15/06/2011.
//=============================================================================
void EEPROM_Failure_Handler(void)
{
  EEPROM_Initialize();
  if (Failure_time >= 10)
  {
#ifdef DEBUG_LOW_POWER
    uart_debug_byte ('x',0);
#endif
    if( first_init == 1)
    {
#if 1
      S_GPIO_Set_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN); 

       /*RmvCodCmntA_K*/

      Delay_ms(500);
      S_I2C_Dis_EN(DISABLE);

      S_GpioInit_PIN(I2C_PORT, I2C_DTA_PIN|I2C_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS);
      S_GPIO_Set_Bits(I2C_PORT, I2C_DTA_PIN);
      S_GPIO_Set_Bits(I2C_PORT, I2C_CLK_PIN);
      Delay_ms(100);
      S_GPIO_Clear_Bits(I2C_PORT, I2C_DTA_PIN);
      S_GPIO_Clear_Bits(I2C_PORT, I2C_CLK_PIN);
      Delay_ms(100);
      S_GPIO_Set_Bits(I2C_PORT, I2C_DTA_PIN);
      S_GPIO_Set_Bits(I2C_PORT, I2C_CLK_PIN);    
#endif
      CORTEX_NVIC_SystemReset(17);      /* system soft reset */
    }
  }
  else
    Failure_time ++;
}
//=============================================================================
// FUNCTION
//=============================================================================
// Name            : EEPROM_Erease
// Description     : Erease the EEPROM
//-----------------------------------------------------------------------------
// Author          : Mohamed H. Elsaidy
// Date            : 11/3/2012
//=============================================================================
uint8_t EEPROM_Erease(uint8_t EEPROM_Add )
{
  uint8_t page_size ;
  uint8_t retVal = STATUS_SUCCESS;
  uint8_t ClearedArray [255] = {0};
  uint32_t i  = 0x00;

  if(EEPROM_Add == DATA_EEPROM)
  {
	  page_size = 255;
	  EEPROM_Add = EEPROM_AT24C256_I2C_ADDRESS;
  }
  else if(EEPROM_Add == BOALOADER_EEPROM)
  {
	  page_size = 255;
	  EEPROM_Add = EEPROM_M24M02_I2C_ADDRESS;
  }

  for(i = 0x00; i < EEPROM_SIZE_IN_BYTES; i += page_size)
  {
    if(EEPROM_write(EEPROM_Add , ClearedArray, i,  sizeof(ClearedArray) ) == FALSE)
    {
      retVal = STATUS_FAIL;
      break;
    }
  }
  return retVal;
}


/*!
 * @brief test EEPROM.
 *
 * @par Description:
 * Testing EEPROM by writing the last page in EEPROM. If it writes successfuly,
 * It will return zero. it's used in self test.
 *
 * @return
 * -  It represents test status. Zero means write successfully.
 */
uint8_t EEPROM_test(uint8_t EEPROM_Add )
{
  /*!@par Pseudo Code:*/
  uint16_t page_size = 256;
  uint8_t test_status = 1;
  uint8_t buf_data[256] = {0xAA};
  uint16_t index = 0;
  uint8_t buf_data_zeros[256] = {0xFF};
  uint8_t buf_data_Erease[256] = {0x00};
  
  S_GPIO_Clear_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN); 
  memset((uint8_t*)buf_data_zeros,(uint8_t)0xff,page_size);
  memset((uint8_t*)buf_data,(uint8_t)0xaa,page_size);
  memset((uint8_t*)buf_data_Erease,(uint8_t)0x00,page_size);

  if(EEPROM_write(EEPROM_Add ,  buf_data_zeros, EEPROM_TEST_PAGE, page_size))
  {
  /*! Writing ones in last page of EEPROM.*/
  if(EEPROM_write(EEPROM_Add , buf_data,EEPROM_TEST_PAGE, page_size))
  {
    for(index = 0x00; index < page_size; index++)
    {
      buf_data[index] = 0x0;
    }
    /*! Reading the last page of EEPROM.*/
    if(EEPROM_read(EEPROM_Add ,  buf_data, EEPROM_TEST_PAGE, page_size))
    {
      test_status = 0;
      /*! Checking all bytes of page equal one or not*/
      for(index = 0x00; index < page_size; index++)
      {
        if(buf_data[index] != 0xAA)
        {
          test_status = 1;
		 // EEPROM_PUBLISH_EVENT(CTRL_DATA_CRPT_FILE_SYS);
	  //    Table03_setFlag_nonVolatileMemoryError();
          break;
        }
      }
    }
  }
  }


  /*! If EEPROM test result is bad */
  if(test_status == 1)
  {
      if(EEPROM_Add == Data_eeprom_address)
      {
        #ifdef DOT_MATRIX_LCD_ENABLE
          Dot_matrix_handling_disp_message("eep1 mal", (uint8_t *)&index); 
        #else
          display_message("eep1 mal", (uint8_t *)&index);
        #endif
      }
      else if(EEPROM_Add == BOALOADER_EEPROM)
      {
        #ifdef DOT_MATRIX_LCD_ENABLE
          Dot_matrix_handling_disp_message("eep2 mal", (uint8_t *)&index); 
        #else
          display_message("eep2 mal", (uint8_t *)&index);
        #endif
      }
  }
  else
  {
      if(EEPROM_Add == Data_eeprom_address)
      {
        #ifdef DOT_MATRIX_LCD_ENABLE
          Dot_matrix_handling_disp_message("eep1 bin", (uint8_t *)&index); 
        #else
          display_message("eep1 bin", (uint8_t *)&index);
        #endif
      }
      else if(EEPROM_Add == BOALOADER_EEPROM)
      {
          #ifdef DOT_MATRIX_LCD_ENABLE
          Dot_matrix_handling_disp_message("eep2 bin", (uint8_t *)&index); 
        #else
          display_message("eep2 bin", (uint8_t *)&index);
        #endif
      }
  }
  vDelay_HW(1);
  EEPROM_write(EEPROM_Add ,  buf_data_Erease, EEPROM_TEST_PAGE, page_size);
  return test_status;
  /*! \n */
}


void check_Eeprom_Fail()
{
  struct Eeprom_check_t EP;
  uint8_t check_val = 0x00;
  file_sys_read((uint8_t*)&EP, FILE_SYS_CHECK_EEPROM_FAIL_ADRS, sizeof(EP));
  check_val = calc_crc((uint8_t*)&EP, sizeof(EP)-1 ,CRC);

  if(check_val != EP.eeprom_crc)
  {
    // SET_EEPROM_FAIL_FLAG();
    if(/*!CTRL_DATA_CRPT_FILE_SYS_STAT*/1)
    {
//        MTR_PUBLISH_EVENT(CTRL_DATA_CRPT_FILE_SYS);
    }
  }
  else
  {
    if(/*CTRL_DATA_CRPT_FILE_SYS_STAT*/1)
    {
        //CLR_EEPROM_FAIL_FLAG();
        //MTR_PUBLISH_EVENT(CTRL_RF_CHIP_COMM_ERR_STOP);
    }
  }

}


void eeprom_check_init(void)
{
  uint8_t count=0x00;
  struct Eeprom_check_t EP;

  for(count = 0 ; count<30;count++)
  {
   EP.eeprom_check_arr[count]=0x55;
  }

  EP.eeprom_crc = calc_crc((uint8_t*)&EP, sizeof(EP)-1,CRC );

  file_sys_write((uint8_t*)&EP, FILE_SYS_CHECK_EEPROM_FAIL_ADRS, sizeof(EP));

}