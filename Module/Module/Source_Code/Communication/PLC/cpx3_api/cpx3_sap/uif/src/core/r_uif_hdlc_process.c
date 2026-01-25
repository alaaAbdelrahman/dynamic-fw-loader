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
* File Name   : r_uif_hdlc_process.c
*    @version
*        $Rev: 9082 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2022-03-14 10:22:21 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_hdlc_process.h"
#include "r_byte_swap.h"
#include "r_uif_buff_declared.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

/*===========================================================================*/
/* HDLC Encode Process                                                       */
/*===========================================================================*/

/******************************************************************************
* Function Name:uif_hdlc_enc
* Description :
* Arguments :
* Return Value :
******************************************************************************/
uint16_t uif_hdlc_enc (uint8_t port_id, r_uif_key_t * pKey, uint8_t * pin, uint16_t len, uint8_t type_id, uint8_t * pout, uint8_t secFlg)
{
    uint16_t            i;
    uint16_t            length;
    uint8_t *           psrc;
    uint8_t *           pdst;
    r_uif_hdlc_pack_t * phdlc = (r_uif_hdlc_pack_t *)pout;
    uint8_t             crc[4];
    r_uif_core_buff_t * pcore = uif_get_core_buff_p (port_id);

    if ((pcore == NULL) ||
        ((NULL == pin) || (NULL == pout)) ||
        ((len >= pcore->uif_command_len_max) ||
         (len <= R_UIF_COMMAND_LEN_MIN))
        )
    {
        return 0u;
    }

    if ((NULL == pKey) || (R_FALSE == pKey->validFlg))
    {
        secFlg = R_FALSE;
    }

    R_memset (&phdlc->head, 0, sizeof (r_uif_hdlc_header_t));
    phdlc->scode_s     = R_UIF_HDLC_SCODE;
    phdlc->head.type   = type_id;
    phdlc->head.fixBit = R_UIF_HDLC_HEADER_FIX_BIT;
    phdlc->head.secFlg = (uint8_t)((secFlg) ? R_TRUE : R_FALSE);

    R_UIF_DrvCrc32 (port_id, 2, (uint8_t *)&phdlc->head, 0, crc);
    R_UIF_DrvCrc32 (port_id, len, pin, R_BYTE_ArrToUInt32 (crc), crc);


    pdst = phdlc->payload;
    psrc = pin;
    for (i = 0u; i < len; i++)
    {
        if (R_UIF_HDLC_EDECODE_E == (*psrc))
        {
            *(pdst++) = R_UIF_HDLC_ECODE_H;
            *(pdst++) = R_UIF_HDLC_ECODE_LE;
        }
        else if (R_UIF_HDLC_EDECODE_D == (*psrc))
        {
            *(pdst++) = R_UIF_HDLC_ECODE_H;
            *(pdst++) = R_UIF_HDLC_ECODE_LD;
        }
        else
        {
            *(pdst++) = *psrc;
        }
        psrc++;
    }

    for (i = 0u; i < 4; i++)
    {
        if (R_UIF_HDLC_EDECODE_E == crc[i])
        {
            *(pdst++) = R_UIF_HDLC_ECODE_H;
            *(pdst++) = R_UIF_HDLC_ECODE_LE;
        }
        else if (R_UIF_HDLC_EDECODE_D == crc[i])
        {
            *(pdst++) = R_UIF_HDLC_ECODE_H;
            *(pdst++) = R_UIF_HDLC_ECODE_LD;
        }
        else
        {
            *(pdst++) = crc[i];
        }
    }

    *(pdst++) = R_UIF_HDLC_SCODE;
    length    = (uint16_t)((r_uintptr_t)pdst - (r_uintptr_t)phdlc);
    return length;
} /* uif_hdlc_enc */
/******************************************************************************
   End of function  uif_hdlc_enc
******************************************************************************/

/*===========================================================================*/
/* HDLC Decode Process                                                       */
/*===========================================================================*/

/******************************************************************************
* Function Name:uif_hdlc_dec
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_sys_status_t uif_hdlc_dec (uint8_t port_id, r_uif_key_t * pKey, uint8_t * pin, uint16_t len)
{
    r_sys_status_t        ret   = R_SYS_STATUS_SUCCESS;
    r_uif_hdlc_pack_t *   phdlc = (r_uif_hdlc_pack_t *)pin;
    r_uif_hdlc_footer_t * pfoot;
    uint8_t               crc[4];
    r_uif_core_buff_t * pcore = uif_get_core_buff_p (port_id);
    if (pcore == NULL)
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }

    UNUSED (pKey);

    if (
        (NULL == pin) ||
        ((len >= (R_UIF_HDLC_HEADER_LEN + (pcore->uif_command_len_max))) ||
         (len <= R_UIF_HDLC_PACK_WOESC_MIN))
        )
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }

    pfoot = (r_uif_hdlc_footer_t *)&pin[(len - 4u) - 1u];
    if (NULL == pfoot)
    {
        return R_SYS_STATUS_INVALID_PARAMETER;
    }


    if (R_SYS_STATUS_SUCCESS == ret)
    {
        if (R_UIF_DrvCrc32 (port_id, (uint16_t)(len - R_UIF_HDLC_HEADER_LEN_WOSCODE), (uint8_t *)&phdlc->head, 0u, crc) == R_SYS_STATUS_SUCCESS)
        {
            if (R_memcmp (pfoot->crc, crc, 4))
            {
                {
                    R_UIF_EventNotify (port_id, R_SYS_EVENT_CRC_ERROR, NULL);
                    ret = R_SYS_STATUS_CRC_ERROR;
                }
            }
        }
        else
        {
            ret = R_SYS_STATUS_DRV_ERROR;
        }
    }

    return ret;
} /* uif_hdlc_dec */
/******************************************************************************
   End of function  uif_hdlc_dec
******************************************************************************/

