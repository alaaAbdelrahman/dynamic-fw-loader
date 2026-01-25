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
 * File Name : r_c3sap_sys_convert.c
 * Description : System layer Bin convert API
 ******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_byte_swap.h"
#include "r_sys_sap.h"
#include "r_sys_cmd.h"
#include "r_sys_binstruct.h"
#include "r_c3sap_sys_if.h"
#include "r_c3sap_sys_convert.h"

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
static uint16_t sys_str2bin_reset_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t sys_str2bin_clearinfo_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t sys_str2bin_getinfo_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t sys_str2bin_sromread_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t sys_str2bin_sromwrite_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);
static uint16_t sys_str2bin_sromerase_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin);

static uint16_t sys_bin2str_ping_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t sys_bin2str_version_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t sys_bin2str_clearinfo_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t sys_bin2str_getinfo_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t sys_bin2str_sromread_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t sys_bin2str_sromwrite_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t sys_bin2str_sromerase_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t sys_bin2str_event_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);
static uint16_t sys_bin2str_rebootreq_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str);

/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

/******************************************************************************
* Function Name:R_SYS_ReqStr2Bin
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_SYS_ReqStr2Bin (uint8_t cmd, void * pin_str, uint16_t max_len, uint8_t * pout_bin, uint16_t * pout_len)
{
    r_result_t ret    = R_RESULT_FAILED;
    uint16_t   length = 0u;

    if (((NULL == pin_str) || (NULL == pout_bin)) || (NULL == pout_len))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    switch (cmd)
    {
        case R_SYS_PKT_CMD_RESET:
            length = sys_str2bin_reset_req (pin_str, max_len, pout_bin);
            break;

        case R_SYS_PKT_CMD_CLEARINFO:
            length = sys_str2bin_clearinfo_req (pin_str, max_len, pout_bin);
            break;

        case R_SYS_PKT_CMD_GETINFO:
            length = sys_str2bin_getinfo_req (pin_str, max_len, pout_bin);
            break;

        case R_SYS_PKT_CMD_SROMREAD:
            length = sys_str2bin_sromread_req (pin_str, max_len, pout_bin);
            break;

        case R_SYS_PKT_CMD_SROMWRITE:
            length = sys_str2bin_sromwrite_req (pin_str, max_len, pout_bin);
            break;

        case R_SYS_PKT_CMD_SROMERASE:
            length = sys_str2bin_sromerase_req (pin_str, max_len, pout_bin);
            break;

#ifdef R_DEFINE_ENABLE_MEMRW
        case R_SYS_PKT_CMD_MEMREAD:
            length = sys_str2bin_sromread_req (pin_str, max_len, pout_bin);
            break;
#endif
        case R_SYS_PKT_CMD_PING:
        case R_SYS_PKT_CMD_VERSION:
            ret    = R_RESULT_SUCCESS;
            break;

        default:
            ret    = R_RESULT_BAD_INPUT_ARGUMENTS;
            break;
    } /* switch */

    if (
        (R_RESULT_SUCCESS == ret) ||
        (length)
        )
    {
        ret       = R_RESULT_SUCCESS;
        *pout_len = length;
    }
    return ret;
} /* R_SYS_ReqStr2Bin */
/******************************************************************************
   End of function  R_SYS_ReqStr2Bin
******************************************************************************/

/******************************************************************************
* Function Name:R_SYS_CbBin2Str
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_SYS_CbBin2Str (uint8_t cmd, uint8_t * pin_bin, uint16_t max_len, void * pout_str, uint16_t * pout_len)
{
    r_result_t ret    = R_RESULT_FAILED;
    uint16_t   length = 0u;

    if (((NULL == pin_bin) || (NULL == pout_str)) || (NULL == pout_len))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    switch (cmd)
    {
        case R_SYS_PKT_CMD_PING:
            length = sys_bin2str_ping_cnf (pin_bin, max_len, pout_str);
            break;

        case R_SYS_PKT_CMD_VERSION:
            length = sys_bin2str_version_cnf (pin_bin, max_len, pout_str);
            break;

        case R_SYS_PKT_CMD_CLEARINFO:
            length = sys_bin2str_clearinfo_cnf (pin_bin, max_len, pout_str);
            break;

        case R_SYS_PKT_CMD_GETINFO:
            length = sys_bin2str_getinfo_cnf (pin_bin, max_len, pout_str);
            break;

        case R_SYS_PKT_CMD_SROMREAD:
            length = sys_bin2str_sromread_cnf (pin_bin, max_len, pout_str);
            break;

        case R_SYS_PKT_CMD_SROMWRITE:
            length = sys_bin2str_sromwrite_cnf (pin_bin, max_len, pout_str);
            break;

        case R_SYS_PKT_CMD_SROMERASE:
            length = sys_bin2str_sromerase_cnf (pin_bin, max_len, pout_str);
            break;

#ifdef R_DEFINE_ENABLE_MEMRW
        case R_SYS_PKT_CMD_MEMREAD:
            length = sys_bin2str_sromread_cnf (pin_bin, max_len, pout_str);
            break;
#endif
        case R_SYS_PKT_CMD_EVENT:
            length = sys_bin2str_event_ind (pin_bin, max_len, pout_str);
            break;

        case R_SYS_PKT_CMD_REBOOTREQ:
            length = sys_bin2str_rebootreq_ind (pin_bin, max_len, pout_str);
            break;

        default:
            ret    = R_RESULT_BAD_INPUT_ARGUMENTS;
            break;
    } /* switch */

    if (length)
    {
        ret       = R_RESULT_SUCCESS;
        *pout_len = length;
    }
    return ret;
} /* R_SYS_CbBin2Str */
/******************************************************************************
   End of function  R_SYS_CbBin2Str
******************************************************************************/

