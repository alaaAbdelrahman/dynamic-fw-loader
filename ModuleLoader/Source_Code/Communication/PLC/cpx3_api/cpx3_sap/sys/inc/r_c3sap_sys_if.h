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
 * File Name : r_c3sap_sys_if.h
 * Description : System layer Internal API
 ******************************************************************************/

#ifndef R_C3SAP_SYS_IF_H
#define R_C3SAP_SYS_IF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_sys_sap.h"
#include "r_c3sap_sys_depend.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define GET_I2C0(s)  ((uint8_t)((s) >> 24))
#define GET_I2C1(s)  ((uint8_t)((s) >> 16))
#define GET_I2C2(s)  ((uint8_t)((s) >> 8))
#define GET_I2C3(s)  ((uint8_t)((s) & 0xFFu))
#define GET_S2C0(s)  ((uint8_t)((s) >> 8))
#define GET_S2C1(s)  ((uint8_t)((s) & 0xFFu))

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct{
    uint8_t  dst_block_id;
    uint8_t  src_block_id;
    uint8_t* pbuffer;
    uint32_t size;
}st_sys_recv_cmd_t;

typedef struct {
    uint8_t  dst_block_id;
    uint8_t  src_block_id;
    uint8_t* pbuffer;
    uint32_t size;
    uint8_t  reserved;
}st_sys_send_cmd_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
r_result_t R_SYS_SendCmd (st_sys_send_cmd_t *p_cmd);
r_result_t R_SYS_ReceiveCmd (st_sys_recv_cmd_t *p_cmd);
r_result_t R_SYS_SemaphoreWait (void);
void R_SYS_SemaphoreRelease (void);

#endif /* R_C3SAP_SYS_IF_H */

