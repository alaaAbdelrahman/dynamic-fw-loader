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
 * File Name : r_c3sap_sys.c
 * Description : System layer API (None environment dependent)
 ******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"
#include "r_sys_cmd.h"
#include "r_c3sap_api.h"
#include "r_c3sap_sys.h"
#include "r_c3sap_sys_if.h"
#include "r_c3sap_sys_convert.h"
#include "r_c3sap_sys_depend.h"
#include "r_sys_binstruct.h"



#include "r_c3sap_g3_if.h"
#include "r_c3sap_g3_buffsize.h"
/******************************************************************************
Macro definitions
******************************************************************************/
#define R_SYS_SET_PKT_HEAD(c, a, p)  ((uint8_t)((c) << 7) | (uint8_t)((a) << 4) | (uint8_t)(p))
#define R_SYS_GET_PKT_IDA(a)         ((uint8_t)((a) >> 4) & 0x03u)

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef union
{
    r_sys_ping_cnf_t              ping_cnf;
    r_sys_version_cnf_t           version_cnf;
    r_sys_clear_info_cnf_t        clear_info_cnf;
    r_sys_get_info_cnf_t          get_info_cnf;
    r_sys_memread_cnf_t           srom_read_cnf;
    r_sys_memwrite_cnf_t          srom_write_cnf;
    r_sys_memerase_cnf_t          srom_erase_cnf;
    r_sys_event_ind_t             event_ind;
    r_sys_rebootreq_ind_t         reboot_req_ind;
} r_sys_cb_str_t;

typedef union
{
    r_sys_reset_req_bin_t         reset;
    r_sys_clear_info_req_bin_t    clear;
    r_sys_get_info_req_bin_t      get;

    r_sys_memread_req_bin_t       mread;
    r_sys_memwrite_req_bin_t      mwrite;
    r_sys_memerase_req_bin_t      merase;
    r_plc_req_buffsize_str_t      buffsize;
} r_sys_req_bin_size_t;

typedef struct
{
    uint8_t              id;
    uint8_t              cmd;
    r_sys_req_bin_size_t payload;
} r_c3sap_req_bin_size_t;

typedef struct
{
    r_sys_cb_str_t cb;
    uint8_t        buff[R_SYS_GETINFO_LEN_MAX];
} r_sys_cb_buffsize_str_t;

typedef union
{
    r_sys_cb_buffsize_str_t sys_buffsize;
    r_plc_cb_buffsize_str_t plc_buffsize;
} r_c3sap_cb_str_size_t;

typedef struct
{
    r_sys_callback_t callbacks;
    uint8_t *        preq_bin;
    r_sys_cb_str_t * pcb_str;
    uint16_t         req_buff_size;
    uint16_t         cb_buff_size;
} r_sys_info_t;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static r_sys_info_t sys_info;
static uint8_t      req_bin_buff[sizeof (r_c3sap_req_bin_size_t)];
static uint32_t     cb_str_buff[(sizeof (r_c3sap_cb_str_size_t) + 3) / sizeof (uint32_t)];

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void sys_base_cb_cnf (uint8_t * pbuff);
static void sys_base_cb_ind (uint8_t * pbuff);
static r_result_t sys_process_request (uint8_t cmd, void * preq);
static r_result_t sys_process_request_without_payload (uint8_t cmd);

/*===========================================================================*/
/* Functions                                                                 */
/*===========================================================================*/

/******************************************************************************
* Function Name : R_SYS_PingReq
* Description   : SYSTEM-PING.request
* Arguments     : None
* Return Value  : R_RESULT_SUCCESS : Successful transmission
*               : R_RESULT_FAILED  : Transmission failure
******************************************************************************/
r_result_t R_SYS_PingReq (void)
{
    r_result_t status;

    status = sys_process_request_without_payload (R_SYS_PKT_CMD_PING);

    return status;
}
/******************************************************************************
   End of function  R_SYS_PingReq
******************************************************************************/