/******************************************************************************
* Function Name:sys_str2bin_reset_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_str2bin_reset_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                length = sizeof (r_sys_reset_req_bin_t);
    r_sys_reset_req_t *     pstr   = (r_sys_reset_req_t *)pin_str;
    r_sys_reset_req_bin_t * pbin   = (r_sys_reset_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pbin->mode        = pstr->mode;
    pbin->interval[0] = GET_S2C0 (pstr->interval);
    pbin->interval[1] = GET_S2C1 (pstr->interval);
    return length;
} /* sys_str2bin_reset_req */
/******************************************************************************
   End of function  sys_str2bin_reset_req
******************************************************************************/

/******************************************************************************
* Function Name:sys_str2bin_clearinfo_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_str2bin_clearinfo_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                     length = sizeof (r_sys_clear_info_req_bin_t);
    r_sys_clear_info_req_t *     pstr   = (r_sys_clear_info_req_t *)pin_str;
    r_sys_clear_info_req_bin_t * pbin   = (r_sys_clear_info_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pbin->infoTypeBit  = pstr->infoTypeBit;
    pbin->infoBlockBit = pstr->infoBlockBit;
    return length;
} /* sys_str2bin_clearinfo_req */
/******************************************************************************
   End of function  sys_str2bin_clearinfo_req
******************************************************************************/

/******************************************************************************
* Function Name:sys_str2bin_getinfo_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_str2bin_getinfo_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                   length = sizeof (r_sys_get_info_req_bin_t);
    r_sys_get_info_req_t *     pstr   = (r_sys_get_info_req_t *)pin_str;
    r_sys_get_info_req_bin_t * pbin   = (r_sys_get_info_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pbin->infoType  = pstr->infoType;
    pbin->infoBlock = pstr->infoBlock;
    return length;
} /* sys_str2bin_getinfo_req */
/******************************************************************************
   End of function  sys_str2bin_getinfo_req
******************************************************************************/


/******************************************************************************
* Function Name:sys_str2bin_sromread_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_str2bin_sromread_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                  length = sizeof (r_sys_memread_req_bin_t);
    r_sys_memread_req_t *     pstr   = (r_sys_memread_req_t *)pin_str;
    r_sys_memread_req_bin_t * pbin   = (r_sys_memread_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }

    if (max_len < length)
    {
        return 0u;
    }
    pbin->addr[0]    = GET_I2C0 (pstr->addr);
    pbin->addr[1]    = GET_I2C1 (pstr->addr);
    pbin->addr[2]    = GET_I2C2 (pstr->addr);
    pbin->addr[3]    = GET_I2C3 (pstr->addr);
    pbin->length[0]  = GET_S2C0 (pstr->length);
    pbin->length[1]  = GET_S2C1 (pstr->length);
    pbin->accessType = pstr->accessType;
    return length;
} /* sys_str2bin_sromread_req */
/******************************************************************************
   End of function  sys_str2bin_sromread_req
******************************************************************************/

