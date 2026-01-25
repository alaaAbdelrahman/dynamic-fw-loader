#include "config.h"
#if (MicroController == Micro_V85XX)
/**
  ******************************************************************************
  * @file    v85xx_iso7816.c 
  * @author  VT Application Team
  * @version V1.0.0
  * @date    2017-06-22
  * @brief   ISO7816 library.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "v85xx_iso7816.h"
#include "v85xx_clk.h"
#include "v85xx_gpio.h"
#include "v85xx_pmu.h"
#include "v85xx_cortex.h"
#include "Driver_ISO7816.h"

//registers default reset values
#define ISO7816_BAUDDIVL_RSTValue   0
#define ISO7816_BAUDDIVH_RSTValue   0
#define ISO7816_CFG_RSTValue        0
#define ISO7816_CLK_RSTValue        0

#define ISO7816_INFO_RC_MASK    (0xECUL) //R/C
#define ISO7816_INFO_RW_MASK    (0x13UL) //R/W

#define DEFAULT_BAUDRATE 8809
ARM_DRIVER_ISO7816  *Drv_ISO7816[MAX_ISO7816_IFC] = {&Driver_ISO78160, &Driver_ISO78161};
IRQn_Type ISO7816_IRQn[MAX_ISO7816_IFC] = {ISO78160_IRQn, ISO78161_IRQn};
uint32_t BAUDRATE[MAX_ISO7816_IFC]={DEFAULT_BAUDRATE, DEFAULT_BAUDRATE};

/**
  * @brief  ISO7816 initialization.
  * @param  ISO7816x: ISO78160~ISO78161
            Init_Struct:iso7816 configuration.
                FirstBit:
                    ISO7816_FIRSTBIT_LSB
                    ISO7816_FIRSTBIT_MSB
                ACKLen:
                    ISO7816_ACKLEN_1
                    ISO7816_ACKLEN_2
                Parity:
                    ISO7816_PARITY_EVEN
                    ISO7816_PARITY_ODD
                Baudrate: Baud rate value
  * @retval None
  */
void ISO7816_Init(ISO7816_InitType *Init_Struct){
	uint32_t ctrl_bits = 0;
	///ctrl_bits = Init_Struct->Com_Mode;
	ctrl_bits|=(uint32_t)Init_Struct->Com_Mode;
	ctrl_bits|=(uint32_t)Init_Struct->FirstBit;
	ctrl_bits|=(uint32_t)Init_Struct->ACKLen;
	ctrl_bits|=(uint32_t)Init_Struct->AutoRX;
	ctrl_bits|=(uint32_t)Init_Struct->RXTrials;
	ctrl_bits|=(uint32_t)Init_Struct->AutoTX;
	ctrl_bits|=(uint32_t)Init_Struct->TXTrials;
	ctrl_bits|=(uint32_t)Init_Struct->Parity;
	switch(Init_Struct->IFC){
		case 0:
		case 1:
      Drv_ISO7816[Init_Struct->IFC]->Initialize(Init_Struct->event_cb);
      Drv_ISO7816[Init_Struct->IFC]->Control(ctrl_bits, Init_Struct->Baudrate);
      #if (MicroController == Micro_V94XX)
      Drv_ISO7816[Init_Struct->IFC]->Control(ARM_ISO7816_SET_CLKDIV, ARM_ISO7816_APBD_4);
      #else  
      Drv_ISO7816[Init_Struct->IFC]->Control(ARM_ISO7816_SET_CLKDIV, ARM_ISO7816_APBD_2);
      #endif
      Drv_ISO7816[Init_Struct->IFC]->Control(ARM_ISO7816_SET_CLKOUTPUT, ENABLE);
      Drv_ISO7816[Init_Struct->IFC]->Control(ARM_ISO7816_CMD_CONTROL, ENABLE);
      //PMU_INTConfig(PMU_APB_ISO78160, ENABLE);
      CORTEX_SetPriority_ClearPending_EnableIRQ(ISO7816_IRQn[Init_Struct->IFC], 0);
			break;
		default :
			break;
	}
}

