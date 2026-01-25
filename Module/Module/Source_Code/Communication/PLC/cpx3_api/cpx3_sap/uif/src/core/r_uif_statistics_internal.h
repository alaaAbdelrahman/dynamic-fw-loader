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
* File Name   : r_uif_statistics_internal.h
*    @version
*        $Rev: 3177 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2017-04-26 14:59:37 +0900#$
* Description :
******************************************************************************/


/*                                                                  COMMENT_E  */

/*!
   \file      r_uif_statistics_internal.h
   \version   1.00
   \brief     This is the internal header file for the statistics module
 */

/*
 * Prevent nested inclusions
 */
#ifndef R_UIF_STATISTICS_INTERNAL_H
#define R_UIF_STATISTICS_INTERNAL_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_uif_statistics.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_UIF_STATS_INC(x, y)  (R_UIF_StatisticsIncrement ((r_uif_stats_info_t **)(x), 0u, (y)))

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef enum
{
    R_UIF_STATS_GROUP_RXTX = 0x00,
    R_UIF_STATS_GROUP_END,
} r_uif_statistics_group_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/


#endif /* R_UIF_STATISTICS_INTERNAL_H */

