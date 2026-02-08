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
* File Name   : r_demo_main.h
*    @version
*        $Rev: 10150 $
*    @last editor
*        $Author: a5089763 $
*    @date
*        $Date:: 2022-05-13 16:16:56 +0900#$
* Description :
******************************************************************************/

#ifndef R_DEMO_APP_THREAD_H
#define R_DEMO_APP_THREAD_H
//#include "iec62056_21_slave.h"
#include "r_typedefs.h"
#include "r_config.h"
#include "r_bsp_api.h"
#include "r_byte_swap.h"
#include "r_memory_api.h"
#include "r_queue_api.h"

#include "r_app_timer.h"

/* g3 part */
#include "r_c3sap_api.h"

/* app part */
#include "r_demo_app.h"
#include "r_demo_app_eap.h"
//#include "r_demo_app_thread.h"
#include "r_demo_api.h"

#include "r_ipv6_headers.h"
#include "r_udp_headers.h"
#include "r_demo_tools.h"
#include "r_demo_api.h"
#include "r_demo_common.h"
#include "r_demo_config.h"
#include "r_demo_statistics.h"
#include "r_demo_ib_access.h"
#include "r_demo_app_ver.h"
#include "r_demo_parameters.h"
#include "r_demo_os_wrapper.h"
/******************************************************************************
Macro definitions
******************************************************************************/
typedef enum
{
    R_DEMO_APP_HANDLE_REBOOT_REQUEST_IND,
    
    R_DEMO_APP_HANDLE_MAC_DATA_IND,
    R_DEMO_APP_HANDLE_MAC_TMR_RCV_IND,
    R_DEMO_APP_HANDLE_MAC_TMR_TRANSMIT_IND,
    
    R_DEMO_APP_HANDLE_DATA_IND,
    R_DEMO_APP_HANDLE_LEAVE_IND,
    R_DEMO_APP_HANDLE_PATH_DIS_IND,
    R_DEMO_APP_HANDLE_LBP_IND,
    R_DEMO_APP_HANDLE_STATUS_IND,
    R_DEMO_APP_HANDLE_BUFFER_IND,
    R_DEMO_APP_HANDLE_KEY_STATE_IND,
    R_DEMO_APP_HANDLE_FRAMECOUNT_IND,
    R_DEMO_APP_HANDLE_ADP_ROUTE_UPDATE_IND,
    R_DEMO_APP_HANDLE_ADP_LOAD_SEQ_NUM_IND,
    R_DEMO_APP_HANDLE_ADP_RREP_IND,
    
    R_DEMO_APP_HANDLE_EAP_NETWORKJOIN_IND,
    R_DEMO_APP_HANDLE_EAP_NETWORKLEAVE_IND,
    R_DEMO_APP_HANDLE_EAP_NEWDEVICE_IND,
    R_DEMO_APP_HANDLE_EAP_EAP_KEY_IND,
    
#if  !defined(R_DEFINE_DISABLE_APP_HYBRID)
    R_DEMO_APP_HANDLE_RFMAC_DATA_IND,
#endif
} r_demo_app_handle_t;


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Functions prototype
******************************************************************************/

/***********************************************************************
* Function Name     : R_DEMO_AppThreadInit
* Description       : Initializes the demo application thread
* Argument          : None
* Return Value      : R_RESULT_SUCCESS or R_RESULT_FAILED
***********************************************************************/
/*!
   \fn r_result_t R_DEMO_AppThreadInit(void)
   \brief Initializes the demo application thread
 */
r_result_t R_DEMO_AppThreadInit (void);

/***********************************************************************
* Function Name     : R_DEMO_AppThreadEnqueInd
* Description       : Enqueues incoming indications into application queue
* Argument          : indPtr : Pointer to indication structure
*                     handle : Handle associated with the indication
*                     size : Size of the structure to be enqueued
* Return Value      : None
***********************************************************************/
/*!
   \fn void R_DEMO_AppThreadEnqueInd(const uint8_t* indPtr,
                                                const uint8_t handle,
                                                const uint16_t size)
   \brief Enqueues incoming indications into application queue
 */
void R_DEMO_AppThreadEnqueInd (const uint8_t * indPtr,
                               const uint8_t   handle,
                               const uint16_t  size);

/***********************************************************************
* Function Name     : R_DEMO_AppThreadEnqueInd
* Description       : Enqueues incoming indications into application queue
* Argument          : indPtr : Pointer to indication structure
*                     handle : Handle associated with the indication
*                     size : Size of the structure to be enqueued
* Return Value      : None
***********************************************************************/
/*!
   \fn void R_DEMO_AppThreadEnqueDataInd(const r_adp_adpd_data_indication_t* ind)
   \brief Enqueues incoming indications into application queue
 */
void R_DEMO_AppThreadEnqueDataInd (const r_adp_adpd_data_ind_t * ind);
void R_DEMO_AppThreadEnqueMacDataInd (const r_g3mac_mcps_data_ind_t * ind);

r_boolean_t R_DEMO_WaitcnfTimeout ( void );
void R_DEMO_WaitcnfTimerOn ( uint32_t timeout );
void R_DEMO_WaitcnfTimerOff ( void );
void R_DEMO_ClearcnfTimeout( void );
void R_DEMO_AppThreadSuspend( void );
void R_DEMO_AppThreadResume( void );


#endif /* R_DEMO_APP_THREAD_H */

