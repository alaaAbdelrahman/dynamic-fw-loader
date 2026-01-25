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
* File Name   : r_demo_metric_computation.c
*    @version
*        $Rev: 7496 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2021-09-24 13:40:27 +0900#$
* Description :
******************************************************************************/


/******************************************************************************
   Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_typedefs.h"

/* g3 part */
#include "r_g3_sap.h"
#include "r_g3rfmac_sap.h"
#include "r_g3hybrid.h"
#include "r_g3mac_sap.h"
#include "r_adp_sap.h"
#include "r_demo_metric_computation.h"

/******************************************************************************
   Typedef definitions
******************************************************************************/

/******************************************************************************
   Macro definitions
******************************************************************************/

/******************************************************************************
   Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
   Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
   Private global variables
******************************************************************************/

/******************************************************************************
   Private global functions
******************************************************************************/


/*!
    \fn static uint16_t R_LOADNG_Computedefault_link_cost_equation(const r_loadng_ib_info_t* ibInfo, const r_loadng_link_info_t* linkInfo);
    \brief Computes G3 default link cost
 */
static uint16_t default_link_cost_equation (const r_loadng_ib_info_t * ibInfo, const r_loadng_link_info_t * linkInfo);

/*!
    \fn static uint16_t R_LOADNG_Computecapacity_link_cost_equation(const r_loadng_link_info_t* linkInfo);
    \brief Computes ERDFs capacity based link cost
 */
static uint16_t capacity_link_cost_equation (const r_loadng_link_info_t * linkInfo);

/******************************************************************************
* Functions
******************************************************************************/

/******************************************************************************
* Function Name: default_link_cost_equation
* Description : Computes G3 default link cost
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t default_link_cost_equation (const r_loadng_ib_info_t * ibInfo, const r_loadng_link_info_t * linkInfo)
{
    uint8_t  highMinusLowLqiValue;
    uint8_t  highMinusCurrentLqiValue;
    uint16_t link_cost;

    /* NULL pointer check. */
    if (NULL == linkInfo)
    {
        return 0xFF;
    }


    /* Taken from Annex B:
       Link Cost = AdpKr*MOD_Kr+ AdpKm*MOD_Km
     + AdpKc*(( Maximum Number of Tones number of active tones))/(Maximum Number of Tones)
     + AdpKq*((Maximum LQI LQI))/(Maximum LQI)  + AdpKh
     + AdpKrt*(number of active routes)/(Maximum number of active routes)
     */


    /* Compute link cost. */
    if (ibInfo->adpHighLQIValue > ibInfo->adpLowLQIValue)
    {
        /* Compute difference. */
        highMinusLowLqiValue = (uint8_t)(ibInfo->adpHighLQIValue - ibInfo->adpLowLQIValue);
    }
    else
    {
        /* Invalid, return error. */
        return 0xFF;
    }

    if (ibInfo->adpHighLQIValue > linkInfo->lqi)
    {
        /* Compute difference. */
        highMinusCurrentLqiValue = (uint8_t)(ibInfo->adpHighLQIValue - linkInfo->lqi);
    }
    else
    {
        /* Optimum LQI reached, set difference to zero. */
        highMinusCurrentLqiValue = 0u;
    }

    if (highMinusCurrentLqiValue > highMinusLowLqiValue)
    {
        /* Set to maximum. */
        highMinusCurrentLqiValue = highMinusLowLqiValue;
    }

    link_cost = (uint16_t)(((uint32_t)ibInfo->adpKq * highMinusCurrentLqiValue) / highMinusLowLqiValue);

    return link_cost;
} /* default_link_cost_equation */
/******************************************************************************
   End of function  default_link_cost_equation
******************************************************************************/

/******************************************************************************
* Function Name: capacity_link_cost_equation
* Description : Computes ERDFs capacity based link cost
* Arguments :
* Return Value :
******************************************************************************/
static uint16_t capacity_link_cost_equation (const r_loadng_link_info_t * linkInfo)
{
    uint16_t directionalLinkCost = 200; // Default link cost

    return directionalLinkCost;
} /* capacity_link_cost_equation */
/******************************************************************************
   End of function  capacity_link_cost_equation
******************************************************************************/

/******************************************************************************
* Function Name: R_DEMO_ComputeDirectionalLinkCost
* Description : Computes the G3 directional link cost
* Arguments :
* Return Value :
******************************************************************************/
uint8_t R_DEMO_ComputeDirectionalLinkCost (const r_loadng_ib_info_t * ibInfo, const r_loadng_link_info_t * linkInfo)
{
    uint16_t directionalLinkCost;

    /* Check which metric type is used. */
    if (R_LOADNG_DEFAULT_METRIC == ibInfo->adpMetricType)
    {
        /* Compute default link cost. */
        directionalLinkCost = default_link_cost_equation (ibInfo, linkInfo);
    }
    else if (
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
        (R_MEDIA_TYPE0_PLC == linkInfo->mediaType) &&
#endif
        (R_LOADNG_CAPACITY_METRIC == ibInfo->adpMetricType)
        )
    {
        /* Compute capacity link cost. */
        directionalLinkCost = (uint16_t)(capacity_link_cost_equation (linkInfo) + ibInfo->adpKh);
    }
    else if (R_LOADNG_HOPCOUNT_METRIC == ibInfo->adpMetricType)
    {
        directionalLinkCost = 1;
    }
    else
    {
        /* If not known or HOP_COUNT, set to maximum (Section 11.2). */
        directionalLinkCost = R_MAX_LINK_COST_VALUE;
    }

    /* Maximum value for link cost is 0xFF. Truncate if needed. */
    if (directionalLinkCost > R_MAX_LINK_COST_VALUE)
    {
        directionalLinkCost = R_MAX_LINK_COST_VALUE;
    }

    return (uint8_t)directionalLinkCost;
} /* R_DEMO_ComputeDirectionalLinkCost */
/******************************************************************************
   End of function  R_DEMO_ComputeDirectionalLinkCost
******************************************************************************/

