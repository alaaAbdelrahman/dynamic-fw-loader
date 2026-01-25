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
* File Name   : r_demo_metric_computation.h
*    @version
*        $Rev: 7496 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2021-09-24 13:40:27 +0900#$
* Description :
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_DEMO_METRIC_COMPUTATION_H
#define R_DEMO_METRIC_COMPUTATION_H

/******************************************************************************
* Defines
******************************************************************************/
#define R_MAX_LQI_VALUE                  (0xFFu) /*!< Maximum value of LQI */
#define R_MAX_LINK_COST_VALUE            (0xFFu) /*!< Maximum link cost value */
/******************************************************************************
* Typedefs
******************************************************************************/

/*!
    \struct r_loadng_ib_info_t
    \brief Structure of ib for loadng
 */
typedef struct
{
    uint8_t         adpMetricType;          /*!< Metric Type to be used for routing purposes */
    uint8_t         adpLowLQIValue;         /*!< The low LQI value defines the LQI value, used in metric computation, below which a link to a neighbour is considered as an unreliable link. This value shall be lower than adpHighLQIValue. */
    uint8_t         adpHighLQIValue;        /*!< The high LQI value defines the LQI value, used in metric computation, above which a link to a neighbour is considered as a reliable link. This value is greater than adpLowLQIValue. */
    uint8_t         adpKq;                  /*!< A weight factor for LQI to calculate route cost */
    uint8_t         adpKh;                  /*!< A weight factor for hop to calculate link cost */
} r_loadng_ib_info_t;


/*!
    \struct r_loadng_link_info_t
    \brief Structure for storing of link information
 */
typedef struct
{
    uint8_t              lqi;       /*!< Link quality indicator */
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    uint8_t              mediaType;
#endif

} r_loadng_link_info_t;

/******************************************************************************
   Functions
******************************************************************************/

/*!
    \fn uint8_t R_DEMO_ComputeDirectionalLinkCost(const r_loadng_ib_info_t* ibInfo, const r_loadng_link_info_t* linkInfo);
    \brief Computes the G3 directional link cost
 */
uint8_t R_DEMO_ComputeDirectionalLinkCost (const r_loadng_ib_info_t * ibInfo, const r_loadng_link_info_t * linkInfo);

#endif             /* R_LOADNG_METRIC_COMPUTATION_H */

