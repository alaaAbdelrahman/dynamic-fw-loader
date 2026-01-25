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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name   : r_demo_sysp.c
*    @version
*        $Rev: 4818 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2019-08-23 17:59:24 +0900#$
* Description :
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include "r_typedefs.h"
#include "r_stdio_api.h"
#include "r_byte_swap.h"

/* sys_part */
#include "r_sys_param2text.h"

/* g3 part */
#include "r_c3sap_api.h"


/* app part */
#include "r_demo_api.h"
#include "r_demo_tools.h"
#include "r_demo_app.h"
#include "r_demo_sys.h"
#include "r_demo_os_wrapper.h"
#include "r_demo_app_thread.h"



/******************************************************************************
Macro definitions
******************************************************************************/
#define R_SYS_SROM_SIZE_BAUD_RATE       (8u)        /* number of bytes storing baud rate parameter: 4 normal + 4 inverted */
#define R_SYS_SROM_SIZE_FW_HEADER       (0x50u)     /* magic number + 4 info tables */

/* parameters of S-ROM chip */
#define R_SYS_SROM_SIZE_WRITE_PAGE_MAX  (0x100)     /* chip can write any size from 1 to 256 bytes at once */
#define R_SYS_SROM_SIZE_ERASE_SECTOR    (0x1000)    /* the minimum size that can be erased at once - must be 4KB aligned */
#define R_SYS_SROM_SIZE_ERASE_GROUP     (0x10000)   /* must be 64KB aligned */

/* S-ROM memory layout */
#define R_SYS_SROM_ADDR_BASE            (0x10000000)
#define R_SYS_SROM_ADDR_BAUD_RATE       (R_SYS_SROM_ADDR_BASE)
#define R_SYS_SROM_ADDR_FW_HEADER       (R_SYS_SROM_ADDR_BASE + 0x10000)
#define R_SYS_SROM_ADDR_FW              (R_SYS_SROM_ADDR_FW_HEADER + R_SYS_SROM_SIZE_FW_HEADER)
#define R_SYS_SROM_ADDR_TOP             (R_SYS_SROM_ADDR_BASE + 0x100000)   /* addressing after this value wraps around from R_SYS_SROM_ADDR_BASE */

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Functions
******************************************************************************/
#if   !(defined __VANGO__)
static uint8_t show_char (uint8_t c);
static void print_buff (void * buff, uint32_t len, uint32_t start_addr);
static void menu_sys_srom (void);
static void menu_sys_srom_read (void);
static void menu_sys_srom_set_baudrate (void);
static void menu_sys_srom_erase (void);
static void menu_sys_srom_copy_fw (void);
#endif  /* #if !(defined R_DEFINE_CPX_SROM_UNUSE) &&  !(defined R_RL78_PLC) */
static void menu_sys_statistics (void);

/******************************************************************************
Exported global variables
******************************************************************************/
extern r_demo_config_t g_demo_config;
extern r_demo_buff_t   g_demo_buff;
#if  !defined R_RL78_PLC
extern uint8_t g_cpxprogtbl[];
#endif  /* #if ( !(defined R_SYNERGY_PLC) && !(defined R_DEFINE_CPX_SROM_UNUSE) && !(defined R_RL78_PLC) ) */


/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
#if   defined(R_DEFINE_OPEN_RX)
uint32_t               g_demo_sys_cb = R_OS_EVT_ID_APP_API_SYS;
#else
uint32_t               g_demo_sys_cb;
#endif

/******************************************************************************
Private global variables and functions
******************************************************************************/
volatile r_demo_sys_cb_str_t g_syscb;

/*===========================================================================*/
/*    SYS APIs                                                               */
/*===========================================================================*/


