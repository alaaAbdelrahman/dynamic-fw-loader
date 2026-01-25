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
 * File Name : r_uif_sap_if.h
 * Description : UART Interface layer Internal API Header
 ******************************************************************************/
#ifndef R_UIF_SAP_IF_H
#define R_UIF_SAP_IF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_UIF_PORT_0                (0u)
#define R_UIF_PORT_1                (1u)
#define R_UIF_COMMAND_LEN_MAX        (400u)


/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    R_UIF_BAUD_9600 = 0,
    R_UIF_BAUD_19200,
    R_UIF_BAUD_38400,
    R_UIF_BAUD_57600,
    R_UIF_BAUD_115200,
    R_UIF_BAUD_230400,
    R_UIF_BAUD_300000,
    R_UIF_BAUD_375000,
    R_UIF_BAUD_460800,
    R_UIF_BAUD_500000,
    R_UIF_BAUD_750000,
    R_UIF_BAUD_937500,
    R_UIF_BAUD_1000000,
    R_UIF_BAUD_MAX,
} r_uif_baud_t;


typedef enum
{
    R_UIF_STATE_UNINIT,
    R_UIF_STATE_IDLE,
    R_UIF_STATE_BUSY
} r_uif_state_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/*===========================================================================*/
/* For UIF SAP API                                                           */
/*===========================================================================*/
r_result_t      R_UIF_Init (uint8_t block_id, r_uif_baud_t baud);
r_result_t      R_UIF_Deinit (uint8_t block_id);
r_result_t      R_UIF_SendCmd (uint8_t dst_block_id, uint8_t src_block_id, uint8_t * pbuffer, uint32_t size, uint8_t reserved);
r_uif_state_t   R_UIF_GetTxState (uint8_t block_id);
void            R_UIF_EventProcess (void);

#endif  /* R_UIF_SAP_IF_H */