/******************************************************************************
* Function Name : R_SYS_VersionReq
* Description   : SYSTEM-VERSION.request
* Arguments     : None
* Return Value  : R_RESULT_SUCCESS : Successful transmission
*               : R_RESULT_FAILED  : Transmission failure
******************************************************************************/
r_result_t R_SYS_VersionReq (void)
{
    r_result_t status;

    status = sys_process_request_without_payload (R_SYS_PKT_CMD_VERSION);

    return status;
}
/******************************************************************************
   End of function  R_SYS_VersionReq
******************************************************************************/

/******************************************************************************
* Function Name : R_SYS_ResetReq
* Description   : SYSTEM-RESET.request
* Arguments     : Pointer to a structure for r_sys_reset_req_t
* Return Value  : R_RESULT_SUCCESS : Successful transmission
*               : R_RESULT_FAILED  : Transmission failure
******************************************************************************/
r_result_t R_SYS_ResetReq (r_sys_reset_req_t * preq)
{
    r_result_t status;

    status = sys_process_request (R_SYS_PKT_CMD_RESET, preq);

    return status;
}
/******************************************************************************
   End of function  R_SYS_ResetReq
******************************************************************************/

/******************************************************************************
* Function Name : R_SYS_ClearInfoReq
* Description   : SYSTEM-CLEAR-INFO.request
* Arguments     : Pointer to a structure for r_sys_clear_info_req_t
* Return Value  : R_RESULT_SUCCESS : Successful transmission
*               : R_RESULT_FAILED  : Transmission failure
******************************************************************************/
r_result_t R_SYS_ClearInfoReq (r_sys_clear_info_req_t * preq)
{
    r_result_t status;

    status = sys_process_request (R_SYS_PKT_CMD_CLEARINFO, preq);

    return status;
}
/******************************************************************************
   End of function  R_SYS_ClearInfoReq
******************************************************************************/

/******************************************************************************
* Function Name : R_SYS_SromReadReq
* Description   : SYSTEM-SROM-READ.request
* Arguments     : Pointer to a structure for r_sys_memread_req_t
* Return Value  : R_RESULT_SUCCESS : Successful transmission
*               : R_RESULT_FAILED  : Transmission failure
******************************************************************************/
r_result_t R_SYS_SromReadReq (r_sys_memread_req_t * preq)
{
    r_result_t status;

    status = sys_process_request (R_SYS_PKT_CMD_SROMREAD, preq);

    return status;
}
/******************************************************************************
   End of function  R_SYS_SromReadReq
******************************************************************************/

/******************************************************************************
* Function Name : R_SYS_SromWriteReq
* Description   : SYSTEM-SROM-WRITE.request
* Arguments     : Pointer to a structure for r_sys_memwrite_req_t
* Return Value  : R_RESULT_SUCCESS : Successful transmission
*               : R_RESULT_FAILED  : Transmission failure
******************************************************************************/
r_result_t R_SYS_SromWriteReq (r_sys_memwrite_req_t * preq)
{
    r_result_t status;

    status = sys_process_request (R_SYS_PKT_CMD_SROMWRITE, preq);

    return status;
}
/******************************************************************************
   End of function  R_SYS_SromWriteReq
******************************************************************************/

/******************************************************************************
* Function Name : R_SYS_SromEraseReq
* Description   : SYSTEM-SROM-ERASE.request
* Arguments     : Pointer to a structure for r_sys_memerase_req_t
* Return Value  : R_RESULT_SUCCESS : Successful transmission
*               : R_RESULT_FAILED  : Transmission failure
******************************************************************************/
r_result_t R_SYS_SromEraseReq (r_sys_memerase_req_t * preq)
{
    r_result_t status;

    status = sys_process_request (R_SYS_PKT_CMD_SROMERASE, preq);

    return status;
}
/******************************************************************************
   End of function  R_SYS_SromEraseReq
******************************************************************************/