/******************************************************************************
* Function Name: R_DEMO_SysPing
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_SysPing (r_sys_ping_cnf_t ** cnf)
{
    r_result_t                    status;
    R_VOLATILE r_sys_ping_cnf_t * pingCfm = (R_VOLATILE r_sys_ping_cnf_t *)&g_syscb.pingCnf;
    *cnf = (r_sys_ping_cnf_t *)pingCfm;

    R_DEMO_ClearEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_PING );

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Sys-Ping requesting... ");
#endif
    }

    status = R_SYS_PingReq ();
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_SYS_PingReq */
        R_DEMO_WaitEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_PING );

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_SYS_STATUS_SUCCESS == pingCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("success. \n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: 0x%.2X\n", pingCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_SysPing */
/******************************************************************************
   End of function  R_DEMO_SysPing
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_SysVersion
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_SysVersion (r_sys_version_cnf_t ** cnf)
{
    r_result_t                       status;
    R_VOLATILE r_sys_version_cnf_t * versionCfm = (R_VOLATILE r_sys_version_cnf_t *)&g_syscb.versionCnf;
    *cnf = (r_sys_version_cnf_t *)versionCfm;

    R_DEMO_ClearEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_VERSION );

    versionCfm->pfVer = 0;

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Sys-Version requesting... ");
#endif
    }

    status = R_SYS_VersionReq ();
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_SYS_VersionReq */
        R_DEMO_WaitEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_VERSION );

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (3u == versionCfm->pfVer)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("success.\n");
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("    platform version:      0x%02X\n", versionCfm->pfVer);
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("    standard:              0x%02X", versionCfm->standard);
#endif
                if ((versionCfm->standard & 0x03) != 0)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf (" (");
#endif
                    if (versionCfm->standard & R_SYS_VERSION_STANDARD_G3)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("G3PLC");
#endif
                    }
                    if ((versionCfm->standard & 0x03) == 0x03)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf (" & ");
#endif
                    }
                    if (versionCfm->standard & R_SYS_VERSION_STANDARD_PRIME)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("PRIME");
#endif
                    }
                    if (versionCfm->standard & R_SYS_VERSION_STANDARD_RF)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf ("+RF");
#endif
                    }
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf (")");
#endif
                }
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n");
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("    band plan:             0x%04X", versionCfm->bandPlan);
#endif
                if (0 != versionCfm->bandPlan)
                {
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf (" (");
#endif
                    if (versionCfm->bandPlan & 0x0001)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf (" G3-CENELEC-A");
#endif
                    }
                    if (versionCfm->bandPlan & 0x0002)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf (" G3-CENELEC-B");
#endif
                    }
                    if (versionCfm->bandPlan & 0x0004)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf (" G3-FCC");
#endif
                    }
                    if (versionCfm->bandPlan & 0x0008)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf (" G3-ARIB");
#endif
                    }
                    if (versionCfm->bandPlan & 0x0100)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf (" PRIME-v1.3.6");
