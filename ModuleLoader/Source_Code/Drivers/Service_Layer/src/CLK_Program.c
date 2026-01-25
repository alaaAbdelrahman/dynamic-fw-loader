
#include "RTC_Interface.h"
#include "CLK_Interface.h"

/*******************************************************************************
  Function Name   :  init_clk

  Parameters(in)  :  None

  Parameters(out) :  None

  Return value    :  None

  Sych/Asynch     :  Synchronous

  Reentrancy      :  None Reentrant

  Description     :  This function is responsible of starting the initialization
                     of system clock.
*******************************************************************************/

void S_CLK_Init(unsigned char freq)
{
  CLK_InitTypeDef CLK_Struct;

  CLK_Struct.ClockType = S_CLK_TYPE_AHBSRC \
                        |S_CLK_TYPE_PLLL   \
                        |S_CLK_TYPE_RTCCLK\
                        |S_CLK_TYPE_HCLK   \
                        |S_CLK_TYPE_PCLK;
  
  /* AHB source is internal RC PLL  */
  CLK_Struct.AHBSource      = S_CLK_AHBSEL_LSPLL;
  if (freq == 0)
  {
  CLK_Struct.PLLL.Frequency = S_CLK_PLLL_13_1072MHz ;// CLK_PLLL_26_2144MHz;
    /* APB divider = 2, freq = 13 MHZ for slow peripherals including SPI, I2C */
  CLK_Struct.PCLK.Divider   = 2;    // 1   /*edit by A_K*/ 
  }
  else if (freq == 1)
  {
    CLK_Struct.PLLL.Frequency = S_CLK_PLLL_26_2144MHz/*CLK_PLLL_13_1072MHz*/ ;// CLK_PLLL_26_2144MHz;
    /* APB divider = 2, freq = 13 MHZ for slow peripherals including SPI, I2C */
    CLK_Struct.PCLK.Divider   = 1;    // 1   /*edit by A_K*/ 
  }
  CLK_Struct.PLLL.Source    = S_CLK_PLLLSRC_XTALL;//CLK_PLLLSRC_RCL;
  CLK_Struct.PLLL.State     = S_CLK_PLLL_ON;
  
  /* RTC with external source without divider */ 
  //CLK_Struct.RTCCLK.Source = CLK_RTCCLKSRC_XTALL | PMU_CONTROL_RTCLK_SEL;//CLK_RTCCLKSRC_XTALL ;
  CLK_Struct.RTCCLK.Source  =  S_CLK_RTCCLKSRC_XTALL;//CLK_RTCCLKSRC_XTALL ;
  CLK_Struct.RTCCLK.Divider =  S_RTC_PSCA_PSCA_0 ;
  
 
  
  /*AHB divider = 1, freq = 26 MHZ for fast peripherals including CPU */ 
  CLK_Struct.HCLK.Divider   = 1;

  
  S_CLK_ClockConfig(&CLK_Struct);
  
  


}/*end function*/

/*------------------------------------------------------------------------------
                       END Function init_clk
------------------------------------------------------------------------------*/