/**
  * @brief  Fills each InitStruct member with its default value.
  * @param  InitStruct: pointer to an ISO7816_InitType structure which will be initialized.
  * @retval None
  */

void ISO7816_StructInit(ISO7816_InitType *InitStruct, uint8_t ifc, ARM_ISO7816_SignalEvent_t event_cb)
{
  /*--------------- Reset ISO7816 init structure parameters values ---------------*/
  /* Initialize the ACKLen member */ 
  InitStruct->IFC = ifc;
  InitStruct->Com_Mode = ARM_ISO7816_MODE_HALFDUPLEX;
  InitStruct->ACKLen = ARM_ISO7816_ACKLEN_1;
  /* Initialize the Baudrate member */ 
  InitStruct->Baudrate = BAUDRATE[ifc];/*11745;*/ //9000;//8809;//35235;
  /* Initialize the FirstBit member */ 
  InitStruct->FirstBit = ARM_ISO7816_LSB_MSB;
  /* Initialize the Parity member */ 
  InitStruct->Parity =  ARM_ISO7816_PARITY_EVEN;
  InitStruct->AutoRX = ARM_ISO7816_AUTO_RERX_0;
   InitStruct->RXTrials = ARM_ISO7816_RX_RETRY_3;
  InitStruct->AutoTX = ARM_ISO7816_AUTO_RETX_0;
  InitStruct->TXTrials = ARM_ISO7816_TX_RETRY_2;
  InitStruct->event_cb = event_cb;
}


/**
  * @brief  Deinitializes the ISO7816 peripheral registers to their default reset 
            values.
  * @param  ISO7816x: ISO78160~ISO78161
  * @retval None
  */

void ISO7816_DeInit(ISO7816_InitType *InitStruct)
{
	switch(InitStruct->IFC){
		case 0 :
		case 1:
      	CORTEX_NVIC_DisableIRQ(ISO7816_IRQn[InitStruct->IFC]);
     		Drv_ISO7816[InitStruct->IFC]->Uninitialize();
     		BAUDRATE[InitStruct->IFC]=DEFAULT_BAUDRATE;
      break;
		default:
			break;
	}
	memset(InitStruct, 0, sizeof(InitStruct));
}

/**
  * @brief  ISO7816 enable control.
  * @param  ISO7816x: ISO78160~ISO78161 
            NewState:
                ENABLE
                DISABLE
  * @retval None.
  */
void ISO7816_Cmd(ISO7816_TypeDef *ISO7816x, uint32_t NewState)
{
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x));
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  if (NewState == ENABLE)
  {
    ISO7816x->CFG |= ISO7816_CFG_EN;
  }
  else
  {
    ISO7816x->CFG &= ~ISO7816_CFG_EN;
  }
}

/**
  * @brief  ISO7816 Baudrate control.
  * @param  ISO7816x: ISO78160~ISO78161
            BaudRate:
  * @retval None
  */
uint8_t ISO7816_BaudrateConfig(uint8_t ifc, uint32_t baudrate)
{
  uint8_t res = 0;
  switch (ifc){
	case 0:
	case 1:
		res = (Drv_ISO7816[ifc]->Control(ARM_ISO7816_CMD_CONTROL, DISABLE) == ARM_DRIVER_OK ? 1:0);
		if (res)
			res = (Drv_ISO7816[ifc]->Control(ARM_ISO7816_SET_BAUDRATE, baudrate) == ARM_DRIVER_OK ? 1:0);
		if (res)
			res = (Drv_ISO7816[ifc]->Control(ARM_ISO7816_CMD_CONTROL, ENABLE)== ARM_DRIVER_OK ? 1:0);
    		if(res)
			BAUDRATE[ifc]=baudrate;
		break;
	default:
		break;
	
  }
  return res;
}