#endif
                    }
                    if (versionCfm->bandPlan & 0x0200)
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf (" PRIME-v1.4");
#endif
                    }
                    if (versionCfm->bandPlan & (uint16_t)(~0x030F))
                    {
#ifdef __PLC__DEBUG__
                        R_STDIO_Printf (" others");
#endif
                    }
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf (" )");
#endif
                }
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\n");
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("    system block version:  0x%04X\n", versionCfm->sysVer);
#endif
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("    hardware version:      0x%08X\n", versionCfm->hwVer);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. pfVer: 0x%.2X\n", versionCfm->pfVer);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_SysVersion */
/******************************************************************************
   End of function  R_DEMO_SysVersion
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_SysGetInfo
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_SysGetInfo (r_sys_get_info_req_t *  req,
                              r_sys_get_info_cnf_t ** cnf)
{
    r_result_t                        status;
    R_VOLATILE r_sys_get_info_cnf_t * getInfoCfm = (R_VOLATILE r_sys_get_info_cnf_t *)&g_syscb.getInfoCnf;
    *cnf = (r_sys_get_info_cnf_t *)getInfoCfm;

    R_DEMO_ClearEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_GET_INFO );

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Sys-GetInfo requesting... ");
#endif
    }

    status = R_SYS_GetInfoReq (req);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_SYS_GetInfoReq */
        R_DEMO_WaitEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_GET_INFO );

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_SYS_STATUS_SUCCESS == getInfoCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("success. \n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: 0x%.2X\n", getInfoCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_SysGetInfo */
/******************************************************************************
   End of function  R_DEMO_SysGetInfo
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_SysClearInfo
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_SysClearInfo (r_sys_clear_info_req_t *  req,
                                r_sys_clear_info_cnf_t ** cnf)
{
    r_result_t                          status;
    R_VOLATILE r_sys_clear_info_cnf_t * clearInfoCfm = (R_VOLATILE r_sys_clear_info_cnf_t *)&g_syscb.clearInfoCnf;
    *cnf = (r_sys_clear_info_cnf_t *)clearInfoCfm;

    R_DEMO_ClearEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_CLEAR_INFO );

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Sys-ClearInfo requesting... ");
#endif
    }

    status = R_SYS_ClearInfoReq (req);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_SYS_ClearInfoReq */
        R_DEMO_WaitEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_CLEAR_INFO );

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_SYS_STATUS_SUCCESS == clearInfoCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("success. \n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: 0x%.2X\n", clearInfoCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_SysClearInfo */
/******************************************************************************
   End of function  R_DEMO_SysClearInfo
******************************************************************************/




/*===========================================================================*/
/*    SROM access                                                            */
/*===========================================================================*/


/*===========================================================================*/
/*    APP for Statistics                                                     */
/*===========================================================================*/
/******************************************************************************
* Function Name: R_DEMO_AppSysGetStatistics
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_AppSysGetStatistics (r_sys_info_block_t block)
{
    uint32_t               i;
    const char *           pTxt;

    r_sys_get_info_req_t   req;
    r_sys_get_info_cnf_t * pCnf;

    req.infoType = R_SYS_INFO_TYPE_STATS;
    req.infoBlock = block;

    if (
        (R_DEMO_SysGetInfo (&req, &pCnf) != R_RESULT_SUCCESS) ||
        (R_SYS_STATUS_SUCCESS != pCnf->status)
        )
    {
        return R_RESULT_FAILED;
    }

    switch (block)
    {
        case R_SYS_INFO_BLOCK_UARTIF_0:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n   UIF Statistics\n");
#endif
            break;

        default:
            block = R_SYS_INFO_BLOCK_MAX;
            break;
    }

    if (R_SYS_INFO_BLOCK_MAX != block)
    {
        for (i = 0; i < (uint32_t)(pCnf->length >> 2); i++)
        {
            pTxt = R_SYS_Stats2Text ((uint8_t)block, i);
            if (NULL == pTxt)
            {
                break;
            }
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("%41s(%2d):%10lu\n", pTxt, i, pCnf->pinfo[i]);
#endif
        }
    }

    return R_RESULT_SUCCESS;
} /* R_DEMO_AppSysGetStatistics */
/******************************************************************************
   End of function  R_DEMO_AppSysGetStatistics
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_AppSysGetLog
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_AppSysGetLog (r_sys_info_block_t block)
{
    uint32_t               i;
    r_sys_get_info_req_t   req;
    r_sys_get_info_cnf_t * pCnf;

    req.infoType  = R_SYS_INFO_TYPE_LOGS;
    req.infoBlock = block;

    if (
        (uint32_t)(R_DEMO_SysGetInfo (&req, &pCnf) != R_RESULT_SUCCESS) ||
        (uint32_t)(R_SYS_STATUS_SUCCESS != pCnf->status)
        )
    {
        return R_RESULT_FAILED;
    }

    switch (block)
    {
        case R_SYS_INFO_BLOCK_UARTIF_0:
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("\n   UIF Log\n");
#endif
            break;

        default:
            break;
    }

    for (i = 0; i < (uint32_t)(pCnf->length >> 2); i += 2)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("      %4ld: 0x%08lX(%10lu[ms])    0x%08lX\n", i, pCnf->pinfo[i], pCnf->pinfo[i], pCnf->pinfo[i + 1]);
#endif
    }

    return R_RESULT_SUCCESS;
} /* R_DEMO_AppSysGetLog */
/******************************************************************************
   End of function  R_DEMO_AppSysGetLog
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_AppSysClearInfo
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_AppSysClearInfo (uint8_t type)
{
    r_sys_clear_info_req_t   req = {0}; // all zero(or all 1) mean all clear
    r_sys_clear_info_cnf_t * pCnf;

    req.infoTypeBit = type;

    if (
        (uint32_t)(R_DEMO_SysClearInfo (&req, &pCnf) != R_RESULT_SUCCESS) ||
        (uint32_t)(R_SYS_STATUS_SUCCESS != pCnf->status)
        )
    {
        return R_RESULT_FAILED;
    }
    return R_RESULT_SUCCESS;
} /* R_DEMO_AppSysClearInfo */
/******************************************************************************
   End of function  R_DEMO_AppSysClearInfo
******************************************************************************/

#if   !(defined __VANGO__)
/******************************************************************************
* Function Name: R_DEMO_SysSromRead
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_SysSromRead (r_sys_memread_req_t* req)
{
    r_result_t                    status;
    R_VOLATILE r_sys_memread_cnf_t * sromReadCfm = (R_VOLATILE r_sys_memread_cnf_t *)&g_syscb.sromReadCnf;
    uint32_t length;
    uint32_t addr;


    R_DEMO_ClearEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_SROM_READ );

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Sys-SromRead requesting... ");
#endif
    }

    status = R_SYS_SromReadReq (req);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_SYS_PingReq */
        R_DEMO_WaitEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_SROM_READ );

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_SYS_STATUS_SUCCESS == sromReadCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("\nReceived data (%ld bytes):\n+------------+-------------+-------------+-------------+-------------+------------------+", sromReadCfm->length);
#endif

                /* use intermediate local variables to avoid IAR warning: the order of volatile accesses is undefined in this statement */
                length = sromReadCfm->length;
                addr = sromReadCfm->addr;
                print_buff (sromReadCfm->pdata, length, addr);

