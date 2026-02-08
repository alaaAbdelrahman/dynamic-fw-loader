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
* File Name   : r_queue_api.c
*    @version
*        $Rev: 7945 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2021-11-30 17:20:39 +0900#$
* Description : 
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <stdlib.h>
#include "r_typedefs.h"
#include "r_config.h"
#include "r_queue_api.h"
#include "r_bsp_api.h"
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
/******************************************************************************
* Static variables
******************************************************************************/

/******************************************************************************
* Local function headers
******************************************************************************/

/******************************************************************************
* Extern variables
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
Function implementations
******************************************************************************/

/******************************************************************************
* Function Name:R_QUEUE_Create
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_queue_result_t R_QUEUE_Create(r_queue_t* pqueue,
                                r_queue_element_t array[],
                                uint16_t max_length)
{
    if (((NULL == array) || (NULL == pqueue)) || (0u == max_length))
    {
        return R_QUEUE_RESULT_FAILED;
    }
    else
    {
        /* do nothing */
    }

    pqueue->length     = 0u;
    pqueue->wi         = 0u;
    pqueue->ri         = 0u;
    pqueue->max_length = max_length;
    pqueue->parray     = array;

    return R_QUEUE_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_QUEUE_Create
******************************************************************************/

/******************************************************************************
* Function Name:R_QUEUE_Flush
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_queue_result_t R_QUEUE_Flush(r_queue_t* pqueue)
{
    if (NULL == pqueue)
    {
        return R_QUEUE_RESULT_FAILED;
    }
    else
    {
        /* do nothing */
    }

    pqueue->length = 0u;
    pqueue->wi     = 0u;
    pqueue->ri     = 0u;

    return R_QUEUE_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_QUEUE_Flush
******************************************************************************/

/******************************************************************************
* Function Name:R_QUEUE_IsFull
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_boolean_t R_QUEUE_IsFull(r_queue_t* pqueue)
{
    if (NULL == pqueue)
    {
        return R_FALSE; /* bad parameters */
    }
    else
    {
        /* do nothing */
    }

    if (pqueue->length == pqueue->max_length)
    {
        return R_TRUE;
    }
    else
    {
        return R_FALSE;
    }
}
/******************************************************************************
   End of function  R_QUEUE_IsFull
******************************************************************************/

/******************************************************************************
* Function Name:R_QUEUE_Enqueue
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_queue_result_t R_QUEUE_Enqueue(r_queue_t* pqueue,
                                 const uint8_t* pelement,
                                 uint16_t size,
                                 uint8_t handle)
{
    if ((NULL == pqueue) || ((NULL != pelement) && 
                             (R_BSP_CheckInStack(pelement) == R_RESULT_SUCCESS)))
    {
        return R_QUEUE_RESULT_FAILED;
    }
    else
    {
        /* do nothing */
    }

    /* Disable interrupts */
    R_BSP_DI();
    if (pqueue->length < pqueue->max_length)
    {
        pqueue->parray[pqueue->wi].pdata  = pelement;
        pqueue->parray[pqueue->wi].size   = size;
        pqueue->parray[pqueue->wi].handle = handle;

        pqueue->wi++;
        pqueue->length++;
        if (pqueue->wi == pqueue->max_length)
        {
            pqueue->wi = 0u;
        }
        else
        {
            /* do nothing */
        }

        /* Enable interrupts */
         R_BSP_EI();
        return R_QUEUE_RESULT_SUCCESS;
    }
    else
    {
        /* do nothing */
    }

    /* Enable interrupts */
    R_BSP_EI();
    return R_QUEUE_RESULT_FULL;
}
/******************************************************************************
   End of function  R_QUEUE_Enqueue
******************************************************************************/

/******************************************************************************
* Function Name:R_QUEUE_Dequeue
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_queue_result_t R_QUEUE_Dequeue(r_queue_t*           pqueue,
                                 r_queue_element_t*   pdequeue)
{
    if ((NULL == pqueue) || (NULL == pdequeue))
    {
        return R_QUEUE_RESULT_FAILED;
    }
    else
    {
        /* do nothing */
    }

    /* Disenable interrupts */
    R_BSP_DI();
    if (0u == pqueue->length)
    {

        /* Enable interrupts */
        R_BSP_EI();
        return R_QUEUE_RESULT_EMPTY;
    }
    else
    {
        /* do nothing */
    }
    pdequeue->pdata  = pqueue->parray[pqueue->ri].pdata;
    pdequeue->size   = pqueue->parray[pqueue->ri].size;
    pdequeue->handle = pqueue->parray[pqueue->ri].handle;
    pqueue->length--;
    pqueue->ri++;

    if (pqueue->ri == pqueue->max_length)
    {
        pqueue->ri = 0;
    }
    else
    {
        /* do nothing */
    }

    /* Enable interrupts */
    R_BSP_EI();

    return R_QUEUE_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_QUEUE_Dequeue
******************************************************************************/
