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
* File Name     : r_constants.h
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : SAP layer constants
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_CONSTANTS_H
#define R_CONSTANTS_H


/******************************************************************************
Macro definitions
******************************************************************************/

    #define R_HANDLE_DONT_CARE        (0x00u)                  /*!< R_HANDLE_DONT_CARE */
    #define R_HANDLE_CPX_FIRST        (0x01u)                  /*!< R_HANDLE_CPX_FIRST */
    #define R_HANDLE_SYS_REQUEST      (0x01u)                  /*!< R_HANDLE_SYS_BOOT_REQUEST */
    #define R_HANDLE_G3_FIRST         (0x02u)                  /*!< R_HANDLE_G3_FIRST */
    #define R_HANDLE_G3_REQUEST       (0x02u)                  /*!< R_HANDLE_G3_REQ */
    #define R_HANDLE_CPX_LAST         (0x02u)                  /*!< R_HANDLE_CPX_LAST */
    #define R_MAX_USED_TIMER_HANDLES  (R_HANDLE_CPX_LAST + 1u) /*!< + 1 due to starting from 0 */

/******************************************************************************
Typedef definitions
******************************************************************************/

/* Defines the handles */

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

#endif /* R_CONSTANTS_H */

