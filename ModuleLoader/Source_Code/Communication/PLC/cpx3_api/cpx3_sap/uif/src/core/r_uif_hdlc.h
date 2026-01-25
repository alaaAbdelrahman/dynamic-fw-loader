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
* File Name   : r_uif_hdlc.h
*    @version
*        $Rev: 7601 $
*    @last editor
*        $Author: a5034286 $
*    @date
*        $Date:: 2021-10-14 09:50:11 +0900#$
* Description :
******************************************************************************/

#ifndef R_UIF_HDLC_H
#define R_UIF_HDLC_H
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_base_param.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_UIF_HDLC_SCODE               (0x7Eu)

#define R_UIF_HDLC_ECODE_H             (0x7Du)
#define R_UIF_HDLC_ECODE_LE            (0x5Eu)
#define R_UIF_HDLC_ECODE_LD            (0x5Du)

#define R_UIF_HDLC_EDECODE_E           (0x7Eu)
#define R_UIF_HDLC_EDECODE_D           (0x7Du)

#define R_UIF_HDLC_HEADER_FIX_BIT      (1u)

#define R_UIF_HDLC_HEADER_LEN          (2u + 2u + 4u)
#define R_UIF_HDLC_HEADER_LEN_WOSCODE  (2u + 4u)
#define R_UIF_HDLC_PACK_MIN            (R_UIF_HDLC_HEADER_LEN + (R_UIF_COMMAND_LEN_MIN))
#define R_UIF_HDLC_PACK_WOESC_MIN      (R_UIF_HDLC_HEADER_LEN + (R_UIF_COMMAND_LEN_MIN))

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef struct
{
    uint8_t fixBit    : 1;
    uint8_t reserve   : 6;
    uint8_t secFlg    : 1;
    uint8_t type;
} r_uif_hdlc_header_t;

typedef struct
{
    uint8_t crc[4];
} r_uif_hdlc_footer_t;

typedef struct
{
    uint8_t             scode_s;
    r_uif_hdlc_header_t head;
    uint8_t             payload[R_UIF_AVOID_COMPILE_ERROR];
    r_uif_hdlc_footer_t foot;
    uint8_t             scode_e;
} r_uif_hdlc_pack_t;

typedef struct
{
    uint8_t scode_s;
    uint8_t reserve;
    uint8_t type;
    uint8_t payload;
    uint8_t crc[4];
    uint8_t scode_e;
} r_uif_hdlc_t;

typedef struct
{
    uint8_t reserve;
    uint8_t type;
    uint8_t payload[R_UIF_AVOID_COMPILE_ERROR];
} t_r_uif_command_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_UIF_HDLC_H */

