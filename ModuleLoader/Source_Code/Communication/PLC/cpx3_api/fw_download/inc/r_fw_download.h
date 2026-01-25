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

/******************************************************************************
* File Name     : r_fw_download.h
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : Sample software
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_FW_DOWNLOAD_H
#define R_FW_DOWNLOAD_H
/******************************************************************************
Include
******************************************************************************/
#include "r_c3sap_api.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_FWDL_INFO_TABLE_UNIQUE_CODE_SIZE (12)
#define R_FWDL_INFO_TABLE_CHKSUM_SIZE       (4)

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/
/***********************************************************************
* Function Name     : R_FW_Download
* Description       : Top level function for the FW DL
* Argument          : boot_info : Pointer to configuration structure
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
/*!
   \fn          r_result_t R_FW_Download(r_sys_boot_info_t *boot_info)
   \brief       Top level function for the FW DL
   \param[in]   boot_info Pointer to configuration structure
   \return      R_RESULT_SUCCESS if the FW DL is successful, else R_RESULT_FAILED
 */
r_result_t R_FW_Download (r_sys_boot_info_t * boot_info);

/******************************************************************************
* Function Name    : R_FW_CheckFwMatch
* Description      : PLC Fw and RF FW version match check
* Arguments        : p_fw_plc : pointer to PLC FW, p_fw_rf : pointer to RF FW
* Return Value     : R_RESULT_SUCCESS or R_RESULT_FAILED
******************************************************************************/
/*!
   \fn          R_FW_CheckFwMatch (const uint8_t * p_fw_plc, const uint8_t * p_fw_rf)
   \brief       PLC FW and RF FW version match check
   \param[in]   p_fw_plc : pointer to PLC FW, p_fw_rf : pointer to RF FW
   \return      R_RESULT_SUCCESS if the Fw version matches, else R_RESULT_FAILED
 */
r_result_t R_FW_CheckFwMatch (const uint8_t * p_fw_plc, const uint8_t * p_fw_rf);


#endif /* R_FW_DOWNLOAD_H */