#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("+------------+-------------+-------------+-------------+-------------+------------------+\n");
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: 0x%.2X\n", sromReadCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_SysSromRead */
/******************************************************************************
   End of function  R_DEMO_SysSromRead
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_SysSromWrite
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_SysSromWrite (r_sys_memwrite_req_t* req)
{
    r_result_t                          status;
    R_VOLATILE r_sys_memwrite_cnf_t *   sromWriteCfm = (R_VOLATILE r_sys_memwrite_cnf_t *)&g_syscb.sromWriteCnf;


    R_DEMO_ClearEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_SROM_WRITE );

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Sys-SromWrite requesting... ");
#endif
    }

    status = R_SYS_SromWriteReq (req);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_SYS_PingReq */
        R_DEMO_WaitEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_SROM_WRITE );

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_SYS_STATUS_SUCCESS == sromWriteCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("Write confirmation status %d\n", sromWriteCfm->status);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: 0x%.2X\n", sromWriteCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_SysSromWrite */
/******************************************************************************
   End of function  R_DEMO_SysSromWrite
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_SysSromErase
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_SysSromErase (r_sys_memerase_req_t* req)
{
    r_result_t                          status;
    R_VOLATILE r_sys_memerase_cnf_t *   sromEraseCfm = (R_VOLATILE r_sys_memerase_cnf_t *)&g_syscb.sromEraseCnf;


    R_DEMO_ClearEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_SROM_ERASE );

    /* Check if verbose is enabled */
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Sys-SromWrite requesting... ");
#endif
    }

    status = R_SYS_SromEraseReq (req);
    if (R_RESULT_SUCCESS != status)
    {
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("failed!");
#endif
        }

        return R_RESULT_FAILED;
    }
    else
    {
        /* Wait for completion of R_SYS_PingReq */
        R_DEMO_WaitEventFlag( &g_demo_sys_cb, R_DEMO_SYS_CB_FLAG_SROM_ERASE );

        /* Check if verbose is enabled */
        if (R_TRUE == g_demo_config.verboseEnabled)
        {
            if (R_SYS_STATUS_SUCCESS == sromEraseCfm->status)
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("Erase confirmation status %d\n", sromEraseCfm->status);
#endif
            }
            else
            {
#ifdef __PLC__DEBUG__
                R_STDIO_Printf ("failed. Status: 0x%.2X\n", sromEraseCfm->status);
#endif
            }
        }

        return R_RESULT_SUCCESS;
    }
} /* R_DEMO_SysSromErase */
/******************************************************************************
   End of function  R_DEMO_SysSromErase
******************************************************************************/

/******************************************************************************
* Function Name: show_char
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static uint8_t show_char (uint8_t c)
{
    uint8_t ch;

    if ((c < 0x20) || (c >= 0x7f))
    {
        ch = '.';
    }
    else
    {
        ch = c;
    }

    return (ch);
} /* show_char */
/******************************************************************************
   End of function  show_char
******************************************************************************/