/******************************************************************************
* Function Name:sys_str2bin_sromwrite_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_str2bin_sromwrite_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    r_sys_memwrite_req_t *     pstr = (r_sys_memwrite_req_t *)pin_str;
    r_sys_memwrite_req_bin_t * pbin = (r_sys_memwrite_req_bin_t *)pout_bin;
    uint16_t                   length;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }

    length           = (uint16_t)((sizeof (r_sys_memwrite_req_bin_t) - R_SYS_MEMREAD_LEN_MAX) + pstr->length);
    if (max_len < length)
    {
        return 0u;
    }

    pbin->addr[0]    = GET_I2C0 (pstr->addr);
    pbin->addr[1]    = GET_I2C1 (pstr->addr);
    pbin->addr[2]    = GET_I2C2 (pstr->addr);
    pbin->addr[3]    = GET_I2C3 (pstr->addr);
    pbin->length[0]  = GET_S2C0 (pstr->length);
    pbin->length[1]  = GET_S2C1 (pstr->length);
    pbin->accessType = pstr->accessType;
    R_memcpy (pbin->data, pstr->pdata, pstr->length);
    return length;
} /* sys_str2bin_sromwrite_req */
/******************************************************************************
   End of function  sys_str2bin_sromwrite_req
******************************************************************************/

/******************************************************************************
* Function Name:sys_str2bin_sromerase_req
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_str2bin_sromerase_req (void * pin_str, uint16_t max_len, uint8_t * pout_bin)
{
    uint16_t                   length = sizeof (r_sys_memerase_req_bin_t);
    r_sys_memerase_req_t *     pstr   = (r_sys_memerase_req_t *)pin_str;
    r_sys_memerase_req_bin_t * pbin   = (r_sys_memerase_req_bin_t *)pout_bin;

    if ((NULL == pin_str) || (NULL == pout_bin))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pbin->addr[0]   = GET_I2C0 (pstr->addr);
    pbin->addr[1]   = GET_I2C1 (pstr->addr);
    pbin->addr[2]   = GET_I2C2 (pstr->addr);
    pbin->addr[3]   = GET_I2C3 (pstr->addr);
    pbin->length[0] = GET_I2C0 (pstr->length);
    pbin->length[1] = GET_I2C1 (pstr->length);
    pbin->length[2] = GET_I2C2 (pstr->length);
    pbin->length[3] = GET_I2C3 (pstr->length);
    return length;
} /* sys_str2bin_sromerase_req */
/******************************************************************************
   End of function  sys_str2bin_sromerase_req
******************************************************************************/

/******************************************************************************
* Function Name:sys_bin2str_ping_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_bin2str_ping_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t               length = sizeof (r_sys_ping_cnf_bin_t);
    r_sys_ping_cnf_bin_t * pbin   = (r_sys_ping_cnf_bin_t *)pin_bin;
    r_sys_ping_cnf_t *     pstr   = (r_sys_ping_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status = pbin->status;
    return length;
}
/******************************************************************************
   End of function  sys_bin2str_ping_cnf
******************************************************************************/

/******************************************************************************
* Function Name:sys_bin2str_version_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_bin2str_version_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                  length = sizeof (r_sys_version_cnf_t);
    r_sys_version_cnf_bin_t * pbin   = (r_sys_version_cnf_bin_t *)pin_bin;
    r_sys_version_cnf_t *     pstr   = (r_sys_version_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->pfVer    = pbin->pfVer;
    pstr->standard = pbin->standard;
    pstr->bandPlan = R_BYTE_ArrToUInt16 (pbin->bandPlan);
    pstr->sysVer   = R_BYTE_ArrToUInt16 (pbin->sysVer);
    pstr->hwVer    = R_BYTE_ArrToUInt32 (pbin->hwVer);
    return length;
} /* sys_bin2str_version_cnf */
/******************************************************************************
   End of function  sys_bin2str_version_cnf
******************************************************************************/

/******************************************************************************
* Function Name:sys_bin2str_clearinfo_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_bin2str_clearinfo_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    uint16_t                     length = sizeof (r_sys_clear_info_cnf_t);
    r_sys_clear_info_cnf_bin_t * pbin   = (r_sys_clear_info_cnf_bin_t *)pin_bin;
    r_sys_clear_info_cnf_t *     pstr   = (r_sys_clear_info_cnf_t *)pout_str;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status = pbin->status;
    return length;
}
/******************************************************************************
   End of function  sys_bin2str_clearinfo_cnf
******************************************************************************/

/******************************************************************************
* Function Name:sys_bin2str_getinfo_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_bin2str_getinfo_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_sys_get_info_cnf_bin_t * pbin = (r_sys_get_info_cnf_bin_t *)pin_bin;
    r_sys_get_info_cnf_t *     pstr = (r_sys_get_info_cnf_t *)pout_str;
    uint16_t                   length;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    length          = (uint16_t)((sizeof (r_sys_get_info_cnf_t) + R_BYTE_ArrToUInt16 (pbin->length)));
    if (max_len < length)
    {
        return 0u;
    }

    pstr->status    = pbin->status;
    pstr->infoType  = pbin->infoType;
    pstr->infoBlock = pbin->infoBlock;
    pstr->length    = R_BYTE_ArrToUInt16 (pbin->length);
    pstr->pinfo     = (uint32_t *)&pstr[1];
    R_memcpy (pstr->pinfo, pbin->info, pstr->length);

    return length;
} /* sys_bin2str_getinfo_cnf */
/******************************************************************************
   End of function  sys_bin2str_getinfo_cnf
******************************************************************************/

