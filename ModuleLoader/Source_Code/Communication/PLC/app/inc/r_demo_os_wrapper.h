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
* File Name   : r_demo_os_wrapper.h
*    @version
*        $Rev: 2347 $
*    @last editor
*        $Author: troba $
*    @date
*        $Date:: 2017-04-21 03:43:28 +0900#$
* Description :
******************************************************************************/

#ifndef R_DEMO_OS_WRAPPER_H_
#define R_DEMO_OS_WRAPPER_H_

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Functions prototype
******************************************************************************/

void R_DEMO_ClearEventFlag( volatile void * flags, uint32_t id );
r_result_t R_DEMO_WaitEventFlag( volatile void * flags, uint32_t id );
void R_DEMO_SetEventFlag( volatile void * flags, uint32_t id );
void R_DEMO_BusyWait( uint32_t time );

//--------------
r_result_t CPX3_MonitorEventFlag(volatile void *flags, uint32_t id);
//--------------

#endif /* R_DEMO_OS_WRAPPER_H_ */

