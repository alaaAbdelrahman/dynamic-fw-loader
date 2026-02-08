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
* Copyright (C) 2018-2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_sys_param2text.c
* Version      : 1.0
* Description  : Convert structure to text for log output
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 13.07.2018 1.00     First Release
******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

#include <stdio.h>
#include "r_typedefs.h"
#include "r_sys_sap.h"
#include "r_sys_api.h"
#include "r_cmn_param2text.h"
#include "r_sys_param2text.h"
#include "r_g3_param2text.h"
#include "r_sys_cmd.h"
#include "r_uif_statistics.h"
#ifdef R_DEFINE_ENABLE_MEMRW
#include "r_sys_cmd_closed.h"
#endif
/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    R_UIF_LOG_ID_REQ,
    R_UIF_LOG_ID_REQ_ERROR,
    R_UIF_LOG_ID_RCV,
    R_UIF_LOG_ID_RCV_RES,
}r_uif_log_id_t;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static const char *uif_param2text_stats(uint32_t index);
static uint32_t uif_param2text_loginfo(uint32_t value, char* p_out_text);
static const char *sys_param2text_info_block(r_sys_info_block_t info_block);
static const char *sys_status2text(uint8_t status);
static uint32_t sys_func_id2text( uint16_t funcId, char* pout_text);
static const char *sys_cmd_type2text(uint8_t type);
static const char *sys_cmdid2text(uint8_t unit, uint8_t cmdId);
static const char *sys_cmdid2text_sys(uint8_t cmdId);


/******************************************************************************
* Function Name : R_SYS_Stats2Text
* Description   : 
* Arguments     :
* Return Value  :
******************************************************************************/
const char *R_SYS_Stats2Text(uint8_t blockid, uint32_t index)
{
    switch(blockid)
    {
        case R_SYS_INFO_BLOCK_UARTIF_0:
        case R_SYS_INFO_BLOCK_UARTIF_1:
            return uif_param2text_stats(index);
        default:
            return  NULL;
    }

}

/******************************************************************************
* Function Name : R_SYS_LogInfo2Text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
uint32_t R_SYS_LogInfo2Text(uint8_t blockid, uint32_t value, char* p_out_text)
{
    switch(blockid)
    {
        case R_SYS_INFO_BLOCK_UARTIF_0:
        case R_SYS_INFO_BLOCK_UARTIF_1:
            return uif_param2text_loginfo(value,p_out_text);
        default:
            return  sprintf(p_out_text, "0x%08X", value);
    }
}
/******************************************************************************
   End of function  R_SYS_LogInfo2Text
******************************************************************************/

