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
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
 * File Name : r_c3sap_sys_depend.h
 * Description : System layer Local Header (Environment dependent part)
 ******************************************************************************/

#ifndef R_C3SAP_SYS_DEPEND_H
#define R_C3SAP_SYS_DEPEND_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_sys_api.h"
/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/* Block ID */
typedef enum
{
    R_C3SAP_BLOCKID_SYSTEM,
    R_C3SAP_BLOCKID_G3,
    R_C3SAP_BLOCKID_PRIME,
    R_C3SAP_BLOCKID_UARTIF_0,
    R_C3SAP_BLOCKID_UARTIF_1,
    R_C3SAP_BLOCKID_MAX
} r_c3sap_blockid_t;

/* Event Type */
typedef enum
{
    R_SYS_EVENT_CRC_ERROR = 1,
    R_SYS_EVENT_OS_ERROR,
    R_SYS_EVENT_DRV_ERROR,
    R_SYS_EVENT_DSP_ERROR,
    R_SYS_EVENT_BLOCK_ERROR,
    R_SYS_EVENT_SEC_ERROR,
    R_SYS_EVENT_MAX
} r_sys_event_t;


/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
void R_SYS_TimeOutEvent (uint8_t handle);
uint32_t R_SYS_GetDirectBaud (uint8_t baud);
r_result_t R_SYS_GetSystemTimeMsec (uint32_t * psys_time);


#endif /* R_C3SAP_SYS_DEPEND_H */

