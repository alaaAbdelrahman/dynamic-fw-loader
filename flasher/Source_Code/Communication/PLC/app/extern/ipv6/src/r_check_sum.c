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
* File Name   : r_check_sum.c
*    @version
*        $Rev: 3113 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2017-04-14 21:00:02 +0900#$
* Description : 
******************************************************************************/

/******************************************************************************
Includes
******************************************************************************/
#include <string.h>
#include "r_typedefs.h"
#include "r_io_vec.h"
#include "r_check_sum.h"
#include "r_byte_swap.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Private global variables and functions
******************************************************************************/
/******************************************************************************
Exported global variables
******************************************************************************/
/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Functions
******************************************************************************/



/******************************************************************************
* Function Name: R_CS_ComputeCheckSum
* Description :
* Arguments :
* Return Value :
******************************************************************************/
r_result_t R_CS_ComputeCheckSum(const r_iovec_class_t* const ioVecClass,
                                uint16_t* const checkSum)
{
    uint16_t       i;
    uint16_t       count;
    uint32_t       sum  = 0;
    const uint32_t mask = 0xFFFF;
    const uint8_t* pAddr;

    if ((NULL == ioVecClass) || (NULL == checkSum))
    {
        return R_RESULT_ILLEGAL_NULL_POINTER;
    }

    for (i = 0 ; i < ioVecClass->used_elements ; i++)
    {
        count   = (uint16_t)(ioVecClass->pio_vec[i].length);
        pAddr = ioVecClass->pio_vec[i].paddress;

        if (NULL == pAddr)
        {
            return R_RESULT_FAILED;
        }

        while (count > 1u)
        {
            sum   += R_BYTE_ArrToUInt16(pAddr);
            pAddr += 2u;
            count  = (uint16_t)(count - 2u);
        }

        /* Add left-over byte if present */
        if (count > 0u)
        {
            sum += ((uint32_t)(*pAddr) << 8u);
        }
    }

    /* Fold 32-bit sum to 16 bits */
    while ((sum >> 16u) != 0UL)
    {
        sum = (sum & mask) + (sum >> 16u);
    }

    *checkSum = (uint16_t) ~sum;

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_CS_ComputeCheckSum
******************************************************************************/