uint8_t ISO7816_GetBaudrate(uint8_t ifc, uint32_t* baudrate){
  uint8_t ret = 0;
  if(ifc < MAX_ISO7816_IFC && baudrate != NULL){
    *baudrate=BAUDRATE[ifc];
    ret = 1;
  }
  return ret;
}


/**
  * @brief  ISO7816 clock divider configure.
  * @param  ISO7816x: ISO78160~ISO78161
            Prescaler:1~128
  * @retval None
  */
void ISO7816_CLKDIVConfig(ISO7816_TypeDef *ISO7816x, uint32_t Prescaler)
{
  uint32_t tmp;
  
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x));
  assert_param(IS_ISO7816_PRESCALER(Prescaler));  
  
  tmp = ISO7816x->CLK;
  tmp &= ~ISO7816_CLK_CLKDIV;
  tmp |= ((Prescaler - 1) & ISO7816_CLK_CLKDIV);
  ISO7816x->CLK = tmp;
}

/**
  * @brief  ISO7816 clock output enable control.
  * @param  ISO7816x: ISO78160~ISO78161
            NewState:
                ENABLE
                DISABLE
  * @retval None
  */
void ISO7816_CLKOutputCmd(ISO7816_TypeDef *ISO7816x, uint32_t NewState)
{
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    ISO7816x->CLK |= ISO7816_CLK_CLKEN;
  }
  else
  {
    ISO7816x->CLK &= ~ISO7816_CLK_CLKEN;
  }
}

/**
  * @brief  Read data.
  * @param  ISO7816: ISO78160~ISO78161
  * @retval The received data.
  */


uint8_t ISO7816_ReceiveData(uint8_t ifc, uint8_t *data)
{
	uint8_t ret = 0;
	switch(ifc){
		case 0:
		case 1:
			ret = Drv_ISO7816[ifc]->Receive(data, 255);
			if(ret == ARM_DRIVER_OK){
				ret = 1;
			}
			break;
		default:
			break;
	}
	return ret;
}


uint8_t ISO7816_TransmitData(uint8_t ifc, uint8_t *data, uint32_t len){
	uint8_t ret = 0;
	switch(ifc){
		case 0:
		case 1:
			ret = Drv_ISO7816[ifc]->Send((const void*)data, len);
			if(ret == ARM_DRIVER_OK){
				ret = 1;
			}
			break;
		default:
			break;
	}
	return ret;
}

uint8_t ISO7816_AbortOperation(uint8_t ifc){
	uint8_t ret = 0;
	switch(ifc){
		case 0:
		case 1:
			ret = Drv_ISO7816[ifc]->Control(ARM_ISO7816_ABORT_RECEIVE, 0);
			ret = Drv_ISO7816[ifc]->Control(ARM_ISO7816_ABORT_TRANSMIT, 0);
			if(ret == ARM_DRIVER_OK){
				ret = 1;
			}
			break;
		default:
			break;
	}
	return ret;

}

uint32_t ISO7816_GetRxCount(uint8_t ifc){
	uint8_t ret = 0;
	switch(ifc){
		case 0:
    	case 1:
			ret = Drv_ISO7816[ifc]->GetRxCount();
			break;
		default:
			break;
	}
	return ret;
}
uint32_t ISO7816_GetTxCount(uint8_t ifc){
	uint8_t ret = 0;
	switch(ifc){
		case 0:
    	case 1:
			ret = Drv_ISO7816[ifc]->GetTxCount();
			break;
		default:
			break;
	}
	return ret;
}



/**
  * @brief  Write data.
  * @param  ISO7816x: ISO78160~ISO78161
  * @retval None
  */
void ISO7816_SendData(ISO7816_TypeDef *ISO7816x, uint8_t ch)
{
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x));
  
  ISO7816x->DATA = ch;
}

/**
  * @brief  Interrupt configure.
  * @param  ISO7816x: ISO78160~ISO78161
            INTMask:
                This parameter can be any combination of the following values
                ISO7816_INT_RXOV
                ISO7816_INT_RX
                ISO7816_INT_TX
            NewState:
                ENABLE
                DISABLE
  * @retval None
  */
