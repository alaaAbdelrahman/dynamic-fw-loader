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
 * File Name : r_sys_cmd.h
 * Description : System Command define
 ******************************************************************************/

#ifndef R_SYS_CMD_H
#define R_SYS_CMD_H

/******************************************************************************
Macro definitions
******************************************************************************/
/* Packet Field Parameters */
/* IDC */
#define R_SYS_PKT_IDC                   (0x0u)

/* IDA */
#define R_SYS_PKT_IDA_REQUEST           (0x0u)
#define R_SYS_PKT_IDA_CONFIRM           (0x1u)
#define R_SYS_PKT_IDA_INDICATION        (0x2u)
#define R_SYS_PKT_IDA_RESERVED          (0x3u)

/* IDP */
#define R_SYS_PKT_IDP_SYS               (0x0u)

/* CMD */
#define R_SYS_PKT_CMD_PING              (0x00u)
#define R_SYS_PKT_CMD_VERSION           (0x01u)
#define R_SYS_PKT_CMD_RESET             (0x02u)
#define R_SYS_PKT_CMD_BOOTUP            (0x03u)
#define R_SYS_PKT_CMD_EVENT             (0x04u)
#define R_SYS_PKT_CMD_REBOOTREQ         (0x05u)
#define R_SYS_PKT_CMD_CLEARINFO         (0x10u)
#define R_SYS_PKT_CMD_GETINFO           (0x11u)
#define R_SYS_PKT_CMD_SROMREAD          (0x20u)
#define R_SYS_PKT_CMD_SROMWRITE         (0x21u)
#define R_SYS_PKT_CMD_SROMERASE         (0x22u)


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#endif /* R_SYS_CMD_H */

