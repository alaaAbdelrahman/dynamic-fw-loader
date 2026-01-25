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
* http://www.renesas.com/disclaimer*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
 * File Name   : r_io_vec.c
 * Version     : 1.0
 * Description : This module implements input/ouput vector structures
 ******************************************************************************/

/*!
   \file      r_io_vec.c
   \version   1.00
   \brief     This module implements input/ouput vector structures
 */

/******************************************************************************
   Includes   <System Includes> , Project Includes
******************************************************************************/
#include <string.h>
#include <stdlib.h>
#include "r_typedefs.h"
#include "r_io_vec.h"


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
   Function implementations
******************************************************************************/

/******************************************************************************
* Function Name:R_IO_VecRese
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void R_IO_VecRese(r_iovec_class_t* pio_vec_class)
{
    uint8_t cnt;

    if ((NULL == pio_vec_class) ||
        (NULL == pio_vec_class->pio_vec)) 
    {
        return;
    }

    /* Reset all class and vector elements */
    pio_vec_class->used_elements = 0;
    pio_vec_class->total_length  = 0;

    for (cnt = 0 ; cnt < pio_vec_class->num_elements ; cnt++)
    {
        pio_vec_class->pio_vec[cnt].paddress = 0;
        pio_vec_class->pio_vec[cnt].length  = 0;
    }
}
/******************************************************************************
   End of function  R_IO_VecRese
******************************************************************************/

/******************************************************************************
* Function Name:R_IO_VecAppendElement
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_IO_VecAppendElement(r_iovec_class_t* pio_vec_class,
                              const uint8_t* paddress,
                              const uint16_t length)
{
    if ((NULL == pio_vec_class) ||
        (NULL == pio_vec_class->pio_vec))
    {
        return R_RESULT_FAILED;
    }

    /* Check if more elements can be assigned */
    if (pio_vec_class->used_elements < pio_vec_class->num_elements)
    {
        pio_vec_class->used_elements++;
        pio_vec_class->pio_vec[pio_vec_class->used_elements - 1u].paddress = paddress;
        pio_vec_class->pio_vec[pio_vec_class->used_elements - 1u].length  = length;

        pio_vec_class->total_length                                += length;

        return R_RESULT_SUCCESS;
    }
    else
    {
        return R_RESULT_FAILED;
    }
}
/******************************************************************************
   End of function  R_IO_VecAppendElement
******************************************************************************/

/******************************************************************************
* Function Name:R_IO_VecPrependElement
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_IO_VecPrependElement(r_iovec_class_t* pio_vec_class,
                               const uint8_t* paddress,
                               const uint16_t length)
{
    uint8_t cnt;

    if ((NULL == pio_vec_class) ||
        (NULL == pio_vec_class->pio_vec))
    {
        return R_RESULT_FAILED;
    }

    /* Check if more elements can be assigned */
    if (pio_vec_class->used_elements < pio_vec_class->num_elements)
    {
        for (cnt = pio_vec_class->used_elements ; cnt > 0u ; cnt--)
        {
            pio_vec_class->pio_vec[cnt].paddress = pio_vec_class->pio_vec[cnt - 1u].paddress;
            pio_vec_class->pio_vec[cnt].length  = pio_vec_class->pio_vec[cnt - 1u].length;
        }

        pio_vec_class->used_elements++;
        pio_vec_class->pio_vec[0].paddress = paddress;
        pio_vec_class->pio_vec[0].length  = length;

        pio_vec_class->total_length     += length;

        return R_RESULT_SUCCESS;
    }
    else
    {
        return R_RESULT_FAILED;
    }
}
/******************************************************************************
   End of function  R_IO_VecPrependElement
******************************************************************************/

/******************************************************************************
* Function Name:R_IO_VecToContinous
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_IO_VecToContinous(const r_iovec_class_t* pio_vec_class,
                            uint8_t* buffer,
                            const uint32_t bufferLength,
                            uint32_t* pwritten_bytes)
{
    uint8_t  cnt;
    uint32_t offset = 0;

    if (((NULL == pio_vec_class)  || (NULL == buffer) ) || 
        ((NULL == pwritten_bytes) || (NULL == pio_vec_class->pio_vec)) )
    {
        return R_RESULT_FAILED;
    }

    /* Check if buffer length is sufficient */
    if (pio_vec_class->total_length > bufferLength)
    {
        (*pwritten_bytes) = 0;

        return R_RESULT_FAILED;
    }
    else
    {
        /* Iterate over all vector elements */
        for (cnt = 0 ; cnt < pio_vec_class->used_elements ; cnt++)
        {
            R_memcpy((buffer + offset), pio_vec_class->pio_vec[cnt].paddress, pio_vec_class->pio_vec[cnt].length);

            offset += pio_vec_class->pio_vec[cnt].length;
        }

        (*pwritten_bytes) = offset;

        return R_RESULT_SUCCESS;
    }
}
/******************************************************************************
   End of function  R_IO_VecToContinous
******************************************************************************/

/******************************************************************************
* Function Name:R_IO_VecCopy
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_IO_VecCopy(const r_iovec_class_t* psrc_io_vec_class,
                     r_iovec_class_t* pdst_io_vec_class)
{
    uint8_t    cnt    = 0;
    r_result_t result = R_RESULT_SUCCESS;

    if (( (NULL == psrc_io_vec_class) ||
          (NULL == psrc_io_vec_class->pio_vec) ) ||
        ( (NULL == pdst_io_vec_class) ||
          (NULL == pdst_io_vec_class->pio_vec) ) )
    {
        return R_RESULT_FAILED;
    }

    /* Check if a sufficient number of elements is available in destination */
    if (psrc_io_vec_class->used_elements > (pdst_io_vec_class->num_elements - pdst_io_vec_class->used_elements))
    {
        result = R_RESULT_FAILED;
    }
    else
    {
        /* Append elements from psrc_io_vec_class to pdst_io_vec_class */
        while ((cnt < psrc_io_vec_class->used_elements) && (R_RESULT_SUCCESS == result))
        {
            result = R_IO_VecAppendElement(pdst_io_vec_class, psrc_io_vec_class->pio_vec[cnt].paddress, (uint16_t)(psrc_io_vec_class->pio_vec[cnt].length));

            cnt++;
        }
    }

    return result;
}
/******************************************************************************
   End of function  R_IO_VecCopy
******************************************************************************/

