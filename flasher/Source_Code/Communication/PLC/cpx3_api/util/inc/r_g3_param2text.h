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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_g3_param2text.h
* Version      : 1.0
* Description  : Convert structure to text for log output
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 01.04.2019 1.00     First Release
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#ifndef R_G3_PARAM2TEXT_H
#define R_G3_PARAM2TEXT_H

/******************************************************************************
Macro definitions
******************************************************************************/
/* PLC MODE */
#define R_STR2TEXT_PLC_MODE_TEST            (0x00000001)    //LML_DATA_REQ(test mode)

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    R_G3_DEF_TYPE_UNIT,
    R_G3_DEF_TYPE_CMD_ID,
    R_G3_DEF_TYPE_CMD_TYPE,
    R_G3_DEF_TYPE_LAYER,
    R_G3_DEF_TYPE_INFO_TYPE,
    R_G3_DEF_TYPE_INFO_LAYER,
    R_G3_DEF_TYPE_MEDIA,
    R_G3_DEF_TYPE_BAND,
    R_G3_DEF_TYPE_ADR_MODE,
    R_G3_DEF_TYPE_KEY_STATE,
    R_G3_DEF_TYPE_KEY_TYPE,
    R_G3_DEF_TYPE_METRIC,
    R_G3_DEF_TYPE_MOD_TYPE,
    R_G3_DEF_TYPE_PMS
} e_g3_def_type_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
uint32_t     R_G3_Param2Text(uint16_t func_id, void *p_str_in, char* p_out_text);
uint32_t     R_G3_LogInfo2Text( uint8_t layer, uint8_t *log, char* pout_text);
const char * R_G3_Stats2Text(uint8_t layer, uint32_t index);
uint32_t     R_G3_FuncId2Text( uint16_t funcId, char* pout_text);
const char * R_G3_Status2Text(uint8_t unit_id, uint8_t status);
const char * R_G3_Ibid2Text(uint8_t layer, uint16_t id, uint8_t * len);
const char * R_G3_Def2text(e_g3_def_type_t type, uint16_t id);

#endif /* R_G3_PARAM2TEXT_H */


