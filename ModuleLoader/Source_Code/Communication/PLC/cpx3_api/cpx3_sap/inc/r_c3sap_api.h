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
 * File Name : r_c3sap_api.h
 * Description : CPX3 SAP API prototype
 ******************************************************************************/

#ifndef R_C3SAP_API_H
#define R_C3SAP_API_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#if defined(R_DEFINE_OPEN_RX)
#include "r_sys3_api.h"
#else
#include "r_sys_api.h"
#endif

//#include "mcu_info.h"
#include "r_g3_api.h"
/******************************************************************************
Macro definitions
******************************************************************************/

#if (defined BSP_MCU_RX631)
#define R_SYS_BAUD_9600     (0u)
#define R_SYS_BAUD_19200    (1u)
#define R_SYS_BAUD_38400    (2u)
#define R_SYS_BAUD_57600    (3u)
#define R_SYS_BAUD_115200   (4u)
#define R_SYS_BAUD_230400   (5u)
#define R_SYS_BAUD_300000   (6u)
#define R_SYS_BAUD_375000   (7u)
//#define R_SYS_BAUD_460800   (8u)        /* PCLK needs to be changed to 96MHz */
#define R_SYS_BAUD_500000   (9u)
#define R_SYS_BAUD_750000   (10u)
#define R_SYS_BAUD_937500   (11u)
#define R_SYS_BAUD_1000000  (12u)

#elif (defined BSP_MCU_RX651) || (defined BSP_MCU_RX65N)
#define R_SYS_BAUD_9600     (0u)
#define R_SYS_BAUD_19200    (1u)
#define R_SYS_BAUD_38400    (2u)
#define R_SYS_BAUD_57600    (3u)
#define R_SYS_BAUD_115200   (4u)
#define R_SYS_BAUD_230400   (5u)
#define R_SYS_BAUD_300000   (6u)
#define R_SYS_BAUD_375000   (7u)
#define R_SYS_BAUD_460800   (8u)
#define R_SYS_BAUD_500000   (9u)
#define R_SYS_BAUD_750000   (10u)
#define R_SYS_BAUD_937500   (11u)
#define R_SYS_BAUD_1000000  (12u)

#elif (defined BSP_MCU_RX231)
#define R_SYS_BAUD_115200   (4u)
#define R_SYS_BAUD_500000   (9u)
#define R_SYS_BAUD_1000000  (12u)

#elif (defined R_RL78_PLC)
#define R_SYS_BAUD_9600     (0u)
#define R_SYS_BAUD_19200    (1u)
#define R_SYS_BAUD_38400    (2u)
#define R_SYS_BAUD_57600    (3u)
#define R_SYS_BAUD_115200   (4u)
#define R_SYS_BAUD_230400   (5u)
#define R_SYS_BAUD_300000   (6u)
#define R_SYS_BAUD_375000   (7u)
#define R_SYS_BAUD_460800   (8u)
#define R_SYS_BAUD_500000   (9u)
#define R_SYS_BAUD_750000   (10u)
//#define R_SYS_BAUD_937500   (11u)
#define R_SYS_BAUD_1000000  (12u)

#elif (defined BSP_MCU_GROUP_RA6M4)
#define R_SYS_BAUD_115200   (4u)
#define R_SYS_BAUD_500000   (9u)
#define R_SYS_BAUD_1000000  (12u)
#endif

#define R_SYS_BAUD_MAX      (13u)



#endif /* R_C3SAP_API_H */

