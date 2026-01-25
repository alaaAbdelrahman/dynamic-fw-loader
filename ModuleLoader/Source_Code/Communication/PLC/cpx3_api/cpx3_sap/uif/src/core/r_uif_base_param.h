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
* File Name   : r_uif_base_param.h
*    @version
*        $Rev: 9315 $
*    @last editor
*        $Author: a5034286 $
*    @date
*        $Date:: 2022-03-23 10:41:46 +0900#$
* Description :
******************************************************************************/

#ifndef R_UIF_BASE_PARAM_H
#define R_UIF_BASE_PARAM_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define GET_FREE_LEN(w, r, len)          (((w) < (r)) ? (uint16_t)((r) - (w) - 1) : (uint16_t)((r) + (len) - ((w) - 1)))  /* PRQA S 3435 */
#define GET_VALID_LEN(w, r, len)         (((w) >= (r)) ? (uint16_t)((w) - (r)) : (uint16_t)((w) + (len) - (r)))           /* PRQA S 3435 */
#define GET_VALID_LEN_linear(w, r, len)  (((w) >= (r)) ? (uint16_t)((w) - (r)) : (uint16_t)((len) - (r)))                 /* PRQA S 3435 */
#define FWD_PTR(x, y, len)               ((((x) + (y)) >= (len)) ? (uint16_t)((x) + (y) - (len)) : (uint16_t)((x) + (y))) /* PRQA S 3435 */

#define ALIGN_4BYTE(s)                   (((r_uintptr_t)(s) + 3u) & (r_uintptr_t)(~3ull))

/*===========================================================================*/
/* PARAM declared                                                            */
/*===========================================================================*/

#define R_UIF_CORE_STATIC_BUFF_NUM       (2u)

#define ZERO                             (0u)

#define R_UIF_TX_UART_TTL_MARGIN_MSEC    (500u)
#define R_UIF_TX_UART_TTL_BUSYWAIT_MSEC  (1000u)
#define R_UIF_RX_UART_TTL_MSEC           (10000u)

#define R_UIF_COMMAND_LEN_MIN            (1u)
#define R_UIF_TX_COMMAND_TTL_SEC         (5u)
#define R_UIF_RX_COMMAND_TTL_SEC         (10u)

#define R_UIF_AVOID_COMPILE_ERROR        (4u)

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#endif /* R_UIF_BASE_PARAM_H */

