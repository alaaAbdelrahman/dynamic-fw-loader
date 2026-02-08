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
* File Name    : r_cmn_param2text.h
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
#ifndef R_PARAM2TEXT_CMN_H
#define R_PARAM2TEXT_CMN_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define mk_string(x)    #x
#define STATUS_CASE_R(id,msg)       case R_ ## id ## _STATUS_ ## msg     : p_ret = mk_string(id ## _ ## msg) ; break;
#define INFO_BLOCK_CASE_R(id,msg)   case R_ ## id ## _INFO_BLOCK_ ## msg : p_ret = # msg ; break;
#define FUNCID_CASE_R(id,msg)       case R_ ## id ## _FUNCID_ ## msg     : p_ret = mk_string(id ## _ ## msg) ; break;
#define STATS_TXT(id,msg)           case R_ ## id ## _STATS_ ## msg      : p_ret = #msg ; break;
#define STATS_TXT_WL(id,msg)        case R_ ## id ## _STATS_ ## msg      : p_ret = mk_string(id ## _ ## msg); break;
#define ENUM_CASE(base, type)       case base ## _ ## type               : p_ret = #type; break;

#define R_GET_IDA(funcId)                    ( (funcId>>12) & 0x0Ful )
#define R_GET_UNITID(funcId)                 ( ((funcId)>>8) & 0x0Ful )
#define R_GET_CMD_ID(funcId)                 ( (funcId) & 0xFFul )

#define R_STR2TEXT_LEN_MAX (2000u) //TBD

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
uint32_t R_CMN_Param2TextLoopPrint( uint32_t num, uint8_t *p_data, uint8_t space, char* p_out_text );
void     R_CMN_Param2TextSetMode( uint32_t mode );
uint32_t R_CMN_Param2TextGetMode( void );
void     R_CMN_Param2TextClearMode( uint32_t mode );
const char* R_CMN_Param2TextResult(e_result_t result);
uint32_t R_CMN_Param2TextDacimal(uint32_t val, uint32_t dev, uint8_t dig);

#endif /* R_PARAM2TEXT_CMN_H */


