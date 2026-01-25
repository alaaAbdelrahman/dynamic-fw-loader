/*************************************************************
**************************************************************
**      File Name      : GPIO Program.c                     **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Description    : this file include the definition   **
**      of the Functions                                    **
**************************************************************
*************************************************************/




/*************************************************************
**************************************************************
**                   Include section                        **
**************************************************************
**************************************************************
*************************************************************/

#include "GPIO_Interface.h"



/*************************************************************
**************************************************************
**                   Global section                         **
**************************************************************
**************************************************************
*************************************************************/
uint8_t Glob_U8_RelayStatus = 0;                 // Represent the status of relays









/*************************************************************
**************************************************************
**           Function definition section                    **
**************************************************************
**************************************************************
*************************************************************/

/**
  * @brief  GPIO AF configure.
  * @param  GPIOx:(uint32_t*)GPIOB (uint32_t*)GPIOE (uint32_t*)GPIOA
            GPIO_AFx:
                GPIOB_AF_PLLHDIV
                GPIOB_AF_OSC  
                GPIOB_AF_PLLLOUT
                GPIOE_AF_CMP1O
                PMUIO7_AF_PLLDIV
                PMUIO_AF_CMP2O
                PMUIO3_AF_PLLDIV
            NewState:
                ENABLE
                DISABLE
  * @retval None.
  */
void GPIO_AFConfig(uint32_t* GPIOx, uint32_t GPIO_AFx, uint8_t NewState)
{
  if (GPIOx == (uint32_t *)GPIOA)
  {
    GPIOA_AFConfig(GPIO_AFx, NewState);
  }
  else
  {
    GPIOBToF_AFConfig((GPIO_TypeDef*)GPIOx, GPIO_AFx, NewState);
  }
}



