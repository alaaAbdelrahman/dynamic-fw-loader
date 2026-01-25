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

/******************************************************************************
* File Name     : r_c3sap_sys_thread.c
* Device(s)     : RX631
* Tool-chain    : KPIT GNURX-ELF 15.01
* H/W platform  : G-CPX / EU-CPX2 / G-CPX3
* Description   : System layer API
******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
#include "r_typedefs.h"
#include "r_config.h"
#include "r_bsp_api.h"
#include "r_queue_api.h"
#include "r_constants.h"
#include "r_sys_sap.h"
#include "r_uif_sap_if.h"
#include "r_c3sap_sys_depend.h"
#include "r_c3sap_g3_depend.h"
#include "r_c3sap_sys_thread.h"
#include "r_app_timer.h"
/******************************************************************************
* Local variables
******************************************************************************/
static r_queue_t                timer_queue = {NULL, 0u, 0u, 0u, 0u}; //!< Queue used for timeout events
static r_queue_element_t        timer_queue_array[QUEUE_SIZE_TIMER]; //!< Queue array for timer queue

/* Timer related static variables */
static r_timer_oneshot_table_t  timer_table[R_MAX_USED_TIMER_HANDLES];
static uint8_t                  active_check[(R_MAX_USED_TIMER_HANDLES / 8u) + 1u];

/******************************************************************************
* External variables
******************************************************************************/

/******************************************************************************
* Local function headers
******************************************************************************/

/*!
   \fn void thread_processing(void)
   \brief The processing thread
 */
static void thread_processing(void);


/*!
   \fn void thread_enqueue_timeout(uint8_t handle)
   \brief  Stores timeout events in timer queue.
 */
static void thread_enqueue_timeout(uint8_t handle);

/******************************************************************************
* Functions
******************************************************************************/

/******************************************************************************
* Function Name:R_THREAD_Init
* Description :Initialization for the processing thread
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_THREAD_Init(void)
{
    /* Clear timer table */
    R_TIMER_TimerOneShotOffRange(R_TIMER_ID_CPX, R_HANDLE_CPX_FIRST, R_HANDLE_CPX_LAST);
    

    
    if (R_TIMER_AssignOneShotTimer(R_TIMER_ID_CPX, timer_table, R_MAX_USED_TIMER_HANDLES, active_check, sizeof(active_check), thread_enqueue_timeout) != R_RESULT_SUCCESS)
    {
       return R_RESULT_FAILED;
    }
    
    /* Configure thread timer */
    if( R_BSP_ConfigureTimer(R_BSP_TIMER_ID_2,R_CPX_THREAD_PERIOD,thread_processing) != R_RESULT_SUCCESS)
    {
       return R_RESULT_FAILED;
    }
    R_BSP_TimerOn(R_BSP_TIMER_ID_2);
    

    
    /* Initialize the timer queue */
    if (R_QUEUE_Create(&timer_queue, timer_queue_array, QUEUE_SIZE_TIMER) != R_QUEUE_RESULT_SUCCESS)
    {
        return R_RESULT_FAILED;
    }
	
    return R_RESULT_SUCCESS;
}

/******************************************************************************
   End of function  R_THREAD_Init
******************************************************************************/

/******************************************************************************
* Function Name:R_THREAD_Deinit
* Description :Deinitialization for the processing thread
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_THREAD_Deinit(void)
{
    R_TIMER_ReleaseOneShotTimer(R_TIMER_ID_CPX);
    R_BSP_TimerOff(R_BSP_TIMER_ID_2);
    
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_THREAD_Deinit
******************************************************************************/

/******************************************************************************
* Function Name:thread_processing
* Description :The processing thread
* Arguments : 
* Return Value : 
******************************************************************************/
static void thread_processing(void)
{
	r_queue_element_t deque;
    r_queue_result_t  queue_return;
    /* Re-enable interrupts to allow processing of UART communication */
    R_BSP_EI();
    
    R_TIMER_EventProcess(); // %%TIMER_INT : Time out check for each event (Move from Timer interruption)
    
    /* Try to dequeue a message from timer queue (highest priority)*/
    queue_return = R_QUEUE_Dequeue(&timer_queue, &deque);
    
    /* Check if something has been dequeued */
    while ( R_QUEUE_RESULT_SUCCESS == queue_return )
    {
        if ( deque.handle < R_HANDLE_G3_FIRST )
        {
              /* Call process function */
              R_SYS_TimeOutEvent( deque.handle );
        }
        else
        {
            /* Call process function */
            R_G3CTRL_TimeOutEvent( deque.handle );
        }
        
        /* Check if another message is in queue */
        queue_return = R_QUEUE_Dequeue(&timer_queue, &deque);
    }

    /* Always call process function */
    R_UIF_EventProcess();

}
/******************************************************************************
   End of function  thread_processing
******************************************************************************/

/******************************************************************************
* Function Name:thread_enqueue_timeout
* Description :Function for enqueuing a timeout event
* Arguments : 
* Return Value : 
******************************************************************************/
static void thread_enqueue_timeout(uint8_t handle)
{
    /* Enqueue timer event if not R_HANDLE_DONT_CARE */
    if (R_HANDLE_DONT_CARE != handle)
    {
        if (R_QUEUE_Enqueue(&timer_queue, NULL, 0u, handle) != R_QUEUE_RESULT_SUCCESS)
        {
            /* if enqueuing did not work perform a MCU reset to avoid hang ups */
            R_BSP_SoftReset();
        }
    }
}
/******************************************************************************
   End of function  thread_enqueue_timeout
******************************************************************************/