#ifdef R_DEFINE_ENABLE_MEMRW
/******************************************************************************
* Function Name : R_SYS_MemReadReq
* Description   : SYSTEM-MEM-READ.request
* Arguments     : Pointer to a structure for r_sys_memread_req_t
* Return Value  : R_RESULT_SUCCESS : Successful transmission
*               : R_RESULT_FAILED  : Transmission failure
******************************************************************************/
r_result_t R_SYS_MemReadReq (r_sys_memread_req_t * preq)
{
    r_result_t status;

    status = sys_process_request (R_SYS_PKT_CMD_MEMREAD, preq);

    return status;
}
/******************************************************************************
   End of function  R_SYS_SromReadReq
******************************************************************************/
#endif
/******************************************************************************
* Function Name : R_SYS_GetInfoReq
* Description   : SYSTEM-GET-INFO.request
* Arguments     : Pointer to a structure for r_sys_get_info_req_t
* Return Value  : R_RESULT_SUCCESS : Successful transmission
*               : R_RESULT_FAILED  : Transmission failure
******************************************************************************/
r_result_t R_SYS_GetInfoReq (r_sys_get_info_req_t * preq)
{
    r_result_t status;

    status = sys_process_request (R_SYS_PKT_CMD_GETINFO, preq);

    return status;
}
/******************************************************************************
   End of function  R_SYS_GetInfoReq
******************************************************************************/


/******************************************************************************
* Function Name : sys_init_sub
* Description   : Initialize  processing for the system command
* Arguments     : pcallbacks : Pointer of the system callback functions
* Return Value  : None
******************************************************************************/
void sys_init_sub (r_sys_callback_t * pcallbacks)
{
    memset (&sys_info, 0, sizeof (r_sys_info_t));
    
    sys_regist_callback(pcallbacks);
    
    sys_info.preq_bin      = req_bin_buff;
    sys_info.req_buff_size = sizeof (req_bin_buff);
    sys_info.pcb_str       = (r_sys_cb_str_t *)cb_str_buff;
    sys_info.cb_buff_size  = sizeof (cb_str_buff);
    R_G3CTRL_Init (sys_info.preq_bin, sizeof (req_bin_buff), (void *)sys_info.pcb_str, sizeof (cb_str_buff));
}
/******************************************************************************
   End of function  sys_init_sub
******************************************************************************/

/******************************************************************************
* Function Name : sys_deinit_sub
* Description   : Deinitialize  processing for the system command
* Arguments     : None
* Return Value  : None
******************************************************************************/
void sys_deinit_sub ( void )
{
    R_G3CTRL_Deinit();
}
/******************************************************************************
   End of function  sys_deinit_sub
******************************************************************************/