/*************************************************************
**************************************************************
**      Function Name  : S_GpioEnableRfidBoard             **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function to enable RFID            **
**************************************************************
*************************************************************/
void S_GpioEnableRfidBoard(void)
{
  S_GPIO_Set_Bits(RFID_MODULE_ENABLE_PORT,RFID_MODULE_ENABLE_PIN);                     //edit by A_K remove the dublicated
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////



/*************************************************************
**************************************************************
**      Function Name  : S_GpioDisableRfidBoard            **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function to Disable RFID           **
**************************************************************
*************************************************************/
void S_GpioDisableRfidBoard(void)
{
  S_GPIO_Clear_Bits(RFID_MODULE_ENABLE_PORT,RFID_MODULE_ENABLE_PIN);                 //edit by A_K remove the dublicated
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




/*************************************************************
**************************************************************
**      Function Name  : S_GpioI2cInitPinsOpLow          **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function to init I2c pins for      **
**      optical in low power mode                           **
**************************************************************
*************************************************************/
void S_GpioI2cInitPinsOpLow()
{

  S_GpioInit_PIN(I2C_PORT, I2C_DTA_PIN|I2C_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Clear_Bits(I2C_PORT, I2C_DTA_PIN|I2C_CLK_PIN);

  S_GpioInit_PIN(I2C_EXTRA_PORT, I2C_EXTRA_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




#if defined (DOT_MATRIX_LCD_ENABLE)
void Dot_Matrix_GPIO_INIT()
{
//  for (int i = 3; i <= 7; i++) // loop for init all pins with LOW
//  {
//    
//    S_GpioInit_PIN(Dot_Matrix_port,(uint16)(1 << i), GPIO_Mode_OUTPUT_CMOS );
//    S_GPIO_Clear_Bits(Dot_Matrix_port, (uint16)(1 << i));
//  }
  S_GpioInit_PIN(Dot_Matrix_port,CS | SDA | SCL | A0, GPIO_Mode_OUTPUT_CMOS );
  S_GPIO_Clear_Bits(Dot_Matrix_port, CS | SDA | SCL | A0);

  // For Lcd power

//  S_GpioInit_PIN(LCD_BACK_LIGHT_PORT,BACK_LIGHT_PIN, GPIO_Mode_OUTPUT_CMOS );
//  S_GPIO_Clear_Bits( LCD_BACK_LIGHT_PORT , BACK_LIGHT_PIN);

  
  S_GpioInit_PIN( LCD_POWER_PORT ,LCD_POWER_PIN  , GPIO_Mode_OUTPUT_CMOS );
  S_GPIO_Set_Bits( LCD_POWER_PORT , LCD_POWER_PIN);                //close POWER LCD
  S_GPIO_Clear_Bits( LCD_POWER_PORT , LCD_POWER_PIN);              // OPEN POWER for LCD
  
  
  S_GpioInit_PIN( Dot_Matrix_port ,CS | SDA | SCL | RES | A0  , GPIO_Mode_OUTPUT_CMOS );
 
  S_GPIO_Set_Bits(Dot_Matrix_port, CS);
  S_GPIO_Set_Bits(Dot_Matrix_port, SDA);
  S_GPIO_Set_Bits(Dot_Matrix_port, SCL);
  S_GPIO_Clear_Bits(Dot_Matrix_port, RES);
  S_GPIO_Clear_Bits(Dot_Matrix_port, A0);
}
#endif










/*************************************************************
**************************************************************
**      Function Name  : S_GpioInit_ALL_Modules             **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function to init GPIO for each     **
**      module that we will use                             **
**************************************************************
*************************************************************/
void S_GpioInit_ALL_Modules()
{

  /*************************************************************
  **************************************************************
  **                   EM122U_BOARD                           **
  **************************************************************
  *************************************************************/
#ifdef EM122U_BOARD
  S_GpioInit_PIN(S_PORT_A, S_GPIO_Pin_2 , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(S_PORT_A,S_GPIO_Pin_2);
#endif





  /*************************************************************
  **************************************************************
  **                   Active & Reactive Section              **
  **************************************************************
  *************************************************************/
 //GPIO_AFConfig((uint32_t*)S_PORT_E, S_GPIO_Pin_12, 1);
 //GPIO_AFConfig((uint32_t*)S_PORT_B, S_GPIO_Pin_8, 1);

  S_GpioInit_PIN(S_PORT_B,S_GPIO_Pin_0|S_GPIO_Pin_6,S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(S_PORT_B,S_GPIO_Pin_0|S_GPIO_Pin_6);
  S_GPIO_Clear_Bits(S_PORT_B,S_GPIO_Pin_0|S_GPIO_Pin_6);
  S_GpioInit_PIN(S_PORT_B,S_GPIO_Pin_0|S_GPIO_Pin_6,S_GPIO_Mode_FORBIDDEN);
  /*************************************************************
  **************************************************************
  **                   RFID Section                           **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(RFID_MODULE_ENABLE_PORT,RFID_MODULE_ENABLE_PIN,S_GPIO_Mode_OUTPUT_CMOS);

  // RFID Module Disable
  if (1)
  {
  S_GPIO_Set_Bits(RFID_MODULE_ENABLE_PORT,RFID_MODULE_ENABLE_PIN);
  Delay_ms(200);
  }
   S_GPIO_Clear_Bits(RFID_MODULE_ENABLE_PORT,RFID_MODULE_ENABLE_PIN);



  /*************************************************************
  **************************************************************
  **        Configure i2c extra, output low  mode             **
  **************************************************************
  *************************************************************/
    S_GpioInit_PIN(I2C_EXTRA_PORT,I2C_EXTRA_PIN , S_GPIO_Mode_OUTPUT_CMOS);

    S_I2C_Dis_EN(DISABLE);

    S_GpioInit_PIN(I2C_PORT, (I2C_DTA_PIN|I2C_CLK_PIN) , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Set_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);
    
    if (1)
    {
    Delay_ms(100);
    S_GPIO_Clear_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);
    }

  S_GPIO_Set_Bits(I2C_PORT, I2C_DTA_PIN);
  S_GPIO_Set_Bits(I2C_PORT, I2C_CLK_PIN);
  Delay_ms(100);
  S_GPIO_Clear_Bits(I2C_PORT, I2C_DTA_PIN);
  S_GPIO_Clear_Bits(I2C_PORT, I2C_CLK_PIN);
  Delay_ms(100);
  S_GPIO_Set_Bits(I2C_PORT, I2C_DTA_PIN);
  S_GPIO_Set_Bits(I2C_PORT, I2C_CLK_PIN);

  S_GpioInit_PIN(I2C_PORT, (I2C_DTA_PIN|I2C_CLK_PIN) , GPIO_Mode_FORBIDDEN);

  /*************************************************************
  **************************************************************
  **                  Sleep mode                              **
  **************************************************************
  *************************************************************/
  /*RmvCodCmntA_K*/
  
  #if defined (MTR_THREE_PH) && !defined (EM130_BOARD) && !defined(EM210_BOARD)
  S_GpioInit_PIN(MTR_SLEEP_PORT, MTR_IRQ_PIN|MTR_SLEEP_PIN, S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(MTR_SLEEP_PORT,MTR_SLEEP_PIN);
  S_GPIO_Set_Bits(MTR_SLEEP_PORT,MTR_IRQ_PIN);
  if (1)
  {
  Delay_ms(200);
  S_GPIO_Clear_Bits(MTR_SLEEP_PORT,MTR_SLEEP_PIN);
  }
  S_GPIO_Clear_Bits(MTR_SLEEP_PORT,MTR_IRQ_PIN);
 #endif

  /*RmvCodCmntA_K*/




 /*************************************************************
  **************************************************************
  **                    Indirect  section                     **
  **************************************************************
  *************************************************************/

#ifdef MTR_INDIRECT
  
  S_GpioInit_PIN(ENCLOSER_COVER_PORT, ENCLOSER_COVER_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GpioInit_PIN(CONTACTOR_PORT, CONTACTOR_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GpioInit_PIN(ENCLOSER_SW_PORT, ENCLOSER_SW_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  
  S_GPIO_Set_Bits(ENCLOSER_COVER_PORT, ENCLOSER_COVER_PIN);
  S_GPIO_Clear_Bits(ENCLOSER_COVER_PORT, ENCLOSER_COVER_PIN);
  
  S_GPIO_Set_Bits(CONTACTOR_PORT, CONTACTOR_PIN);
  S_GPIO_Clear_Bits(CONTACTOR_PORT, CONTACTOR_PIN);
  
  S_GPIO_Set_Bits(ENCLOSER_SW_PORT, ENCLOSER_SW_PIN);
  S_GPIO_Clear_Bits(ENCLOSER_SW_PORT, ENCLOSER_SW_PIN);

  
  S_GpioInit_PIN(ENCLOSER_COVER_PORT, ENCLOSER_COVER_PIN , S_GPIO_Mode_INPUT);
  S_GpioInit_PIN(CONTACTOR_PORT, CONTACTOR_PIN , S_GPIO_Mode_INPUT);
  S_GpioInit_PIN(ENCLOSER_SW_PORT, ENCLOSER_SW_PIN , S_GPIO_Mode_INPUT);

  S_GpioInit_PIN(CONTACTOR_STATUS_PORT, CONTACTOR_STATUS_PIN , S_GPIO_Mode_INPUT);

#endif



  /*************************************************************
  **************************************************************
  **                 CONTACT CARD  section                    **
  **************************************************************
  *************************************************************/
#if defined (CONTACT_CARD) || defined (CONTACT_CONTACTLESS)    
  //#ifdef CONTACT_CARD
/*Configure Contact Card GPIOs*/
  S_GpioInit_PIN(CNTCT_CRD_DTCT_PORT, CRD_DTCT_PIN , S_GPIO_Mode_INPUT);
  // TODO : set pin for active high config
  S_GPIO_Set_Bits(CNTCT_CRD_DTCT_PORT, CRD_DTCT_PIN);
  
  S_GpioInit_PIN(CNTCT_CRD_PORT, CRD_RST_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Clear_Bits(CNTCT_CRD_PORT, CRD_RST_PIN);
  #endif


  /*************************************************************
  **************************************************************
  **         Configure Buzzer, output low CMOS mode           **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(BUZZER_PORT, BUZZER_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Clear_Bits(BUZZER_PORT, BUZZER_PIN);
  S_GPIO_Set_Bits(BUZZER_PORT, BUZZER_PIN);
  S_GPIO_Clear_Bits(BUZZER_PORT, BUZZER_PIN);


  /*************************************************************
  **************************************************************
  **           Configure Relay, output CMOS mode              **
  **************************************************************
  *************************************************************/
  S_GPIO_Clear_Bits(RELAY_PORT, RELAY_CONNECT|RELAY_DISCONNECT);
  S_GpioInit_PIN(RELAY_PORT, RELAY_CONNECT|RELAY_DISCONNECT ,S_GPIO_Mode_OUTPUT_CMOS );

  /*************************************************************
  **************************************************************
  **     Configure LED1 high, LED2 high, output  CMOS mode     **
  **************************************************************
  *************************************************************/
  /*S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
  S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
  S_GpioInit_PIN(TMPR_LED_PORT, TMPR_LED_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GpioInit_PIN(CRDT_LED_PORT, CRDT_LED_PIN , S_GPIO_Mode_OUTPUT_CMOS);
*/
    S_GpioInit_PIN(CRDT_LED_PORT, CRDT_LED_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GpioInit_PIN(TMPR_LED_PORT, TMPR_LED_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  
  S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
  S_GPIO_Set_Bits(CRDT_LED_PORT, CRDT_LED_PIN);


#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('g',5);
#endif

  /*************************************************************
  **************************************************************
  **     Configure LED3 low, output  CMOS mode                **
  **************************************************************
  *************************************************************/
 // S_GPIO_Clear_Bits(LED_3_PORT, LED_3_PIN);
  //S_GpioInit_PIN(LED_3_PORT, LED_3_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  /*RmvCodCmntA_K*/

  /*************************************************************
  **************************************************************
  **     Configure EEPORM WP, output high CMOS mode           **
  **************************************************************
  *************************************************************/
  S_GPIO_Set_Bits(EEPROM_WP_PORT,EEPROM_WP_PIN);
  S_GpioInit_PIN(EEPROM_WP_PORT, EEPROM_WP_PIN ,S_GPIO_Mode_OUTPUT_CMOS );


  /*************************************************************
  **************************************************************
  **     Configure RF CTRl, output low high CMOS mode         **
  **************************************************************
  *************************************************************/
  S_GPIO_Clear_Bits(RF_CTL_PORT,RF_CTL1_PIN|RF_CTL2_PIN);
  S_GpioInit_PIN(RF_CTL_PORT, (RF_CTL1_PIN|RF_CTL2_PIN) , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Clear_Bits(RF_CTL_PORT,RF_CTL1_PIN|RF_CTL2_PIN);


  /*************************************************************
  **************************************************************
  **     Configure Backlight, output low CMOS mode            **
  ***********************MTR_THREE_PH***************************
  *************************************************************/

S_GpioInit_PIN(LCD_BACK_LIGHT_PORT, BACK_LIGHT_PIN , S_GPIO_Mode_OUTPUT_CMOS);

#ifdef MTR_THREE_PH
  S_GPIO_Clear_Bits(LCD_BACK_LIGHT_PORT,BACK_LIGHT_PIN);
#else
  S_GPIO_Set_Bits(LCD_BACK_LIGHT_PORT,BACK_LIGHT_PIN);
#endif



  /*************************************************************
  **************************************************************
  **                 DEBUGING Section                         **
  **************************************************************
  *************************************************************/
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('q',5);
#endif


  /*********************ASHNTTI_PROJECT*************************
  **************************************************************
  **        Configure SW Scroll up , scroll down,             **
  **        MD,terminal,mag,telt,  -->> input mode            **
  **************************************************************
  *************************************************************/
#ifdef ASHNTTI_PROJECT
  S_GpioInit_PIN (SW_PORT, SW_SCROLL_UP_PIN|SW_SCROLL_DOWN_PIN|MD_SW_PIN|TERMINAL_SW_PIN|TELT_SW_PIN |MD_SW_PIN|MAG_SW_PIN,S_GPIO_Mode_INPUT );
#else
  S_GpioInit_PIN (SW_PORT, SW_SCROLL_UP_PIN|SW_SCROLL_DOWN_PIN|MD_SW_PIN|TERMINAL_SW_PIN|TELT_SW_PIN ,S_GPIO_Mode_INPUT );
#endif

  /*************************************************************
  **************************************************************
  **                 DEBUGING Section                         **
  **************************************************************
  *************************************************************/
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('d',5);
#endif

  /*************************************************************
  **************************************************************
  **           Configure  RS485_PIN input mode                **
  **************************************************************
  *************************************************************/
#if (defined(EM122U_BOARD) || defined(EM122_BOARD))
  S_GpioInit_PIN(RS485_PORT_OUT, RS485_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Clear_Bits(RS485_PORT_OUT, RS485_PIN);
#else
  S_GpioInit_PIN(RS485_PORT_OUT, RS485_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Clear_Bits(RS485_PORT_OUT, RS485_PIN);
#ifdef EM_DL130_BOARD
  S_GpioInit_PIN(RS485_ENABLE_Pwr_PORT, RS485_ENABLE_Pwr_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(RS485_ENABLE_Pwr_PORT, RS485_ENABLE_Pwr_PIN);

 S_GpioInit_PIN(RS485_UART_TX_PORT, RS485_UART_TX_PIN , S_GPIO_Mode_OUTPUT_CMOS);
 S_GpioInit_PIN(RS485_UART_RX_PORT, RS485_UART_RX_PIN ,S_GPIO_Mode_INPUT );
 S_GPIO_Set_Bits(RS485_UART_TX_PORT, RS485_UART_TX_PIN);

#endif

#endif


  /*************************************************************
  **************************************************************
  **                 DEBUGING Section                         **
  **************************************************************
  *************************************************************/
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('m',5);
#endif

  /*RmvCodCmntA_K*/

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('b',5);
#endif

  /******************V9203 && ADE7953***************************
  **************************************************************
  **      Configure Meter interrupt request, input   mode     **
  **************************************************************
  *************************************************************/
#ifdef V9203_ENABLE
  S_GpioInit_PIN(MTR_IRQ_PORT, MTR_IRQ_PIN , S_GPIO_Mode_INPUT);
#elif defined (ADE7953_ENABLE)
  PMU_WakeUpPinConfig(MTR_IRQ_PIN, IOA_FALLING);
  PMU_ClearIOAINTStatus(MTR_IRQ_PIN  );
  PMU_INTConfig(PMU_INT_IOAEN, ENABLE);
  CORTEX_SetPriority_ClearPending_EnableIRQ(PMU_IRQn, 0);
#else
  //todo
#endif




  /********************MTR_THREE_PH V9203***********************
  **************************************************************
  **        configure Chip select (CS) pin , output mode      **
  **************************************************************
  *************************************************************/
#if defined( MTR_THREE_PH ) && !defined(MTR_TWO_PH)
  S_GpioInit_PIN(V9203_PORT, V9203_CS_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(V9203_PORT, V9203_CS_PIN );
  
    
  S_GpioInit_PIN(V9203_PORT, ( V9203_CLK_PIN|V9203_MOSI_PIN) , S_GPIO_Mode_FORBIDDEN);    
  S_GpioInit_PIN(V9203_PORT, V9203_MISO_PIN , S_GPIO_Mode_FORBIDDEN);     
#endif


  /**********************ADE7953********************************
  **************************************************************
  **        Configure Volt-DC in, DefAULT, GPIO MODE input    **
  **************************************************************
  *************************************************************/
#ifdef ADE7953_ENABLE
  S_GpioInit_PIN(VDCIN_PORT, VDCIN_PIN , S_GPIO_Mode_INPUT);
#else
  S_GpioInit_PIN(VDCIN_PORT, VDCIN_PIN , GPIO_Mode_FORBIDDEN);
#endif


  /*************************************************************
  **************************************************************
  **                 DEBUGING Section                         **
  **************************************************************
  *************************************************************/
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('v',5);
#endif


  /*************************************************************
  **************************************************************
  **      Configure calibration jumber, input CMOS mode       **
  **************************************************************
  *************************************************************/
#ifndef EM330_BOARD
  S_GpioInit_PIN(CALIB_JUMBER_PORT, CALIB_JUMBER_PIN , S_GPIO_Mode_INPUT);
#endif

  /*************************************************************
  **************************************************************
  **                    Missing Neutarl                       **
  **************************************************************
  *************************************************************/
 #ifdef EM330_BOARD
  S_GpioInit_PIN(Missing_Neutral_Port, Missing_Neutral_Pin , S_GPIO_Mode_INPUT);
 #endif
  /*************************************************************
  **************************************************************
  **                 DEBUGING Section                         **
  **************************************************************
  *************************************************************/
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('w',5);
#endif



  /*************************************************************
  **************************************************************
  **                 GPRS FEATURE                             **
  **************************************************************
  *************************************************************/
#ifdef GPRS_FEATURE
/// For S_UART0
  S_UART_DISABLE(S_UART0);
  S_GPIO_Set_Bits(S_PORT_B, S_GPIO_Pin_2);
  S_GpioInit_PIN(S_PORT_B, S_GPIO_Pin_2 , S_GPIO_Mode_OUTPUT_CMOS);
  S_GpioInit_PIN(S_PORT_A, S_GPIO_Pin_12 ,S_GPIO_Mode_INPUT );

  S_GpioInit_PIN(GPRS_PORT, GPRS_POWER_PIN ,S_GPIO_Mode_OUTPUT_CMOS );//S_PORT_B
  S_GPIO_Clear_Bits(GPRS_PORT, GPRS_POWER_PIN);
  S_GPIO_Set_Bits(GPRS_PORT,GPRS_POWER_PIN);

  S_GpioInit_PIN(RF_CTL_PORT, RF_CTL2_PIN , S_GPIO_Mode_OUTPUT_CMOS);//S_PORT_B
  S_GPIO_Clear_Bits(RF_CTL_PORT, RF_CTL2_PIN);

#endif

  /*RmvCodCmntA_K*/

  /*************************************************************
  **************************************************************
  **                    SuperCap Section                      **
  **************************************************************
  *************************************************************/
#ifdef SUPERCAP_FEATURE
  S_GpioInit_PIN(Super_Cap_Enable_PORT , Super_Cap_Enable_PIN , S_GPIO_Mode_OUTPUT_CMOS);  
  S_GPIO_Clear_Bits(Super_Cap_Enable_PORT,Super_Cap_Enable_PIN);    
#endif
 

 /*************************************************************
  **************************************************************
  **                   Optical Section                        **
  **************************************************************
  *************************************************************/
#if ( defined(OPTICAL_CTRL_FEATURE) && defined(EM130_BOARD) )
  S_GpioInit_PIN(Opt_VDD_Ctrl_PORT , Opt_VDD_Ctrl_PIN , S_GPIO_Mode_INPUT);    //to contorl switch between LINE and Neutarl EM130
#elif (( defined(OPTICAL_CTRL_FEATURE) && defined(EM330_BOARD) ))
    S_GpioInit_PIN(Opt_VDD_Ctrl_PORT , Opt_VDD_Ctrl_PIN , S_GPIO_Mode_OUTPUT_CMOS);    
    S_GPIO_Set_Bits(Opt_VDD_Ctrl_PORT,Opt_VDD_Ctrl_PIN);  
#endif


  /*************************************************************
  **************************************************************
  **                    Security Chip Section                 **
  **************************************************************
  *************************************************************/
  #if ( defined (EM130_BOARD) || defined (EM330_BOARD)   )
    S_GpioInit_PIN(Security_Chip_ENABLE_PORT, Security_Chip_ENABLE_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Set_Bits(Security_Chip_ENABLE_PORT,Security_Chip_ENABLE_PIN);  
#endif


  /*************************************************************
  **************************************************************
  **                 DEBUGING Section                         **
  **************************************************************
  *************************************************************/

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('M',0);
#endif




  /*************************************************************
  **************************************************************
  **                    RFID Section                          **
  **************************************************************
  *************************************************************/
#ifdef MTR_THREE_PH
  S_GpioInit_PIN(RFID_MODULE_ENABLE_PORT, RFID_MODULE_ENABLE_PIN , S_GPIO_Mode_OUTPUT_CMOS);
                    /*RFID Module Disable*/
  S_GPIO_Clear_Bits(RFID_MODULE_ENABLE_PORT,RFID_MODULE_ENABLE_PIN);


  


  /*************************************************************
  **************************************************************
  **                 DEBUGING Section                         **
  **************************************************************
  *************************************************************/
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('=',5);
#endif


  /*************************************************************
  **************************************************************
  **                 Segments for lCD Section                 **
  **************************************************************
  *************************************************************/
#ifndef DOT_MATRIX_LCD_ENABLE
  /* Configure COMs , SEGs pins .... For Each COM and SEG, before enable the corresponding
  pin�s LCD function, programmer should disable corresponding IO input and output
  function manually, and also disable all special function of the corresponding IO
  to ensure LCD function work correctly.*/
  S_GpioInit_PIN(COM_PORT, COM_0_PIN|COM_1_PIN|COM_2_PIN|COM_3_PIN|COM_4_PIN|COM_5_PIN|COM_6_PIN|COM_7_PIN , S_GPIO_Mode_FORBIDDEN);//S_PORT_D
  
  S_GpioInit_PIN(SEG_PART_C_PORT,  SEG_11_PIN|SEG_12_PIN|SEG_13_PIN|SEG_14_PIN|SEG_15_PIN|SEG_16_PIN|SEG_17_PIN|SEG_18_PIN|SEG_19_PIN|SEG_20_PIN|SEG_21_PIN|SEG_22_PIN|SEG_23_PIN ,S_GPIO_Mode_FORBIDDEN );//S_PORT_C
  S_GpioInit_PIN(SEG_PART_A_PORT, SEG_10_PIN , S_GPIO_Mode_FORBIDDEN);//S_PORT_A
  S_GpioInit_PIN(SEG_PART_B_PORT, SEG_24_PIN | SEG_9_PIN , S_GPIO_Mode_FORBIDDEN);//S_PORT_B
  S_GpioInit_PIN(SEG_PART_D_PORT, SEG_0_PIN|SEG_1_PIN|SEG_4_PIN|SEG_5_PIN|SEG_6_PIN|SEG_7_PIN|SEG_8_PIN ,S_GPIO_Mode_FORBIDDEN );//S_PORT_D
  S_GpioInit_PIN(SEG_PART_E_PORT, SEG_3_PIN|SEG_2_PIN , S_GPIO_Mode_FORBIDDEN);//S_PORT_E

#endif


#elif !defined (DOT_MATRIX_LCD_ENABLE)


  /*************************************************************
  **************************************************************
  **                Config Pins as ForBidden  (floating)      **
  **************************************************************
  *************************************************************/
  //TODO new board
  S_GpioInit_PIN(COM_PORT, COM_0_PIN|COM_1_PIN|COM_2_PIN|COM_3_PIN|COM_4_PIN|COM_5_PIN|COM_6_PIN|COM_7_PIN ,S_GPIO_Mode_FORBIDDEN );//S_PORT_D
  S_GpioInit_PIN(SEG_PART_C_PORT, SEG_11_PIN  | SEG_12_PIN  | SEG_13_PIN  | SEG_14_PIN  | SEG_15_PIN  | SEG_16_PIN  | SEG_17_PIN  | SEG_18_PIN  | SEG_19_PIN  | SEG_20_PIN  | SEG_21_PIN  | SEG_22_PIN , S_GPIO_Mode_FORBIDDEN);//S_PORT_C
  S_GpioInit_PIN(SEG_PART_E_PORT, SEG_0_PIN | SEG_1_PIN    |SEG_4_PIN| SEG_23_PIN  | SEG_24_PIN|S_GPIO_Pin_6 , S_GPIO_Mode_FORBIDDEN);//S_PORT_E
  S_GpioInit_PIN(SEG_PART_D_PORT, SEG_2_PIN  | SEG_3_PIN  | SEG_6_PIN  | SEG_7_PIN  | SEG_8_PIN  | SEG_9_PIN  | SEG_10_PIN , S_GPIO_Mode_FORBIDDEN );//S_PORT_D  


#endif



  /*************************MTR_THREE_PH************************
  **************************************************************
  **                Unused Pins as output low                 **
  **************************************************************
  *************************************************************/
#ifdef MTR_THREE_PH
  /*RmvCodCmntA_K*/
#ifndef MTR_INDIRECT
  S_GPIO_Clear_Bits(UN_USED_PORTE,UN_USED_PIN2|UN_USED_PIN10|UN_USED_PIN11/*|UN_USED_PIN6|UN_USED_PIN7*/);
  S_GpioInit_PIN(UN_USED_PORTE, UN_USED_PIN2 , S_GPIO_Mode_OUTPUT_CMOS);
#endif

  /*************************ASHNTTI*****************************
  **************************************************************
  **                Unused Pins as output low                 **
  **************************************************************
  *************************************************************/
  /*RmvCodCmntA_K*/
#ifdef ASHNTTI_PROJECT
  S_GPIO_Clear_Bits(UN_USED_PORTA,UN_USED_PIN0|UN_USED_PIN1|UN_USED_PIN12|UN_USED_PIN13);
  S_GpioInit_PIN(UN_USED_PORTA, UN_USED_PIN0|UN_USED_PIN1|UN_USED_PIN12|UN_USED_PIN13 , S_GPIO_Mode_OUTPUT_CMOS);
#else

  /*************************************************************
  **************************************************************
  **                Unused Pins as output low                 **
  **************************************************************
  *************************************************************/
  S_GPIO_Clear_Bits(UN_USED_PORTA,UN_USED_PIN0|UN_USED_PIN1|MAG_SW_PIN|UN_USED_PIN12|UN_USED_PIN13);
  S_GpioInit_PIN(UN_USED_PORTA, UN_USED_PIN0|UN_USED_PIN1|MAG_SW_PIN|UN_USED_PIN12|UN_USED_PIN13 , S_GPIO_Mode_OUTPUT_CMOS );
#endif

#ifdef MTR_INDIRCT
  S_GPIO_Clear_Bits(UN_USED_PORTB,UN_USED_PIN2);
  S_GpioInit_PIN(UN_USED_PORTB, UN_USED_PIN2 ,S_GPIO_Mode_OUTPUT_CMOS );
#else
  S_GPIO_Clear_Bits(UN_USED_PORTB,(UN_USED_PIN2|ENCLOSER_SW_PIN));//amr rx
  S_GpioInit_PIN(UN_USED_PORTB, (UN_USED_PIN2|ENCLOSER_SW_PIN) ,S_GPIO_Mode_OUTPUT_CMOS );
#endif

#endif


  /**********************Meter Single Phase*********************
  **************************************************************
  **                         PLC Pin                          **
  **************************************************************
  *************************************************************/
#ifndef MTR_THREE_PH
  S_GpioInit_PIN(PLC_RESET_PORT, PLC_RESET_PIN , S_GPIO_Mode_FORBIDDEN);//S_PORT_E
#endif

  /*************************************************************
  **************************************************************
  **                 SPI Section              **
  **************************************************************
  *************************************************************/
#ifdef HW_SPI
/* cs*/
  S_GPIO_Clear_Bits(HARD_SPI_CS_PORT,HARD_SPI_CS_PIN);
  S_GpioInit_PIN(HARD_SPI_CS_PORT, HARD_SPI_CS_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(HARD_SPI_CS_PORT, HARD_SPI_CS_PIN);   
  /* MISO*/
  
  S_GPIO_Clear_Bits(HARD_SPI_MISO_PORT, HARD_SPI_MISO_PIN);
  S_GpioInit_PIN(HARD_SPI_MISO_PORT, HARD_SPI_MISO_PIN ,S_GPIO_Mode_INPUT ); 
  /* MOSI ,CLk */

  S_GPIO_Clear_Bits(HARD_SPI_MOSI_PORT, HARD_SPI_MOSI_PIN|HARD_SPI_CLK_PIN);
  S_GpioInit_PIN(HARD_SPI_MOSI_PORT, HARD_SPI_MOSI_PIN|HARD_SPI_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(HARD_SPI_MOSI_PORT, HARD_SPI_MOSI_PIN|HARD_SPI_CLK_PIN);
#endif
  
#ifdef SW_SPI
  
  /* cs*/
  S_GPIO_Clear_Bits(SOFTSPI_CS_PORT,SOFTSPI_CS_PIN);
  S_GpioInit_PIN(SOFTSPI_CS_PORT, SOFTSPI_CS_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(SOFTSPI_CS_PORT, SOFTSPI_CS_PIN);   
  /* MISO*/
  
  S_GPIO_Clear_Bits(SOFTSPI_MISO_PORT, SOFTSPI_MISO_PIN);
  S_GpioInit_PIN(SOFTSPI_MISO_PORT, SOFTSPI_MISO_PIN ,S_GPIO_Mode_INPUT ); 
  /* MOSI ,CLk */

  S_GPIO_Clear_Bits(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN|SOFTSPI_CLK_PIN);
  S_GpioInit_PIN(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN|SOFTSPI_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Set_Bits(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN|SOFTSPI_CLK_PIN);
  
#endif
  
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





#ifdef MTR_THREE_PH


/****************Meter is Three phase*************************
**************************************************************
**      Function Name  : S_GpioShutdownPWD                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function to power off the GPIO     **
**      if Three phase                                      **
**************************************************************
*************************************************************/
void S_GpioShutdownPWD (void)
{
  //GPIO_InitType GPIO_InitStruct;

  /*************************************************************
  **************************************************************
  **                    GPRS Section                          **
  **************************************************************
  *************************************************************/
#ifdef GPRS_FEATURE
  S_GPIO_Clear_Bits(GPRS_PORT,GPRS_POWER_PIN);//gprs
#endif


  /*************************************************************
  **************************************************************
  **                  GPIO PORT_D Section                     **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(COM_PORT, COM_0_PIN|COM_1_PIN|COM_2_PIN|COM_3_PIN|COM_4_PIN|COM_5_PIN|COM_6_PIN|COM_7_PIN|SEG_0_PIN|SEG_1_PIN|SEG_4_PIN|SEG_5_PIN|SEG_6_PIN|SEG_7_PIN|SEG_8_PIN, S_GPIO_Mode_OUTPUT_CMOS);//S_PORT_D


  /*************************************************************
  **************************************************************
  **                  GPIO PORT_C Section                     **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(SEG_PART_C_PORT, SEG_11_PIN|SEG_12_PIN|SEG_13_PIN|SEG_14_PIN|SEG_15_PIN|SEG_16_PIN|SEG_17_PIN|SEG_18_PIN|SEG_19_PIN|SEG_20_PIN|SEG_21_PIN|SEG_22_PIN|SEG_23_PIN ,  S_GPIO_Mode_OUTPUT_CMOS);//S_PORT_C



  /*************************************************************
  **************************************************************
  **                  GPIO PORT_A Section                     **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(SEG_PART_A_PORT, SEG_15_PIN|SEG_13_PIN , S_GPIO_Mode_OUTPUT_CMOS);//S_PORT_A


  /*************************************************************
  **************************************************************
  **                  GPIO PORT_B Section                     **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(SEG_PART_B_PORT, SEG_24_PIN|SEG_9_PIN , S_GPIO_Mode_OUTPUT_CMOS);//S_PORT_B

  /*************************************************************
  **************************************************************
  **                  GPIO PORT_D Section                     **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(SEG_PART_D_PORT, SEG_0_PIN , S_GPIO_Mode_OUTPUT_CMOS);//S_PORT_D


  /*************************************************************
  **************************************************************
  **                  GPIO PORT_E Section                     **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(SEG_PART_E_PORT, SEG_3_PIN|S_GPIO_Pin_6|S_GPIO_Pin_7 , S_GPIO_Mode_OUTPUT_CMOS);//S_PORT_E


  /*************************************************************
  **************************************************************
  **                  Battery sense Section                   **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(BAT_SENCE_PORT, BAT_SENCE_PIN , S_GPIO_Mode_OUTPUT_CMOS);


  /*************************************************************
  **************************************************************
  **                  Switches Section                        **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(SW_PORT,SW_SCROLL_UP_PIN|SW_SCROLL_DOWN_PIN|MD_SW_PIN , S_GPIO_Mode_FORBIDDEN);
  S_GPIO_Set_Bits(SW_PORT,SW_SCROLL_UP_PIN|SW_SCROLL_DOWN_PIN|MD_SW_PIN);




  // MCU_TX_RF_TX , MCU_RX_AMR_TX_PIN
  S_GpioInit_PIN(RF_TX_PORT,MCU_TX_RF_TX|MCU_RX_AMR_TX_PIN , S_GPIO_Mode_FORBIDDEN);
  //MCU_RX_RF_RX , MCU_TX_AMR_RX_PIN
  S_GpioInit_PIN(RF_RX_PORT, MCU_RX_RF_RX|MCU_TX_AMR_RX_PIN , S_GPIO_Mode_OUTPUT_CMOS);



  /*************************************************************
  **************************************************************
  **                  Optical Section                        **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(MCU_RX_OP_TXD_PORT, MCU_RX_OP_RXD_PIN|MCU_RX_OP_TXD_PIN , S_GPIO_Mode_FORBIDDEN);//S_PORT_B




  /*************************************************************
  **************************************************************
  **                 V9203 metering chip Section              **
  **************************************************************
  *************************************************************/
  #if defined (MTR_THREE_PH) && !defined (EM130_BOARD) && !defined (EM210_BOARD)
  S_GpioInit_PIN(V9203_PORT, ( V9203_CS_PIN|V9203_CLK_PIN|V9203_MISO_PIN|V9203_MOSI_PIN|MTR_SLEEP_PIN) , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Clear_Bits(V9203_PORT, V9203_CS_PIN|V9203_CLK_PIN|V9203_MISO_PIN|V9203_MOSI_PIN );
  S_GPIO_Set_Bits(V9203_PORT, MTR_SLEEP_PIN );
  #endif

#ifdef EM330_BOARD

   
    //EEPROM Section
   // S_GpioInit_PIN(I2C_EXTRA_PORT, I2C_EXTRA_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Set_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);

  //  S_GpioInit_PIN(I2C_PORT, I2C_DTA_PIN|I2C_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(I2C_PORT,I2C_DTA_PIN|I2C_CLK_PIN); 

 //   S_GpioInit_PIN(EEPROM_WP_PORT, EEPROM_WP_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(EEPROM_WP_PORT,EEPROM_WP_PIN);     

    
        // Flash 
    
#ifdef HW_SPI
    S_GpioInit_PIN(HARD_SPI_WP_PORT, HARD_SPI_WP_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(HARD_SPI_WP_PORT,HARD_SPI_WP_PIN);  

    S_GpioInit_PIN(HARD_SPI_MOSI_PORT, HARD_SPI_MOSI_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(HARD_SPI_MOSI_PORT,HARD_SPI_MOSI_PIN);  

    S_GpioInit_PIN(HARD_SPI_MISO_PORT, HARD_SPI_MISO_PIN , S_GPIO_Mode_INPUT); 
    //S_GPIO_Clear_Bits(HARD_SPI_MISO_PORT,HARD_SPI_MISO_PIN);  

    S_GpioInit_PIN(HARD_SPI_CLK_PORT, HARD_SPI_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(HARD_SPI_CLK_PORT,HARD_SPI_CLK_PIN); 

    S_GpioInit_PIN(HARD_SPI_CS_PORT, HARD_SPI_CS_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(HARD_SPI_CS_PORT,HARD_SPI_CS_PIN); 
    
#endif
    
#ifdef SW_SPI
    
    S_GpioInit_PIN(SOFTSPI_WP_PORT, SOFTSPI_WP_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(SOFTSPI_WP_PORT,SOFTSPI_WP_PIN);  

    S_GpioInit_PIN(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(SOFTSPI_MOSI_PORT,SOFTSPI_MOSI_PIN);  

    S_GpioInit_PIN(SOFTSPI_MISO_PORT, SOFTSPI_MISO_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(SOFTSPI_MISO_PORT,SOFTSPI_MISO_PIN);  

    S_GpioInit_PIN(SOFTSPI_CLK_PORT, SOFTSPI_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(SOFTSPI_CLK_PORT,SOFTSPI_CLK_PIN); 

    S_GpioInit_PIN(SOFTSPI_CS_PORT, SOFTSPI_CS_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(SOFTSPI_CS_PORT,SOFTSPI_CS_PIN);  
#endif
  
  
#endif
    //Super Cap 
    #if defined (EM330_BOARD) && defined(SUPERCAP_FEATURE)
    S_GpioInit_PIN(Super_Cap_SENCE_PORT, Super_Cap_SENCE_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(Super_Cap_SENCE_PORT,Super_Cap_SENCE_PIN);
       // Super Cap 
    S_GpioInit_PIN(Super_Cap_Enable_PORT , Super_Cap_Enable_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Set_Bits(Super_Cap_Enable_PORT,Super_Cap_Enable_PIN);   
       // LCD  
    //LCD_ENABLE
    S_GpioInit_PIN(LCD_POWER_PORT, LCD_POWER_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Set_Bits(LCD_POWER_PORT,LCD_POWER_PIN);
    S_GpioInit_PIN(Dot_Matrix_port, SDA|SCL|A0|RES|CS , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(Dot_Matrix_port,SDA|SCL|A0|RES|CS);  
   #endif
   #if ( defined (EM330_BOARD) ) 
    S_GpioInit_PIN(Security_Chip_ENABLE_PORT, Security_Chip_ENABLE_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(Security_Chip_ENABLE_PORT,Security_Chip_ENABLE_PIN); 
    #endif
  //  S_GpioInit_PIN(MCU_RX_OP_TXD_PORT, MCU_RX_OP_TXD_PIN , S_GPIO_Mode_INPUT); 
  //  S_GPIO_Clear_Bits(MCU_RX_OP_TXD_PORT,MCU_RX_OP_TXD_PIN); 
    
   // S_GpioInit_PIN(MCU_RX_OP_RXD_PORT, MCU_RX_OP_RXD_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
  //  S_GPIO_Clear_Bits(MCU_RX_OP_RXD_PORT,MCU_RX_OP_RXD_PIN); 
    
#if (( defined(OPTICAL_CTRL_FEATURE) && defined(EM330_BOARD) ))
    S_GpioInit_PIN(Opt_VDD_Ctrl_PORT , Opt_VDD_Ctrl_PIN , S_GPIO_Mode_OUTPUT_CMOS);    
    S_GPIO_Clear_Bits(Opt_VDD_Ctrl_PORT,Opt_VDD_Ctrl_PIN);  
#endif
   
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////



#else           /* Meter is Single Phase*/



/*************************************************************
**************************************************************
**      Function Name  : GPIO_Shutdown_PWD                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function to power off the GPIO     **
**      if Single phase                                     **
**************************************************************
*************************************************************/
void S_GpioShutdownPWD (void)
{


  /*************************************************************
  **************************************************************
  **                    GPRS Section                          **
  **************************************************************
  *************************************************************/
#ifdef GPRS_FEATURE
  S_GPIO_Clear_Bits(GPRS_PORT,GPRS_POWER_PIN);//gprs
#endif

  /*************************************************************
  **************************************************************
  **                  GPIO PORT_D Section                     **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(COM_PORT, COM_0_PIN|COM_1_PIN|COM_2_PIN|COM_3_PIN|COM_4_PIN|COM_5_PIN|COM_6_PIN|COM_7_PIN| SEG_2_PIN  | SEG_3_PIN  | SEG_6_PIN  | SEG_7_PIN  | SEG_8_PIN  | SEG_9_PIN  | SEG_10_PIN ,GPIO_Mode_FORBIDDEN );//S_PORT_D


  /*************************************************************
  **************************************************************
  **                  GPIO PORT_C Section                     **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(SEG_PART_C_PORT, S_GPIO_Pin_4|  S_GPIO_Pin_5 | S_GPIO_Pin_6 |  S_GPIO_Pin_7 | S_GPIO_Pin_8 | S_GPIO_Pin_9 | S_GPIO_Pin_10 | S_GPIO_Pin_11 | S_GPIO_Pin_12 | S_GPIO_Pin_13 | S_GPIO_Pin_14 | S_GPIO_Pin_15 , GPIO_Mode_FORBIDDEN );//S_PORT_C


  /*************************************************************
  **************************************************************
  **                  GPIO PORT_E Section                     **
  **************************************************************
  *************************************************************/
  S_GpioInit_PIN(SEG_PART_E_PORT, SEG_0_PIN | SEG_1_PIN    | SEG_23_PIN  | SEG_24_PIN , GPIO_Mode_FORBIDDEN);//S_PORT_E

  S_GpioInit_PIN(S_PORT_E, S_GPIO_Pin_0|S_GPIO_Pin_1 | S_GPIO_Pin_13 |S_GPIO_Pin_14|S_GPIO_Pin_15 , S_GPIO_Mode_FORBIDDEN );



  /*************************************************************
  **************************************************************
  **                  Battery sense Section                   **
  **************************************************************
  *************************************************************/
  //S_GpioInit_PIN(BAT_SENCE_PORT, BAT_SENCE_PIN , S_GPIO_Mode_OUTPUT_CMOS);


  /*************************************************************
  **************************************************************
  **                  SEG Section Port B                      **
  **************************************************************
  *************************************************************/
#if (defined(EM122U_BOARD) || defined(EM122_BOARD))
  S_GpioInit_PIN(SEG_PART_B_PORT, S_GPIO_Pin_0|S_GPIO_Pin_2|S_GPIO_Pin_3 , S_GPIO_Mode_FORBIDDEN);
#else
  S_GpioInit_PIN(SEG_PART_B_PORT, S_GPIO_Pin_0|S_GPIO_Pin_2|S_GPIO_Pin_3|S_GPIO_Pin_4 , S_GPIO_Mode_FORBIDDEN);
#endif

  /*************************************************************
  **************************************************************
  **                  SEG Section Port A                      **
  **************************************************************
  *************************************************************/
#ifdef ASHNTTI_PROJECT
  S_GpioInit_PIN(SEG_PART_A_PORT, S_GPIO_Pin_3| S_GPIO_Pin_11|S_GPIO_Pin_12|S_GPIO_Pin_5 ,S_GPIO_Mode_FORBIDDEN );
#else
  S_GpioInit_PIN(SEG_PART_A_PORT, S_GPIO_Pin_8| S_GPIO_Pin_3| S_GPIO_Pin_11|S_GPIO_Pin_12|S_GPIO_Pin_5 ,S_GPIO_Mode_FORBIDDEN );
#endif


  /*************************************************************
  **************************************************************
  **          check which board to clear pins                 **
  *********EM122U_BOARD || EM122_BOARD or EM110_BOARD***********
  *************************************************************/
#if (defined(EM122U_BOARD) || defined(EM122_BOARD))
  S_GPIO_Clear_Bits(S_PORT_A,S_GPIO_Pin_14);
  S_GPIO_Clear_Bits(S_PORT_B,S_GPIO_Pin_4);
#else

#if !defined(EM110_BOARD) &&!defined (EM330_BOARD)&& !defined(EM130_BOARD) && !defined(EM210_BOARD)
  S_GPIO_Clear_Bits(S_PORT_A,S_GPIO_Pin_15);
  S_GPIO_Clear_Bits(S_PORT_B,S_GPIO_Pin_5);
#else
    S_GPIO_Clear_Bits(S_PORT_A,S_GPIO_Pin_14);
    S_GPIO_Clear_Bits(S_PORT_B,S_GPIO_Pin_4);
#endif
#endif


  /*************************************************************
  **************************************************************
  **                  Optical Reset Section                   **
  **************************************************************
  *************************************************************/
 // S_GpioInit_PIN(MCU_RX_OP_TXD_PORT, MCU_RX_OP_RXD_PIN|MCU_RX_OP_TXD_PIN , S_GPIO_Mode_FORBIDDEN );//S_PORT_B
 // S_GpioInit_PIN(MCU_RX_OP_TXD_PORT, MCU_RX_OP_RXD_PIN|MCU_RX_OP_TXD_PIN , S_GPIO_Mode_OUTPUT_CMOS );//S_PORT_B
 // S_GPIO_Clear_Bits(MCU_RX_OP_TXD_PORT,MCU_RX_OP_TXD_PIN);
 // S_GPIO_Clear_Bits(MCU_RX_OP_RXD_PORT,MCU_RX_OP_RXD_PIN);
  
#ifdef EM130_BOARD
     // LED1
     S_GpioInit_PIN(CRDT_LED_PORT, CRDT_LED_PIN,S_GPIO_Mode_FORBIDDEN);
    //VDCN
     S_GpioInit_PIN(VDCIN_PORT, VDCIN_PIN , GPIO_Mode_FORBIDDEN);
    //BAT Sence
    S_GpioInit_PIN(BAT_SENCE_PORT, BAT_SENCE_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(BAT_SENCE_PORT,BAT_SENCE_PIN);


    // Relay close
    S_GpioInit_PIN(RELAY_PORT, RELAY_CONNECT , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(RELAY_PORT,RELAY_CONNECT);
    // Relay open
    S_GpioInit_PIN(RELAY_PORT, RELAY_DISCONNECT , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(RELAY_PORT,RELAY_DISCONNECT);
    
    // PLC Reset 
    S_GpioInit_PIN(RF_CTL_PORT, RF_CTL1_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(RF_CTL_PORT,RF_CTL1_PIN);
   // RF CTL2 
    S_GpioInit_PIN(RF_CTL_PORT, RF_CTL2_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(RF_CTL_PORT,RF_CTL2_PIN);
    // LED3 LED 4 for Active and reactive
    S_GpioInit_PIN(UN_USED_PORTA, UN_USED_PIN0|UN_USED_PIN1,S_GPIO_Mode_FORBIDDEN);  
    //Switchs 
   //S_GpioInit_PIN(SW_PORT, SW_SCROLL_UP_PIN |SW_SCROLL_DOWN_PIN |MD_SW_PIN |MAG_SW_PIN |TELT_SW_PIN |TERMINAL_SW_PIN,S_GPIO_Mode_INPUT);
   // S_GpioInit_PIN(SW_PORT, SW_SCROLL_UP_PIN |SW_SCROLL_DOWN_PIN |MD_SW_PIN |MAG_SW_PIN |TELT_SW_PIN |TERMINAL_SW_PIN,S_GPIO_Mode_FORBIDDEN);
    S_GpioInit_PIN(SW_PORT,SW_SCROLL_UP_PIN|SW_SCROLL_DOWN_PIN|MD_SW_PIN |M_SW_PIN, S_GPIO_Mode_FORBIDDEN);
    S_GPIO_Set_Bits(SW_PORT,SW_SCROLL_UP_PIN|SW_SCROLL_DOWN_PIN|MD_SW_PIN|M_SW_PIN);
    // optical
    S_GpioInit_PIN(MCU_RX_OP_TXD_PORT, MCU_RX_OP_RXD_PIN|MCU_RX_OP_TXD_PIN , S_GPIO_Mode_FORBIDDEN );
    //S_GpioInit_PIN(MCU_RX_OP_TXD_PORT ,MCU_RX_OP_TXD_PIN,S_GPIO_Mode_FORBIDDEN);
    //S_GpioInit_PIN(MCU_RX_OP_RXD_PORT , MCU_RX_OP_RXD_PIN , S_GPIO_Mode_INPUT);
    // RS485

    S_GpioInit_PIN(S_PORT_B ,S_GPIO_Pin_7,S_GPIO_Mode_FORBIDDEN);
    S_GpioInit_PIN(S_PORT_B , S_GPIO_Pin_1 , S_GPIO_Mode_INPUT);
        
    
    //EEPROM Section
    S_GpioInit_PIN(I2C_EXTRA_PORT, I2C_EXTRA_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Set_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);

    S_GpioInit_PIN(I2C_PORT, I2C_DTA_PIN|I2C_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(I2C_PORT,I2C_DTA_PIN|I2C_CLK_PIN); 

    S_GpioInit_PIN(EEPROM_WP_PORT, EEPROM_WP_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(EEPROM_WP_PORT,EEPROM_WP_PIN);     

    // CF  Reactive & Active
    S_GpioInit_PIN(CF_Reactive_PORT, CF_Reactive_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(CF_Reactive_PORT,CF_Reactive_PIN);     

    S_GpioInit_PIN(CF_Active_PORT, CF_Active_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(CF_Active_PORT,CF_Active_PIN);     
    
    // Flash 
    
#ifdef HW_SPI
    S_GpioInit_PIN(HARD_SPI_WP_PORT, HARD_SPI_WP_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(HARD_SPI_WP_PORT,HARD_SPI_WP_PIN);  

    S_GpioInit_PIN(HARD_SPI_MOSI_PORT, HARD_SPI_MOSI_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(HARD_SPI_MOSI_PORT,HARD_SPI_MOSI_PIN);  

    S_GpioInit_PIN(HARD_SPI_MISO_PORT, HARD_SPI_MISO_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(HARD_SPI_MISO_PORT,HARD_SPI_MISO_PIN);  

    S_GpioInit_PIN(HARD_SPI_CLK_PORT, HARD_SPI_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(HARD_SPI_CLK_PORT,HARD_SPI_CLK_PIN); 

    S_GpioInit_PIN(HARD_SPI_CS_PORT, HARD_SPI_CS_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(HARD_SPI_CS_PORT,HARD_SPI_CS_PIN); 
    
#endif
    
#ifdef SW_SPI
    
    S_GpioInit_PIN(SOFTSPI_WP_PORT, SOFTSPI_WP_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(SOFTSPI_WP_PORT,SOFTSPI_WP_PIN);  

    S_GpioInit_PIN(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(SOFTSPI_MOSI_PORT,SOFTSPI_MOSI_PIN);  

    S_GpioInit_PIN(SOFTSPI_MISO_PORT, SOFTSPI_MISO_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(SOFTSPI_MISO_PORT,SOFTSPI_MISO_PIN);  

    S_GpioInit_PIN(SOFTSPI_CLK_PORT, SOFTSPI_CLK_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(SOFTSPI_CLK_PORT,SOFTSPI_CLK_PIN); 

    S_GpioInit_PIN(SOFTSPI_CS_PORT, SOFTSPI_CS_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(SOFTSPI_CS_PORT,SOFTSPI_CS_PIN);  
#endif
#ifdef SUPERCAP_FEATURE
   // Super Cap 
   // S_GpioInit_PIN(Super_Cap_Enable_PORT , Super_Cap_Enable_PIN , S_GPIO_Mode_INPUT);
      S_GpioInit_PIN(Super_Cap_Enable_PORT , Super_Cap_Enable_PIN , S_GPIO_Mode_OUTPUT_CMOS);   
      S_GPIO_Set_Bits(Super_Cap_Enable_PORT,Super_Cap_Enable_PIN);  
#endif
    // GPIO Modem 
    S_GpioInit_PIN(GPIO_M_1_PORT , GPIO_M_1_PIN , S_GPIO_Mode_FORBIDDEN);   
    S_GpioInit_PIN(GPIO_M_2_PORT , GPIO_M_2_PIN , S_GPIO_Mode_FORBIDDEN);   
    S_GpioInit_PIN(GPIO_M_3_PORT , GPIO_M_3_PIN , S_GPIO_Mode_FORBIDDEN);  
    S_GpioInit_PIN(MBUS_GPIO_M_2_PORT , MBUS_GPIO_M_2_PIN , S_GPIO_Mode_FORBIDDEN);      
     // LED2
     S_GpioInit_PIN(TMPR_LED_PORT, TMPR_LED_PIN,S_GPIO_Mode_FORBIDDEN); 

     // Security Chip
    S_GpioInit_PIN(Security_Chip_ENABLE_PORT, Security_Chip_ENABLE_PIN , S_GPIO_Mode_OUTPUT_CMOS); 
    S_GPIO_Clear_Bits(Security_Chip_ENABLE_PORT,Security_Chip_ENABLE_PIN);  

    // RFID   
    S_GpioInit_PIN(RFID_MODULE_ENABLE_PORT,RFID_MODULE_ENABLE_PIN,S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(RFID_MODULE_ENABLE_PORT,RFID_MODULE_ENABLE_PIN);

   //Contact card 
    S_GpioInit_PIN(CNTCT_CRD_DTCT_PORT, CRD_DTCT_PIN , S_GPIO_Mode_FORBIDDEN);
    S_GpioInit_PIN(CNTCT_CRD_PORT, CRD_RST_PIN , S_GPIO_Mode_FORBIDDEN);


     //  unsed pins  
    S_GpioInit_PIN(UN_USED_PORTC, S_GPIO_Pin_6|S_GPIO_Pin_8 | S_GPIO_Pin_4, S_GPIO_Mode_FORBIDDEN);

    // UART 2 
    S_GpioInit_PIN(UART_RX2_PORT , UART_RX2_PIN|UART_TX2_PIN , S_GPIO_Mode_FORBIDDEN);   
    //S_GpioInit_PIN(UART_RX2_PORT , UART_RX2_PIN , S_GPIO_Mode_INPUT);   
    //S_GpioInit_PIN(UART_TX2_PORT, UART_TX2_PIN, S_GPIO_Mode_FORBIDDEN);
           
    // LCD  
    //LCD_ENABLE
    S_GpioInit_PIN(LCD_POWER_PORT, LCD_POWER_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Set_Bits(LCD_POWER_PORT,LCD_POWER_PIN);
    S_GpioInit_PIN(Dot_Matrix_port, SDA|SCL|A0|RES|CS , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(Dot_Matrix_port,SDA|SCL|A0|RES|CS);  

    // Calibration 
    S_GpioInit_PIN(CALIB_JUMBER_PORT, CALIB_JUMBER_PIN, S_GPIO_Mode_FORBIDDEN);
    
    //back light
    S_GpioInit_PIN(LCD_BACK_LIGHT_PORT, BACK_LIGHT_PIN, S_GPIO_Mode_FORBIDDEN);
     
    //RS485 control 
    S_GpioInit_PIN(RS485_PORT_OUT, RS485_PIN, S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(RS485_PORT_OUT,RS485_PIN);     
    //buzzer
    S_GpioInit_PIN(BUZZER_PORT, BUZZER_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(BUZZER_PORT,BUZZER_PIN);  
    
    // 
    S_GpioInit_PIN(S_PORT_D, S_GPIO_Pin_0 , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(S_PORT_D,S_GPIO_Pin_0);  
    S_GpioInit_PIN(S_PORT_E, S_GPIO_Pin_3 , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(S_PORT_E,S_GPIO_Pin_3); 

    S_GpioInit_PIN(S_PORT_A, S_GPIO_Pin_13|S_GPIO_Pin_12|S_GPIO_Pin_7|S_GPIO_Pin_5 , S_GPIO_Mode_INPUT);

    S_GpioInit_PIN(S_PORT_B, S_GPIO_Pin_3|S_GPIO_Pin_2 , S_GPIO_Mode_FORBIDDEN);

    //programing Pins  65micro 
    S_GpioInit_PIN(S_PORT_A, S_GPIO_Pin_0 , S_GPIO_Mode_OUTPUT_CMOS);
    S_GpioInit_PIN(S_PORT_A, S_GPIO_Pin_1 , S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Clear_Bits(S_PORT_A,S_GPIO_Pin_1|S_GPIO_Pin_0);

    

   
    
    S_GpioInit_PIN(S_PORT_F, S_GPIO_Pin_1 , S_GPIO_Mode_FORBIDDEN);  // this pin for Bat1 must be forbidden to remove the effect from battery to ADC at low power mode 

#endif
  

}
#endif
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




/*************************************************************
**************************************************************
**      Function Name  : S_GpioInitCalibPinINPUT          **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function to init Calibration pin   **
**************************************************************
*************************************************************/
void S_GpioInitCalibPinINPUT(void)
{
#ifndef EM330_BOARD
  S_GpioInit_PIN(CALIB_JUMBER_PORT, CALIB_JUMBER_PIN , S_GPIO_Mode_INPUT);
#endif
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





/*************************************************************
**************************************************************
**      Function Name  : S_GpioWakeUpPinsInit              **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): None                               **
**      Return value   : None                               **
**      Description    : Function to init the wakeUp pin    **
**************************************************************
*************************************************************/
void S_GpioWakeUpPinsInit (void)
{
  //note it depened on port A only
    /* Configure SW Scroll up , scroll down,MD,terminal,mag,telt,  input mode */
  S_GpioInit_PIN(SW_PORT, TERMINAL_SW_PIN|TELT_SW_PIN , S_GPIO_Mode_INPUT );

 /*************************************************************
  **************************************************************
  **              ASHNTTI_PROJECT section                     **
  **************************************************************
  *************************************************************/ 
#ifdef ASHNTTI_PROJECT
  S_GpioInit_PIN(SW_PORT, SW_SCROLL_UP_PIN | SW_SCROLL_DOWN_PIN |TERMINAL_SW_PIN|TELT_SW_PIN , S_GPIO_Mode_INPUT );
#endif
  S_GpioInit_PIN(SW_PORT, TERMINAL_SW_PIN|TELT_SW_PIN , S_GPIO_Mode_INPUT );

  PMU_WakeUpPinConfig(TELT_SW_PIN , IOA_FALLING);
  PMU_WakeUpPinConfig(TERMINAL_SW_PIN , IOA_FALLING);


#ifdef ASHNTTI_PROJECT
  if (S_GPIO_Get_Bits(SW_PORT, SW_SCROLL_UP_PIN) == 0)
    PMU_WakeUpPinConfig(SW_SCROLL_UP_PIN , IOA_RISING);
  else
    PMU_WakeUpPinConfig(SW_SCROLL_UP_PIN , IOA_FALLING);

  if (S_GPIO_Get_Bits(SW_PORT, SW_SCROLL_DOWN_PIN) == 0)
    PMU_WakeUpPinConfig(SW_SCROLL_DOWN_PIN , IOA_RISING);
  else
    PMU_WakeUpPinConfig(SW_SCROLL_DOWN_PIN , IOA_FALLING);

  PMU_ClearIOAINTStatus(SW_SCROLL_UP_PIN | SW_SCROLL_DOWN_PIN);
#endif

  PMU_ClearIOAINTStatus(TELT_SW_PIN|TERMINAL_SW_PIN  );
  PMU_INTConfig(PMU_INT_IOAEN, ENABLE);
  CORTEX_SetPriority_ClearPending_EnableIRQ(PMU_IRQn, 0);
   #ifdef DEBUG_LOW_POWER
  uart_debug_byte ('%',1);
#endif
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////





/*************************************************************
**************************************************************
**      Function Name  : S_GpioControlRelay                 **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 15/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): uint8_t  &&   uint8_t              **
**      Return value   : None                               **
**      Description    : Function to Control the Relay      **
**************************************************************
*************************************************************/
void S_GpioControlRelay(uint8_t RelayNumber, uint8_t ControlAction)
{
  uint32_t loop_index = 0;
  uint16_t ControlPin;
  switch(RelayNumber)
  {
  case Relay0:
    if(ControlAction == Connect)
    {
#ifdef MTR_INDIRECT
      ControlPin =SRELAY_0_CONNECT;
#else
      ControlPin = RELAY_CONNECT;
#endif
    }
    else
    {
#ifdef MTR_INDIRECT
      ControlPin = RELAY_0_DISCONNECT;
#else
      ControlPin = RELAY_DISCONNECT;
#endif
    }
    break;
  }
#ifdef MTR_INDIRECT
  S_GPIO_Set_Bits(SRELAY_PORT_OUT, ControlPin);
  loop_index = 6*14563*625/1000;//40ms *.625 = 25msec as //1*14563===6.75m
  S_GPIO_Clear_Bits(SRELAY_PORT_OUT, ControlPin);
#else
  S_GPIO_Set_Bits(RELAY_PORT,ControlPin);
#ifdef MTR_THREE_PH
  loop_index = 6*14563*625/1000;//40ms *.625 = 25msec as //1*14563===6.75m
#else
  loop_index = 3*14563*83/100;//30 msec*83/100 =25msec
#endif
  while(loop_index != 0)
    loop_index--;
  S_GPIO_Clear_Bits(RELAY_PORT,ControlPin);
#endif


  if(ControlAction == Connect)
  {
	  Glob_U8_RelayStatus |= (0x01 << RelayNumber);                       //edit_4_by_AK change the name of var
  }
  else if(ControlAction == Disconnect)
  {
	  Glob_U8_RelayStatus &= ~(0x01 << RelayNumber);                     //edit_5_by_AK change the name of var
  }
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////




/*************************************************************
**************************************************************
**      Function Name  : S_GPIO_Set_Bits                    **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 22/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): void *  &&   uint16_t              **
**      Return value   : None                               **
**      Description    : Function to set spcific bit        **
**************************************************************
*************************************************************/
void S_GPIO_Set_Bits(void *Port,uint16_t PIN)
{  
  if(Port == S_PORT_A)
  {
    S_GPIOA_SetBits(Port, PIN);
  }
  else if ( (Port == S_PORT_B) || (Port == S_PORT_C) || (Port == S_PORT_D) || (Port == S_PORT_E) || (Port == S_PORT_F) ) 
  {
    S_GPIO_SetBits(Port, PIN);
  }
  else
  {
    /* DO Nothing MSRA_c*/
  }

}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////



/*************************************************************
**************************************************************
**      Function Name  : S_GPIO_Clear_Bits                  **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 22/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): void *  &&   uint16_t              **
**      Return value   : None                               **
**      Description    : Function to Clear spcific  bit     **
**************************************************************
*************************************************************/
void S_GPIO_Clear_Bits(void *Port,uint16_t PIN)
{
  if(Port == S_PORT_A)
  {
    S_GPIOA_ClearBits(Port, PIN);
  }
  else if ( (Port == S_PORT_B) || (Port == S_PORT_C) || (Port == S_PORT_D) || (Port == S_PORT_E) || (Port == S_PORT_F) ) 
  {
    S_GPIO_ClearBits(Port, PIN);
  }
  else
  {
    /* DO Nothing MSRA_c*/
  }

}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////



/*************************************************************
**************************************************************
**      Function Name  : S_GPIO_Get_Bits                    **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 22/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): void *  &&   uint16_t              **
**      Return value   : uint8_t                            **
**      Description    : Function to retrun value of the bit**
**************************************************************
*************************************************************/
uint8_t S_GPIO_Get_Bits(void *Port,uint16_t PIN)
{
  if(Port == S_PORT_A)
  {
    return S_GPIOA_GetBit(Port, PIN);
  }
  else if ( (Port == S_PORT_B) || (Port == S_PORT_C) || (Port == S_PORT_D) || (Port == S_PORT_E) || (Port == S_PORT_F) ) 
  {
    return S_GPIO_GetBit(Port, PIN);
  }
  else
  {
    /* DO Nothing MSRA_c*/
  }

return 0;
}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////



/*************************************************************
**************************************************************
**      Function Name  : S_GPIO_Toggle_Bits                 **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 22/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): void *  &&   uint16_t              **
**      Return value   : None                               **
**      Description    : Function to toggle the bit         **
**************************************************************
*************************************************************/
void S_GPIO_Toggle_Bits(void *Port,uint16_t PIN)
{

     S_GPIO_ToggleBits(Port, PIN);

}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////



/*************************************************************
**************************************************************
**      Function Name  : S_GpioInit_PIN                     **
**      Author         :                                    **
**      Edit by        : Ahmed Mustafa                      **
**      Date           : 22/3/2023                          **
**      Reentrancy     : Reentrant                          **
**      Sync/Async     : Synchronous                        **
**      Parameters (in): void *  &&  uint16_t  &&  uint16_t **
**      Return value   : None                               **
**      Description    : Function to init PIN               **
**************************************************************
*************************************************************/
void S_GpioInit_PIN(void *Port,uint16_t PIN,uint16_t Mode )
{
      GPIO_InitType GPIO_InitStruct;
      
    if(Port == S_PORT_A)
    {
      GPIO_InitStruct.GPIO_Mode = Mode;
      GPIO_InitStruct.GPIO_Pin = PIN;
      GPIOA_Init(Port, &GPIO_InitStruct);
      }


  else if ( (Port == S_PORT_B) || (Port == S_PORT_C) || (Port == S_PORT_D) || (Port == S_PORT_E) || (Port == S_PORT_F) ) 
  {
    GPIO_InitStruct.GPIO_Mode = Mode;
    GPIO_InitStruct.GPIO_Pin = PIN;
    GPIOBToF_Init(Port, &GPIO_InitStruct);
    }

  else
  {
    /* DO Nothing MSRA_c*/
  }

}
/////////////////////////////-_E_-///////////////////////////////////////
/////////////////////////////-_N_-///////////////////////////////////////
/////////////////////////////-_D_-///////////////////////////////////////