void ISO7816_INTConfig(ISO7816_TypeDef *ISO7816x, uint32_t INTMask, uint8_t NewState)
{
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x));
  assert_param(IS_ISO7816_INT(INTMask));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if (NewState == ENABLE)
  {
    ISO7816x->CFG |= INTMask;
  }
  else
  {
    ISO7816x->CFG &= ~INTMask;
  }
}

/**
  * @brief  Get interrupt state
  * @param  ISO7816x: ISO78160~ISO78161
            INTMask:
                ISO7816_INTSTS_RXOV
                ISO7816_INTSTS_RX
                ISO7816_INTSTS_TX
  * @retval 1: state set
            0: state reset
  */
uint8_t ISO7816_GetINTStatus(ISO7816_TypeDef *ISO7816x, uint32_t INTMask)
{
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x));
  assert_param(IS_ISO7816_INTFLAGR(INTMask));
  
  if (ISO7816x->INFO & INTMask)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Clear interrupt state.
  * @param  ISO7816x: ISO78160~ISO78161
            INTMask:
                This parameter can be any combination of the following values
                ISO7816_INTSTS_RXOV
                ISO7816_INTSTS_RX
                ISO7816_INTSTS_TX
  * @retval None
  */
void ISO7816_ClearINTStatus(ISO7816_TypeDef *ISO7816x, uint32_t INTMask)
{
    uint32_t tmp;
    
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x)); 
  assert_param(IS_ISO7816_INTFLAGC(INTMask));
  
  tmp = ISO7816x->INFO;
  tmp &= ~ISO7816_INFO_RC_MASK;
  tmp |= INTMask;
  ISO7816x->INFO = tmp;
}

/**
  * @brief  Get peripheral flag.
  * @param  ISO7816x: ISO78160~ISO78161
            FlagMask:
                ISO7816_FLAG_SDERR
                ISO7816_FLAG_RCERR
  * @retval 1: state set
            0: state reset
  */
uint8_t ISO7816_GetFlag(ISO7816_TypeDef *ISO7816x, uint32_t FlagMask)
{
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x));
  assert_param(IS_ISO7816_FLAGR(FlagMask));  
  
  if (ISO7816x->INFO&FlagMask)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
  * @brief  Clear peripheral flag.
  * @param  ISO7816x: ISO78160~ISO78161
            FlagMask:
                This parameter can be any combination of the following values
                ISO7816_FLAG_SDERR
                ISO7816_FLAG_RCERR
  * @retval None
  */
void ISO7816_ClearFlag(ISO7816_TypeDef *ISO7816x, uint32_t FlagMask)
{
  uint32_t tmp;
  
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x)); 
  assert_param(IS_ISO7816_FLAGC(FlagMask));  
  
  tmp = ISO7816x->INFO;
  tmp &= ~ISO7816_INFO_RC_MASK;
  tmp |= FlagMask;
  ISO7816x->INFO = tmp;
}

/**
  * @brief  Get last transmit ACK.
  * @param  ISO7816: ISO78160~ISO78161
  * @retval ACK value
  */
uint8_t ISO7816_GetLastTransmitACK(ISO7816_TypeDef *ISO7816x)
{
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x));
  
  if (ISO7816x->INFO&ISO7816_INFO_RCACK)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
            
/**
  * @brief  Get last receive check sum bit.
  * @param  ISO7816: ISO78160~ISO78161
  * @retval CHKSUM bit value
  */
uint8_t ISO7816_GetLastReceiveCHKSUM(ISO7816_TypeDef *ISO7816x)
{
  /* Check parameters */
  assert_param(IS_ISO7816_ALL_INSTANCE(ISO7816x));
  
  if (ISO7816x->INFO&ISO7816_INFO_CHKSUM)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

#endif /* MicroController */

/******************* (C) COPYRIGHT Vango Technologies, Inc *****END OF FILE****/
