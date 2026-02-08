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
* File Name   : r_demo_parameters.c
*    @version
*        $Rev: 2372 $
*    @last editor
*        $Author: troba $
*    @date
*        $Date:: 2017-04-26 17:49:29 +0200#$
* Description :
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include "r_typedefs.h"
#include "r_c3sap_api.h"
#include "r_demo_app.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/
const r_demo_config_t g_cdemo_config =
{
    R_PLATFORM_TYPE_CPX3,           //r_modem_platform_type_t
    R_BOARD_TYPE_G_CPX3,            //r_modem_board_type_t
    R_G3_BANDPLAN_CENELEC_A,        //r_g3_bandplan_t
    R_ADP_DEVICE_TYPE_PEER,         //r_adp_device_type_t
    R_FALSE,                        //r_g3_route_type_t
    R_FALSE,
    R_TRUE,                         //verboseEnabled
    R_FALSE,                        //macPromiscuousEnabled
    R_DEMO_MODE_CUI,                //appMode
    R_G3MAC_QOS_NORMAL,             //Normal QoS
    R_TRUE,                         //discoverRoute
    0xFF0102FFFE000000,             //Eui64
    0x781D,                         //panId
    0x0000,                         //coordSHort
    {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF}, //PSK
    {0xAF, 0x4D, 0x6D, 0xCC, 0xF1, 0x4D, 0xE7, 0xC1, 0xC4, 0x23, 0x5E, 0x6F, 0xEF, 0x6C, 0x15, 0x1F}, //GMK0
    {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88}, //GMK1
    0,                                                                                                //activeKeyIndex
    {0x00, 0x00, 0x00, 0x00},                                                                         //wait[4]
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,},                                                  //tonemask[9]
    {0x10u,                                                                                           //extIDLength (max 36)
    {0x48, 0x45, 0x4D, 0x53, 0x00, 0x01, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C}} //extID
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    ,R_MEDIA_TYPE0_PLC, /* 0:PLC only 1:RF only 2:Hybrid */
    {.macOperatingMode_RF= R_RFMAC_FSK_OPEMODE_1,.macFrequencyBand_RF=R_RFMAC_FREQ_BAND_863} /* OpeMode:1, EU Band */
    ,R_FALSE, R_FALSE /* plcDisabled rfDisabled */
#endif
};

r_demo_config_t g_demo_config;

