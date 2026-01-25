/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : r_demo_config.h
*    @version
*        $Rev: 2585 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2017-01-11 15:58:25 +0900#$
* Description :
******************************************************************************/

#ifndef R_DEMO_CONFIG_H
#define R_DEMO_CONFIG_H

/******************************************************************************
Macro definitions
******************************************************************************/
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
#define R_RFMAC_FREQ_BAND_MAX         (R_RFMAC_FREQ_BAND_920+1)
#define R_RFMAC_FSK_OPEMODE_MAX       (R_RFMAC_FSK_OPEMODE_6)
#define R_RFMAC_FSK_OPEMODE_TABLE     (2u)
#define R_RFMAC_NO_SUPPORT            (int8_t)(-1)
#endif

/******************************************************************************
Typedef definitions
******************************************************************************/

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
typedef struct
{
    uint8_t   macFreqBandId;
    uint8_t   macOpeMode;
    uint8_t   macChannelNumber;
    uint8_t   macFrontendGain;
    uint8_t   phyAntennaSelectTx;
    uint8_t   phyRegulatoryMode;
    uint8_t   phyTransmitPower;
    uint8_t   phyAgcWaitGainOffset;
    uint16_t  phyCcaVth;
    uint8_t   phyCcaVthOffset;
    uint16_t  phyAntennaSwitchEnaTiming;
    uint8_t   phyGpio0Setting;
    uint8_t   phyAntennaSwitchEna;
} r_demo_rf_table_t;
#endif

typedef enum
{
    R_CONFIG_STATUS_SUCCESS                =   0x00,
    R_CONFIG_STATUS_FAILED                 =   0x01,
    R_CONFIG_STATUS_UNSUPPORTED            =   0x02,
} r_demo_config_status_t;

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)

/*!
    \struct r_demo_rf_config_t
    \brief Structure for RF configuration defined in certification test spec
 */
typedef struct
{
    uint8_t              macFrequencyBand_RF;
    uint8_t              macOperatingMode_RF;
    uint8_t              freqHopExt;    /*!< 0x00 indicates no freq hopping, 0x01 indicates freq hopping for future extension */
    uint8_t              chNumber;
} r_demo_rf_config_t;
#endif

/******************************************************************************
Functions prototype
******************************************************************************/
/******************************************************************************
* Function Name     : R_DEMO_GetDemoConfig
* Description       : Function for reading and setting the demo configuration from data non-volatile memory
* Argument          : None
* Return Value      : None
******************************************************************************/
/*!
   \fn void R_DEMO_GetDemoConfig (void);
   \brief Function for reading and setting the demo configuration from data non-volatile memory
 */
void R_DEMO_GetDemoConfig (void);

/******************************************************************************
* Function Name     : R_DEMO_SetIbContent
* Description       : Function for setting the MAC, ADP and EAP information base attributes
* Argument          : None
* Return Value      : None
******************************************************************************/
/*!
   \fn r_result_t R_DEMO_SetIbContent(void);
   \brief Function for setting the MAC, ADP and EAP information base attributes
 */
r_result_t R_DEMO_SetIbContent(void);

/***********************************************************************
* Function Name     : R_DEMO_SetContext0forCert
* Description       : Function for set contextinfo for Certification
* Argument          : The current PANID value
* Return Value      : None
***********************************************************************/
/*!
   \fn r_result_t R_DEMO_SetContext0forCert (uint16_t panId);
   \brief Function for set contextinfo for Certification
 */
r_result_t R_DEMO_SetContext0forCert(uint16_t panId);

/***********************************************************************
* Function Name     : R_DEMO_AppPreserveProcess
* Description       : Function for storing information in non-volatile memory
* Argument          : The current frame counter value
* Return Value      : None
***********************************************************************/
/*!
   \fn r_result_t R_DEMO_AppPreserveProcess (uint32_t framecounter);
   \brief Function for storing information in non-volatile memory
 */
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
r_result_t R_DEMO_AppPreserveProcess (uint32_t framecounter, uint8_t mediaType);
#else
r_result_t R_DEMO_AppPreserveProcess(uint32_t framecounter);
#endif

/***********************************************************************
* Function Name     : R_DEMO_AppPresetProcess
* Description       : Function for restoring information from non-volatile memory
* Argument          : None
* Return Value      : None
***********************************************************************/
/*!
   \fn r_result_t R_DEMO_AppPresetProcess (void);
   \brief Function for restoring information from non-volatile memory
 */
r_result_t R_DEMO_AppPresetProcess (void);


#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
r_demo_config_status_t R_DEMO_RfBandSwitch(r_demo_rf_config_t *pRfConfig);
void R_DEMO_RfContinuousTx();
#endif

r_demo_config_status_t R_DEMO_SetIbForETTDLL353738(r_boolean_t activate);

#endif /* R_DEMO_CONFIG_H */