/******************************************************************************
* Function Name : sys_regist_callback
* Description   : Regist system callbak functions
* Arguments     : pcallbacks : Pointer of the system callback functions
* Return Value  : None
******************************************************************************/
r_result_t sys_regist_callback (r_sys_callback_t * pcallbacks)
{
    r_result_t status = R_RESULT_SUCCESS;
    
    if (NULL != pcallbacks)
    {
        sys_info.callbacks = *pcallbacks;
    }
    else
    {
        status = R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    
    return status;
}
/******************************************************************************
   End of function  sys_regist_callback
******************************************************************************/

/******************************************************************************
* Function Name : sys_base_cb
* Description   : Callback processing for the system command
* Arguments     : pbuff : Pointer of the binary command data
* Return Value  : None
******************************************************************************/
void sys_base_cb (uint8_t * pbuff)
{
    uint8_t ida;

    if (NULL != pbuff)
    {
        ida = R_SYS_GET_PKT_IDA (pbuff[0]);
        if (R_SYS_PKT_IDA_CONFIRM == ida)
        {
            sys_base_cb_cnf (pbuff);
        }
        else if (R_SYS_PKT_IDA_INDICATION == ida)
        {
            sys_base_cb_ind (pbuff);
        }
        else
        {
            /* Do Nothing */
        }
    }
} /* sys_base_cb */
/******************************************************************************
   End of function  sys_base_cb
******************************************************************************/

/******************************************************************************
* Function Name : sys_base_cb_cnf
* Description   : Confirm to the user
* Arguments     : pbuff : Pointer of the binary command data
* Return Value  : None
******************************************************************************/
static void sys_base_cb_cnf (uint8_t * pbuff)
{
    uint8_t    cmd;
    uint16_t   length;
    uint16_t   max_len = sys_info.cb_buff_size;
    r_result_t status;

    if ((NULL == pbuff) || (NULL == sys_info.pcb_str))
    {
        return;
    }

    cmd = pbuff[1];
    switch (cmd)
    {
        case R_SYS_PKT_CMD_PING:
        {
            if (NULL != sys_info.callbacks.R_SYS_PingCnf)
            {
                status = R_SYS_CbBin2Str (cmd, &pbuff[2], max_len, (void *)&sys_info.pcb_str->ping_cnf, &length);
                if ((R_RESULT_SUCCESS == status) && (0u != length))
                {
                    sys_info.callbacks.R_SYS_PingCnf (&sys_info.pcb_str->ping_cnf);
                }
            }
            break;
        }

        case R_SYS_PKT_CMD_VERSION:
        {
            if (NULL != sys_info.callbacks.R_SYS_VersionCnf)
            {
                status = R_SYS_CbBin2Str (cmd, &pbuff[2], max_len, (void *)&sys_info.pcb_str->version_cnf, &length);
                if ((R_RESULT_SUCCESS == status) && (0u != length))
                {
                    sys_info.callbacks.R_SYS_VersionCnf (&sys_info.pcb_str->version_cnf);
                }
            }
            break;
        }

        case R_SYS_PKT_CMD_CLEARINFO:
        {
            if (NULL != sys_info.callbacks.R_SYS_ClearInfoCnf)
            {
                status = R_SYS_CbBin2Str (cmd, &pbuff[2], max_len, (void *)&sys_info.pcb_str->clear_info_cnf, &length);
                if ((R_RESULT_SUCCESS == status) && (0u != length))
                {
                    sys_info.callbacks.R_SYS_ClearInfoCnf (&sys_info.pcb_str->clear_info_cnf);
                }
            }
            break;
        }

        case R_SYS_PKT_CMD_GETINFO:
        {
            if (NULL != sys_info.callbacks.R_SYS_GetInfoCnf)
            {
                status = R_SYS_CbBin2Str (cmd, &pbuff[2], max_len, (void *)&sys_info.pcb_str->get_info_cnf, &length);
                if ((R_RESULT_SUCCESS == status) && (0u != length))
                {
                    sys_info.callbacks.R_SYS_GetInfoCnf (&sys_info.pcb_str->get_info_cnf);
                }
            }
            break;
        }

        case R_SYS_PKT_CMD_SROMREAD:
        {
            if (NULL != sys_info.callbacks.R_SYS_SromReadCnf)
            {
                status = R_SYS_CbBin2Str (cmd, &pbuff[2], max_len, (void *)&sys_info.pcb_str->srom_read_cnf, &length);
                if ((R_RESULT_SUCCESS == status) && (0u != length))
                {
                    sys_info.callbacks.R_SYS_SromReadCnf (&sys_info.pcb_str->srom_read_cnf);
                }
            }
            break;
        }

        case R_SYS_PKT_CMD_SROMWRITE:
        {
            if (NULL != sys_info.callbacks.R_SYS_SromWriteCnf)
            {
                status = R_SYS_CbBin2Str (cmd, &pbuff[2], max_len, (void *)&sys_info.pcb_str->srom_write_cnf, &length);
                if ((R_RESULT_SUCCESS == status) && (0u != length))
                {
                    sys_info.callbacks.R_SYS_SromWriteCnf (&sys_info.pcb_str->srom_write_cnf);
                }
            }
            break;
        }

        case R_SYS_PKT_CMD_SROMERASE:
        {
            if (NULL != sys_info.callbacks.R_SYS_SromEraseCnf)
            {
                status = R_SYS_CbBin2Str (cmd, &pbuff[2], max_len, (void *)&sys_info.pcb_str->srom_erase_cnf, &length);
                if ((R_RESULT_SUCCESS == status) && (0u != length))
                {
                    sys_info.callbacks.R_SYS_SromEraseCnf (&sys_info.pcb_str->srom_erase_cnf);
                }
            }
            break;
        }

#ifdef R_DEFINE_ENABLE_MEMRW
        case R_SYS_PKT_CMD_MEMREAD:
        {
            if (NULL != sys_info.callbacks.R_SYS_MemReadCnf)
            {
                status = R_SYS_CbBin2Str (cmd, &pbuff[2], max_len, (void *)&sys_info.pcb_str->srom_read_cnf, &length);
                if ((R_RESULT_SUCCESS == status) && (0u != length))
                {
                    sys_info.callbacks.R_SYS_MemReadCnf (&sys_info.pcb_str->srom_read_cnf);
                }
            }
            break;
        }
#endif
        default:
            break;
    } /* switch */
} /* sys_base_cb_cnf */
/******************************************************************************
   End of function  sys_base_cb_cnf
******************************************************************************/

/******************************************************************************
* Function Name : sys_base_cb_ind
* Description   : Indication to the user
* Arguments     : pbuff : Pointer of the binary command data
* Return Value  : None
******************************************************************************/
static void sys_base_cb_ind (uint8_t * pbuff)
{
    uint8_t    cmd;
    uint16_t   length;
    uint16_t   max_len = sys_info.cb_buff_size;
    r_result_t status;

    if ((NULL == pbuff) || (NULL == sys_info.pcb_str))
    {
        return;
    }

    cmd = pbuff[1];
    switch (cmd)
    {
        case R_SYS_PKT_CMD_BOOTUP:
        {
            if (NULL != sys_info.callbacks.R_SYS_BootUpInd)
            {
                sys_info.callbacks.R_SYS_BootUpInd();
            }
            sys_set_signal (R_SYS_SIGNAL_CONFIRM);
            break;
        }

        case R_SYS_PKT_CMD_EVENT:
        {
            if (NULL != sys_info.callbacks.R_SYS_EventInd)
            {
                status = R_SYS_CbBin2Str (cmd, &pbuff[2], max_len, (void *)&sys_info.pcb_str->event_ind, &length);
                if ((R_RESULT_SUCCESS == status) && (0u != length))
                {
                    sys_info.callbacks.R_SYS_EventInd (&sys_info.pcb_str->event_ind);
                }

            }
            break;
        }

        case R_SYS_PKT_CMD_REBOOTREQ:
        {
            if (NULL != sys_info.callbacks.R_SYS_ReBootReqInd)
            {
                status = R_SYS_CbBin2Str (cmd, &pbuff[2], max_len, (void *)&sys_info.pcb_str->reboot_req_ind, &length);
                if ((R_RESULT_SUCCESS == status) && (0u != length))
                {
                    sys_info.callbacks.R_SYS_ReBootReqInd (&sys_info.pcb_str->reboot_req_ind);
                }
            }
            break;
        }

        default:
            break;
    } /* switch */
} /* sys_base_cb_ind */
/******************************************************************************
   End of function  sys_base_cb_ind
******************************************************************************/

/******************************************************************************
* Function Name : sys_check_block_id
* Description   : Check the block ID
* Arguments     : block_id : block ID
* Return Value  : R_RESULT_SUCCESS : Normal value
                : R_RESULT_BAD_INPUT_ARGUMENTS : Abnormal value
******************************************************************************/
r_result_t sys_check_block_id (uint8_t block_id)
{
    r_result_t ret = R_RESULT_SUCCESS;

    if (R_C3SAP_BLOCKID_MAX <= block_id)
    {
        ret = R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    return ret;
}
/******************************************************************************
   End of function  sys_check_block_id
******************************************************************************/

/******************************************************************************
* Function Name:sys_process_request
* Description : Main function for the processing of SYS requests. All commands
*               and functions are processed in the same way.
* Arguments : uint8_t cmd: The command ID to be set in the packet (e.g. R_SYS_PKT_CMD_RESET)
*             void *preq: A generic pointer containing information about the specific request
* Return Value : - R_RESULT_BAD_INPUT_ARGUMENTS if the argument pointer is NULL
*                - R_RESULT_INVALID_REQUEST if the handle is invalid
*                - R_RESULT_SUCCESS if the sending of the command succeeds.
*                - R_RESULT_FAILED otherwise.
******************************************************************************/
static r_result_t sys_process_request (uint8_t cmd, void * preq)
{
    uint16_t   length  = 0u;
    #if defined (__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wconversion"
    /* -Wconversion false positive: 
     * https://bugzilla.mozilla.org/show_bug.cgi?id=450196 
     * and many others
     */
    #endif
    uint16_t   max_len = sys_info.req_buff_size - 2u;
    #if defined (__GNUC__)
    #pragma GCC diagnostic pop
    #endif
    st_sys_send_cmd_t   bin_cmd;
    r_result_t          status;

    if (NULL == preq)
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    status = R_SYS_SemaphoreWait ();
    if ((R_RESULT_SUCCESS == status) && (NULL != sys_info.preq_bin))
    {
        status = R_SYS_ReqStr2Bin (cmd, preq, max_len, &sys_info.preq_bin[2], &length);

        if ((R_RESULT_SUCCESS == status) && (0u != length))
        {
            sys_info.preq_bin[0] = R_SYS_SET_PKT_HEAD (0, R_SYS_PKT_IDA_REQUEST, R_SYS_PKT_IDP_SYS);
            sys_info.preq_bin[1] = cmd;
            
            bin_cmd.dst_block_id = R_C3SAP_BLOCKID_UARTIF_0;
            bin_cmd.src_block_id = R_C3SAP_BLOCKID_SYSTEM;
            bin_cmd.pbuffer      = sys_info.preq_bin;
            bin_cmd.size         = length + 2u;
            bin_cmd.reserved     = R_FALSE;
            status               = R_SYS_SendCmd (&bin_cmd);
        }

        R_SYS_SemaphoreRelease ();
    }

    return status;
} /* sys_process_request */
/******************************************************************************
   End of function  sys_process_request
******************************************************************************/

/******************************************************************************
* Function Name:sys_process_request_without_payload
* Description : Main function for the processing of SYS requests. All commands
*               and functions are processed in the same way.
* Arguments : uint8_t cmd: The command ID to be set in the packet (e.g. R_SYS_PKT_CMD_PING)
* Return Value : - R_RESULT_INVALID_REQUEST if the handle is invalid
*                - R_RESULT_SUCCESS if the sending of the command succeeds.
*                - R_RESULT_FAILED otherwise.
******************************************************************************/
static r_result_t sys_process_request_without_payload (uint8_t cmd)
{
    st_sys_send_cmd_t   bin_cmd;
    r_result_t          status;

    status = R_SYS_SemaphoreWait ();
    if ((R_RESULT_SUCCESS == status) && (NULL != sys_info.preq_bin))
    {
        sys_info.preq_bin[0] = R_SYS_SET_PKT_HEAD (0, R_SYS_PKT_IDA_REQUEST, R_SYS_PKT_IDP_SYS);
        sys_info.preq_bin[1] = cmd;

        bin_cmd.dst_block_id = R_C3SAP_BLOCKID_UARTIF_0;
        bin_cmd.src_block_id = R_C3SAP_BLOCKID_SYSTEM;
        bin_cmd.pbuffer      = sys_info.preq_bin;
        bin_cmd.size         = 2u;
        bin_cmd.reserved     = R_FALSE;
        status               = R_SYS_SendCmd (&bin_cmd);

        R_SYS_SemaphoreRelease ();
    }

    return status;
}
/******************************************************************************
   End of function  sys_process_request_without_payload
******************************************************************************/