/******************************************************************************
* Function Name:sys_bin2str_sromread_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_bin2str_sromread_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_sys_memread_cnf_bin_t * pbin = (r_sys_memread_cnf_bin_t *)pin_bin;
    r_sys_memread_cnf_t *     pstr = (r_sys_memread_cnf_t *)pout_str;
    uint16_t                  length;

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    length           = (uint16_t)((sizeof (r_sys_memread_cnf_t) + R_BYTE_ArrToUInt16 (pbin->length)));
    if (max_len < length)
    {
        return 0u;
    }
    pstr->status     = pbin->status;
    pstr->addr       = R_BYTE_ArrToUInt32 (pbin->addr);
    pstr->length     = R_BYTE_ArrToUInt16 (pbin->length);
    pstr->accessType = pbin->accessType;
    pstr->pdata      = (uint8_t *)&pstr[1];
    R_memcpy (pstr->pdata, pbin->data, pstr->length);

    return length;
} /* sys_bin2str_sromread_cnf */
/******************************************************************************
   End of function  sys_bin2str_sromread_cnf
******************************************************************************/

/******************************************************************************
* Function Name:sys_bin2str_sromwrite_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_bin2str_sromwrite_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_sys_memwrite_cnf_bin_t * pbin   = (r_sys_memwrite_cnf_bin_t *)pin_bin;
    r_sys_memwrite_cnf_t *     pstr   = (r_sys_memwrite_cnf_t *)pout_str;
    uint16_t                   length = sizeof (r_sys_memwrite_cnf_t);

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    if (max_len < length)
    {
        return 0u;
    }

    pstr->status     = pbin->status;
    pstr->addr       = R_BYTE_ArrToUInt32 (pbin->addr);
    pstr->length     = R_BYTE_ArrToUInt16 (pbin->length);
    pstr->accessType = pbin->accessType;

    return length;
} /* sys_bin2str_sromwrite_cnf */
/******************************************************************************
   End of function  sys_bin2str_sromwrite_cnf
******************************************************************************/

/******************************************************************************
* Function Name:sys_bin2str_sromerase_cnf
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_bin2str_sromerase_cnf (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_sys_memerase_cnf_bin_t * pbin   = (r_sys_memerase_cnf_bin_t *)pin_bin;
    r_sys_memerase_cnf_t *     pstr   = (r_sys_memerase_cnf_t *)pout_str;
    uint16_t                   length = sizeof (r_sys_memerase_cnf_t);

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }

    if (max_len < length)
    {
        return 0u;
    }

    pstr->status = pbin->status;
    pstr->addr   = R_BYTE_ArrToUInt32 (pbin->addr);
    pstr->length = R_BYTE_ArrToUInt32 (pbin->length);

    return length;
} /* sys_bin2str_sromerase_cnf */
/******************************************************************************
   End of function  sys_bin2str_sromerase_cnf
******************************************************************************/

/******************************************************************************
* Function Name:sys_bin2str_event_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_bin2str_event_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_sys_event_ind_bin_t * pbin   = (r_sys_event_ind_bin_t *)pin_bin;
    r_sys_event_ind_t *     pstr   = (r_sys_event_ind_t *)pout_str;
    uint16_t                length = sizeof (r_sys_event_ind_t);

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }

    pstr->cause = pbin->cause;

    return length;
} /* sys_bin2str_event_ind */
/******************************************************************************
   End of function  sys_bin2str_event_ind
******************************************************************************/

/******************************************************************************
* Function Name:sys_bin2str_rebootreq_ind
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t sys_bin2str_rebootreq_ind (uint8_t * pin_bin, uint16_t max_len, void * pout_str)
{
    r_sys_rebootreq_ind_bin_t * pbin   = (r_sys_rebootreq_ind_bin_t *)pin_bin;
    r_sys_rebootreq_ind_t *     pstr   = (r_sys_rebootreq_ind_t *)pout_str;
    uint16_t                    length = sizeof (r_sys_event_ind_t);

    if ((NULL == pin_bin) || (NULL == pout_str))
    {
        return 0u;
    }
    if (max_len < length)
    {
        return 0u;
    }

    pstr->status = pbin->status;

    return length;
} /* sys_bin2str_rebootreq_ind */
/******************************************************************************
   End of function  sys_bin2str_rebootreq_ind
******************************************************************************/

