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
* File Name   : r_checksum.h
*    @version
*        $Rev: 1918 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2016-08-02 10:24:25 +0900#$
* Description : 
******************************************************************************/



#ifndef R_CHECKSUM_H
#define R_CHECKSUM_H

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Functions prottype
******************************************************************************/


/*!
    \fn r_result_t R_CS_ComputeCheckSum(const r_iovec_class_t* const ioVecClass,
                                uint16_t* const checkSum);
    \brief Checksum computation function
    \param[in] ioVecClass Pointer to a iovec class containing the data input
    \param[in] checkSum  Pointer to a memory location where the output will be written to
    \return R_RESULT_ILLEGAL_NULL_POINTER or R_RESULT_SUCCESS
 */
r_result_t R_CS_ComputeCheckSum(const r_iovec_class_t* const ioVecClass,
                                uint16_t* const checkSum);

#endif /* R_CHECKSUM_H */