/******************************************************************************
* Function Name: print_buff
* Description : outputs an easy to read HEX & ASCII dump of the given buffer
* Arguments :
* Return Value :
******************************************************************************/
static void print_buff (void * buff, uint32_t len, uint32_t start_addr)
{
    uint32_t i;
    uint32_t j;
    uint8_t strings[17];
    uint8_t line[128];
    uint32_t line_index;

    memset (strings, 0x20, sizeof (strings));

    for (i = 0; (i < len) && (i < 65536); i += 16)
    {
        line_index = (uint32_t)snprintf ((char *)line, sizeof (line), "\n| 0x%08x | ", (unsigned int)(start_addr + i));
        line_index = 14;

        for (j = 0; j < 16; j++)
        {
            if ( 0 == (j % 4) )
            {
                snprintf ((char *)&line[line_index], 3, "| ");
                line_index += (uint8_t)(2u);
            }

            if ((i + j) < len)
            {
                snprintf ((char *)&line[line_index], 4, "%02x ", ((uint8_t *)buff)[i + j]);
                line_index += (uint8_t)(3u);
                snprintf ((char *)&strings[j], 2, "%c", show_char (((uint8_t *)buff)[i + j]));
            }
            else
            {
                snprintf ((char *)&line[line_index], 4, "   ");
                line_index += (uint8_t)(3u);
                snprintf ((char *)&strings[j], 2, " ");
            }
        }

#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("%s| %s |", line, strings);
#endif
    }

#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("\n");
#endif
} /* print_buff */
/******************************************************************************
   End of function  print_buff
******************************************************************************/

/******************************************************************************
* Function Name: menu_sys_srom
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void menu_sys_srom (void)
{
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\f------------------------ S-ROM Menu ------------------------");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n r - Read baud rate setting and firmware header");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n b - Erase and set new baud rate parameter");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n e - Erase everything");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n c - Copy current firmware into S-ROM");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n z - Return");
#endif

        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

        if (strlen ((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
                case 'r':
                    menu_sys_srom_read ();
                    break;

                case 'b':
                    menu_sys_srom_set_baudrate ();
                    break;

                case 'e':
                    menu_sys_srom_erase ();
                    break;

                case 'c':
                    menu_sys_srom_copy_fw ();
                    break;

                case 'z':
                    return;

                default:
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n\n Invalid option! \n");
#endif
                    break;
            }

        }
    }
}/* menu_sys_srom */
/******************************************************************************
   End of function  menu_sys_srom
******************************************************************************/

/******************************************************************************
* Function Name: menu_sys_srom_read
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void menu_sys_srom_read (void)
{
    r_sys_memread_req_t srom_read_req;

#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("\n -> SYS-SROM-READ requesting... ");
#endif
#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("0x10000000 - 8 bytes: baudrate setting\n");
#endif

    srom_read_req.accessType = 1;
    srom_read_req.addr = R_SYS_SROM_ADDR_BAUD_RATE;
    srom_read_req.length = R_SYS_SROM_SIZE_BAUD_RATE;
    R_DEMO_SysSromRead (&srom_read_req);

#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("\n -> SYS-SROM-READ requesting... ");
#endif
#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("0x10010000 - 80 bytes: firmware header\n");
#endif

    srom_read_req.accessType = 1;
    srom_read_req.addr = R_SYS_SROM_ADDR_FW_HEADER;
    srom_read_req.length = R_SYS_SROM_SIZE_FW_HEADER;
    R_DEMO_SysSromRead (&srom_read_req);

#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("\n");
#endif
} /* menu_sys_srom_read */
/******************************************************************************
   End of function  menu_sys_srom_read
******************************************************************************/

