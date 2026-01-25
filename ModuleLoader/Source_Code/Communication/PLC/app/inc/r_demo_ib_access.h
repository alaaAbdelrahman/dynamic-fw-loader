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
* File Name   : r_demo_ib_access.h
*    @version
*        $Rev: 2585 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2017-01-11 15:58:25 +0900#$
* Description :
******************************************************************************/

#ifndef R_DEMO_IB_ACCESS_H
#define R_DEMO_IB_ACCESS_H

/******************************************************************************
Macro definitions
******************************************************************************/
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Functions prototype
******************************************************************************/
/******************************************************************************
* Function Name     : R_DEMO_MenuConfig
* Description       : The sub-menu for manual configuration setting
* Argument          : None
* Return Value      : None
******************************************************************************/
/*!
   \fn void R_DEMO_MenuConfig (void);
   \brief The sub-menu for manual configuration setting
 */
r_result_t R_DEMO_MenuConfig (void);

#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
/******************************************************************************
* Function Name     : R_DEMO_TestMenu
* Description       : The sub-menu for manual configuration setting
* Argument          : None
* Return Value      : None
******************************************************************************/
/*!
   \fn void R_DEMO_TestMenu (void);
   \brief The sub-menu for manual configuration setting
 */
r_result_t R_DEMO_TestMenu(void);
#endif

#endif /* R_DEMO_IB_ACCESS_H */