/******************************************************************************
* Function Name : uif_param2text_stats
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *uif_param2text_stats(uint32_t index)
{
    const char *p_ret;
    
    switch (index)
    {
        STATS_TXT(UIF,RX_IN_CRC_ERROR)                 // @suppress("Switch statement")
        STATS_TXT(UIF,RX_IN_SYNC_BREAK)                // @suppress("Switch statement")
        STATS_TXT(UIF,RX_IN_TIMEOUT)                   // @suppress("Switch statement")
        STATS_TXT(UIF,RX_IN_LENGTH_ERROR)              // @suppress("Switch statement")
        STATS_TXT(UIF,RX_IN_BUFF_OVERFLOW)             // @suppress("Switch statement")
        STATS_TXT(UIF,RX_IN_SECURITY_SUCCESS)          // @suppress("Switch statement")
        STATS_TXT(UIF,RX_IN_SECURITY_ERROR)            // @suppress("Switch statement")
        STATS_TXT(UIF,RX_IN_DISCARD)                   // @suppress("Switch statement")
        STATS_TXT(UIF,RX_OUT_COMMAND_REQD)             // @suppress("Switch statement")
        STATS_TXT(UIF,RX_OUT_COMMAND_REQD_ERROR)       // @suppress("Switch statement")
        STATS_TXT(UIF,RX_OUT_COMMAND_SUCCESS)          // @suppress("Switch statement")
        STATS_TXT(UIF,TX_IN_RCV_COMMAND)               // @suppress("Switch statement")
        STATS_TXT(UIF,TX_IN_RCV_ERROR)                 // @suppress("Switch statement")
        STATS_TXT(UIF,TX_IN_SECURITY_SUCCESS)          // @suppress("Switch statement")
        STATS_TXT(UIF,TX_IN_SECURITY_ERROR)            // @suppress("Switch statement")
        STATS_TXT(UIF,TX_OUT_COMMAND_REQD)             // @suppress("Switch statement")
        STATS_TXT(UIF,TX_OUT_COMMAND_SUCCESS)          // @suppress("Switch statement")
        STATS_TXT(UIF,TX_OUT_TIMEOUT)                  // @suppress("Switch statement")
        STATS_TXT(UIF,TX_DRV_ERROR)                    // @suppress("Switch statement")
        default:
        {
            p_ret = NULL;
        }
        break;
    }
    
    return p_ret;
}



/******************************************************************************
* Function Name : uif_param2text_loginfo
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static uint32_t uif_param2text_loginfo(uint32_t value, char* p_out_text)
{
    uint16_t length=0;
    uint8_t port_id = (uint8_t)((value>>30)&0x03u);
    uint8_t layer   = (uint8_t)((value>>28)&0x03u);
    uint8_t blk_id  = (uint8_t)((value>>24)&0x0Fu);
    uint8_t log_id  = (uint8_t)((value>>16)&0xFFu);
    uint16_t param  = (uint16_t)( value    &0xFFFFu);
    
    if (R_UIF_LOG_ID_REQ_ERROR == log_id)
    {
        length += sprintf(&p_out_text[length], "%s cmd=0x%02X", sys_status2text(param&0xFFu),(param>>4)&0xFFu);
    }
    else if (R_UIF_LOG_ID_RCV_RES == log_id)
    {
        length += sprintf(&p_out_text[length], "Res ");
    }
    else
    {
        if (R_SYS_INFO_BLOCK_SYSTEM == blk_id)
        {
            length += sys_func_id2text((param & 0x7FFF), &p_out_text[length]) ;
        }
        else
        {
            length += R_G3_FuncId2Text((param & 0x7FFF), &p_out_text[length]);
        }
    }
    
    return length;
}
/******************************************************************************
   End of function  st2text_loginfo_uif
******************************************************************************/

/******************************************************************************
* Function Name:str2text_sys_info_block
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static const char *sys_param2text_info_block(e_sys_info_block_t info_block)
{
    const char *p_ret;
    
    switch (info_block)
    {
        INFO_BLOCK_CASE_R(SYS, SYSTEM)       // @suppress("Switch statement")    // @suppress("Case statement braces")
        INFO_BLOCK_CASE_R(SYS, UARTIF_0)     // @suppress("Switch statement")
        INFO_BLOCK_CASE_R(SYS, UARTIF_1)     // @suppress("Switch statement")
//      INFO_BLOCK_CASE_R(SYS, A2DIF)        // @suppress("Switch statement")
//      INFO_BLOCK_CASE_R(SYS, FRWKA)        // @suppress("Switch statement")
        INFO_BLOCK_CASE_R(SYS, MAX)          // @suppress("Switch statement")
        default:
        {
            p_ret = "FATAL UNDEFINED ERROR";
        }
        break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  sys_param2text_info_block
******************************************************************************/