/******************************************************************************
* Function Name: menu_sys_srom_set_baudrate
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void menu_sys_srom_set_baudrate (void)
{
    uint8_t data[8];
    uint32_t baudrate;
    r_sys_memerase_req_t srom_erase_req;
    r_sys_memwrite_req_t srom_write_req;

    baudrate = 0xFFFFFFFF;

    do
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Select baud-rate speed ...");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 0 - 9600");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 1 - 19200");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 2 - 38400");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 3 - 57600");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 4 - 115200");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 5 - 230400");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 6 - 300000");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 7 - 375000");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 8 - 460800");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 9 - 500000");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n a - 750000");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n b - 937500");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n c - 1000000 \n");
#endif

        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

        if (strlen ((char *)g_demo_buff.getStringBuffer) == 1)
        {
            baudrate = hex_string_to_uint32 (g_demo_buff.getStringBuffer);
            if (baudrate < 0xd)
            {
                break;
            }
        }
        else
        {
#ifdef __PLC__DEBUG__
            R_STDIO_Printf ("Invalid option");
#endif
        }
    }
    while (baudrate > 0xc);

    /* baud rate and baud rate inverted in big-endian format */
    R_BYTE_UInt32ToArr (baudrate, &data[0]);
    R_BYTE_UInt32ToArr ((uint32_t)(~baudrate), &data[4]);

    /* erase */
    srom_erase_req.addr = R_SYS_SROM_ADDR_BAUD_RATE;
    srom_erase_req.length = R_SYS_SROM_SIZE_ERASE_SECTOR;
    R_DEMO_SysSromErase (&srom_erase_req);

    /* write new */
    srom_write_req.accessType = 1;
    srom_write_req.addr = R_SYS_SROM_ADDR_BAUD_RATE;
    srom_write_req.length = R_SYS_SROM_SIZE_BAUD_RATE;
    srom_write_req.pdata = data;
    R_DEMO_SysSromWrite (&srom_write_req);
} /* menu_sys_srom_set_baudrate */
/******************************************************************************
   End of function  menu_sys_srom_set_baudrate
******************************************************************************/

/******************************************************************************
* Function Name: menu_sys_srom_erase
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void menu_sys_srom_erase (void)
{
    r_sys_memerase_req_t srom_erase_req;

    srom_erase_req.length = R_SYS_SROM_SIZE_ERASE_GROUP;
    for (srom_erase_req.addr = R_SYS_SROM_ADDR_BASE; srom_erase_req.addr < R_SYS_SROM_ADDR_TOP; srom_erase_req.addr += R_SYS_SROM_SIZE_ERASE_GROUP)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("Erase at S-ROM address: 0x%08x\n", srom_erase_req.addr);
#endif
        R_DEMO_SysSromErase (&srom_erase_req);
    }
} /* menu_sys_srom_erase */
/******************************************************************************
   End of function  menu_sys_srom_erase
******************************************************************************/

/******************************************************************************
* Function Name: menu_sys_srom_copy_fw
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void menu_sys_srom_copy_fw (void)
{
    uint32_t    i;
    uint32_t    srom_addr;
    uint8_t    *psrc;
    uint32_t    length;
    r_sys_memwrite_req_t srom_write_req;

    
    psrc = (uint8_t *)g_cpxprogtbl;


    /* calculate firmware size */
    length = R_SYS_SROM_SIZE_FW_HEADER;
    length += (((((uint32_t)psrc[24] << 0) + ((uint32_t)psrc[25] << 8)) + ((uint32_t)psrc[26] << 16)) + ((uint32_t)psrc[27] << 24));
    length += (((((uint32_t)psrc[40] << 0) + ((uint32_t)psrc[41] << 8)) + ((uint32_t)psrc[42] << 16)) + ((uint32_t)psrc[43] << 24));
    length += (((((uint32_t)psrc[56] << 0) + ((uint32_t)psrc[57] << 8)) + ((uint32_t)psrc[58] << 16)) + ((uint32_t)psrc[59] << 24));
    length += (((((uint32_t)psrc[72] << 0) + ((uint32_t)psrc[73] << 8)) + ((uint32_t)psrc[74] << 16)) + ((uint32_t)psrc[75] << 24));

#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("Firmware size: %d bytes\n", length);
#endif

    srom_addr = R_SYS_SROM_ADDR_FW_HEADER;
    for (i = 0; i < length; i += R_SYS_SROM_SIZE_WRITE_PAGE_MAX, srom_addr += R_SYS_SROM_SIZE_WRITE_PAGE_MAX)
    {
        srom_write_req.accessType = 1;
        srom_write_req.addr = srom_addr;
        srom_write_req.length = R_SYS_SROM_SIZE_WRITE_PAGE_MAX;
        srom_write_req.pdata = (uint8_t *)&psrc[i];

#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("Write at S-ROM address: 0x%08x\n", srom_addr);
#endif

        R_DEMO_SysSromWrite (&srom_write_req);
    }
} /* menu_sys_srom_copy_fw */
/******************************************************************************
   End of function  menu_sys_srom_copy_fw
******************************************************************************/
#endif  /* #if !(defined R_DEFINE_CPX_SROM_UNUSE) &&  !(defined R_RL78_PLC) */



/******************************************************************************
* Function Name: menu_sys_statistics
* Description :
* Arguments :
* Return Value :
******************************************************************************/
static void menu_sys_statistics (void)
{
    /* Configuration Menu */
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\f-------------------Configuration Menu-----------------------");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 0 - Clear CPX3 UIF Statistics");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 1 - Clear CPX3 UIF Log");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 2 - Get CPX3 UIF Statistics");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 3 - Get CPX3 UIF Log");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n z - Return");
#endif

        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

        if (strlen ((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
                case '0':
                    R_DEMO_AppSysClearInfo (R_SYS_INFO_TYPE_BIT_STATS);
                    break;

                case '1':
                    R_DEMO_AppSysClearInfo (R_SYS_INFO_TYPE_BIT_LOGS);
                    break;

                case '2':
                    R_DEMO_AppSysGetStatistics (R_SYS_INFO_BLOCK_UARTIF_0);
                    break;

                case '3':
                    R_DEMO_AppSysGetLog (R_SYS_INFO_BLOCK_UARTIF_0);
                    break;

                case 'z':
                    return;

                default:
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n\n Invalid option! \n");
#endif
                    break;
            } /* switch */
        }
    }
} /* menu_sys_statistics */
/******************************************************************************
   End of function  menu_sys_statistics
******************************************************************************/



/******************************************************************************
* Function Name: R_DEMO_AppSysMenu
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_DEMO_AppSysMenu (void)
{
    /* Configuration Menu */
    while (1)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\f------------------------System Menu-------------------------");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 0 - ping");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 1 - version");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 7 - Statistics/Log");
#endif
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n 9 - reboot");
#endif
#if  !(defined __VANGO__)
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n s - S-ROM menu");
#endif
#endif  /* #if ( !(defined R_DEFINE_CPX_SROM_UNUSE) && !(defined R_RL78_PLC) ) */
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n z - Return");
#endif

        R_STDIO_Gets ((char *)g_demo_buff.getStringBuffer);

        if (strlen ((char *)g_demo_buff.getStringBuffer) == 1)
        {
            switch (g_demo_buff.getStringBuffer[0])
            {
                case '0':
                {
                    r_sys_ping_cnf_t * pCnf;
                    R_DEMO_SysPing (&pCnf);
                    break;
                }

                case '1':
                {
                    r_sys_version_cnf_t * pCnf;
                    R_DEMO_SysVersion (&pCnf);
                    break;
                }

                case '7':
                    menu_sys_statistics ();
                    break;

                case '9':
                    R_DEMO_SoftReset ();

                    break;

#if  !(defined __VANGO__)
                case 's':
                    menu_sys_srom ();
                    break;
#endif  /* #if ( !(defined R_DEFINE_CPX_SROM_UNUSE) && !(defined R_RL78_PLC) ) */


                case 'z':
                    return R_RESULT_SUCCESS;

                default:
#ifdef __PLC__DEBUG__
                    R_STDIO_Printf ("\n\n Invalid option! \n");
#endif
                    break;
            } /* switch */

        }
    }
} /* R_DEMO_AppSysMenu */
/******************************************************************************
   End of function  R_DEMO_AppSysMenu
******************************************************************************/


/******************************************************************************
* Function Name: R_DEMO_AppHandleSysRebootReqInd
* Description :
* Arguments :
* Return Value :
******************************************************************************/
void R_DEMO_AppHandleSysRebootReqInd (const r_sys_rebootreq_ind_t * ind)
{
    uint8_t buffer[1];
    
    if (R_TRUE == g_demo_config.verboseEnabled)
    {
#ifdef __PLC__DEBUG__
        R_STDIO_Printf ("\n -> Received SYS-ReBootRequest indication with status value: 0x%.2X", ind->status);
#endif
    }
    R_DEMO_LED(R_DEMO_G3_USE_SECONDARY_CH, R_DEMO_LED_ALERT);
#ifdef __PLC__DEBUG__
    R_STDIO_Printf ("\n -> Press Enter to reboot cpx!");
#endif
    R_STDIO_Gets ((char *)buffer);
    R_DEMO_LED(R_DEMO_G3_USE_SECONDARY_CH, R_DEMO_LED_STOP);
    
    /* Call Reboot Routine. */
    R_DEMO_ModemReboot();

}
/******************************************************************************
   End of function  R_DEMO_AppHandleSysRebootReqInd
******************************************************************************/