/******************************************************************************
* Function Name:sys_status2text
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static const char *sys_status2text(uint8_t status)
{
    const char *p_ret;
    
    switch (status)
    {
        STATUS_CASE_R(SYS,SUCCESS)                  // @suppress("Switch statement")    // @suppress("Case statement braces")
        STATUS_CASE_R(SYS,FAILURE)                  // @suppress("Switch statement")
        STATUS_CASE_R(SYS,INVALID_STATUS)           // @suppress("Switch statement")
        STATUS_CASE_R(SYS,INVALID_PARAMETER)        // @suppress("Switch statement")
        STATUS_CASE_R(SYS,TIMEOUT)                  // @suppress("Switch statement")
//        STATUS_CASE_R(SYS,BUSY)                     // @suppress("Switch statement")
        
        STATUS_CASE_R(SYS,INSUFFICIENT_MEMSIZE)     // @suppress("Switch statement")
        STATUS_CASE_R(SYS,DRV_ERROR)                // @suppress("Switch statement")
        STATUS_CASE_R(SYS,OUT_OF_CAPS)              // @suppress("Switch statement")
        STATUS_CASE_R(SYS,CRC_ERROR)                // @suppress("Switch statement")
        STATUS_CASE_R(SYS,SEC_ERROR)                // @suppress("Switch statement")
        default:
        {
            p_ret = "FATAL UNDEFINED ERROR";
        }
        break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  sys_status2text
******************************************************************************/

/******************************************************************************
* Function Name : sys_func_id2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static uint32_t sys_func_id2text( uint16_t funcId, char* pout_text)
{
    uint32_t length;
    uint8_t type = R_GET_IDA(funcId);
    uint8_t unit = R_GET_UNITID(funcId);
    uint8_t cmdId = R_GET_CMD_ID(funcId);
    
    length   = sprintf(pout_text, "%s ", sys_cmd_type2text(type) );
    length  += sprintf(&pout_text[length], "%s", sys_cmdid2text(unit,cmdId) );
    
    return length;
}

/******************************************************************************
* Function Name : sys_cmd_type2text
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *sys_cmd_type2text(uint8_t type)
{
    const char *p_ret;

    switch( type )
    {
    ENUM_CASE(R_SYS_PKT_IDA, REQUEST )
    ENUM_CASE(R_SYS_PKT_IDA, CONFIRM )
    ENUM_CASE(R_SYS_PKT_IDA, INDICATION )
    default:
            p_ret = "Unknown Type";
        break;
    }
    return p_ret;
}
/******************************************************************************
   End of function  sys_cmd_type2text
******************************************************************************/

/******************************************************************************
* Function Name : sys_cmdid2text_sys
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *sys_cmdid2text(uint8_t unit, uint8_t cmdId)
{
    const char *p_ret;
    
    switch(unit)
    {
        case R_SYS_PKT_IDP_SYS:
        {
            p_ret = sys_cmdid2text_sys(cmdId);
        }
        break;
        default:
        {
            p_ret = "Unknown Unit";
        }
    }
    
    return p_ret;
}

/******************************************************************************
* Function Name : sys_cmdid2text_sys
* Description   : 
* Arguments     : 
* Return Value  : 
******************************************************************************/
static const char *sys_cmdid2text_sys(uint8_t cmdId)
{
    const char *p_ret;

    switch( cmdId )
    {
    ENUM_CASE(R_SYS_PKT_CMD, PING  )
    ENUM_CASE(R_SYS_PKT_CMD, VERSION )
    ENUM_CASE(R_SYS_PKT_CMD, RESET )
    ENUM_CASE(R_SYS_PKT_CMD, BOOTUP )
    ENUM_CASE(R_SYS_PKT_CMD, EVENT )
    ENUM_CASE(R_SYS_PKT_CMD, REBOOTREQ )
    ENUM_CASE(R_SYS_PKT_CMD, CLEARINFO )
    ENUM_CASE(R_SYS_PKT_CMD, GETINFO )
    ENUM_CASE(R_SYS_PKT_CMD, SROMREAD )
    ENUM_CASE(R_SYS_PKT_CMD, SROMWRITE )
    ENUM_CASE(R_SYS_PKT_CMD, SROMERASE )
    default:
        p_ret = "Unknown Type";
        break;
    }
    
    return p_ret;
}
/******************************************************************************
   End of function  sys_cmdid2text_sys
******************************************************************************/





