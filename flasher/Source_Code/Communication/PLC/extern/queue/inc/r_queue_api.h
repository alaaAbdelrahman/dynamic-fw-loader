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
* File Name   : r_pr_queue.h
*    @version
*        $Rev: 1997 $
*    @last editor
*        $Author: a0202438 $
*    @date  
*        $Date:: 2016-08-30 19:05:39 +0900#$
* Description : 
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_QUEUE_API_H
#define R_QUEUE_API_H

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
/*!
    \enum r_queue_result_t
    \brief Enumeration of queue return types
 */
typedef enum
{
    R_QUEUE_RESULT_SUCCESS      = 0, // SUCCESS
    R_QUEUE_RESULT_FULL         = 1, // FULL
    R_QUEUE_RESULT_EMPTY        = 2, // EMPTY
    R_QUEUE_RESULT_FAILED       = 3, // FAILED
    R_QUEUE_RESULT_UNKNOWN      = 4  // UNKNOWN

} r_queue_result_t;

/*!
    \struct r_queue_element_t
    \brief Structure for the queue array elements
 */
typedef struct
{
    const uint8_t* pdata;  // The address of the element in the queue
    uint16_t size;         // Size of element
    uint8_t  handle;       // The handle of the element

} r_queue_element_t;

/*!
    \struct r_queue_t
    \brief Structure for the general purpose queue
 */
typedef struct
{
    r_queue_element_t*  parray;              // Pointer to the queue array
    uint16_t            length;              // The number of enqueued elements
    uint16_t            wi;                  // Index of the next free queue element
    uint16_t            ri;                  // Index of the next queue element to be read
    uint16_t            max_length;          // Maximum length of the queue array

} r_queue_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
* Function Name     : R_QUEUE_Create
* Description       : This function creates a queue
* Argument          : pqueue : Pointer to the queue to be created
*                     array : Array used for enqueuing elements
*                     max_length : Maximum length of the queue array
* Return Value      : R_QUEUE_RESULT_SUCCESS on successful queue creation,
*                     else R_QUEUE_RESULT_FAILED
******************************************************************************/
/*!
    \fn r_queue_result_t R_QUEUE_Create(r_queue_t* pqueue,
                                        r_queue_element_t array[],
                                        uint16_t max_length);
    \brief This function creates a queue
    \param[in] pqueue Pointer to the queue to be created
    \param[in] array Array used for enqueuing elements
    \param[in] max_length Maximum length of the queue array
    \return R_QUEUE_RESULT_SUCCESS on successful queue creation, else R_QUEUE_RESULT_FAILED
 */
r_queue_result_t R_QUEUE_Create(r_queue_t* pqueue,
                                r_queue_element_t array[],
                                uint16_t max_length);

/******************************************************************************
* Function Name     : R_QUEUE_IsFull
* Description       : This function returns R_TRUE if the queue is full, else R_FALSE
* Argument          : pqueue : Pointer to the queue 
* Return Value      : R_TRUE if the queue is full, else R_FALSE
******************************************************************************/
/*!
    \fn r_boolean_t R_QUEUE_IsFull(r_queue_t* pqueue);
    \brief This function returns R_TRUE if the queue is full, else R_FALSE
    \param[in] pqueue Pointer to the queue 
    \return R_TRUE if the queue is full, else R_FALSE
 */
r_boolean_t R_QUEUE_IsFull(r_queue_t* pqueue);

/******************************************************************************
* Function Name     : R_QUEUE_Enqueue
* Description       : This function enqueues an element to the queue
* Argument          : pqueue : Pointer to the queue where the element should be enqueued to
*                     pelement : const pointer to an element to be enqueued
*                     size : Size of the element at the address which is enqueued
*                     handle : Type of the element at the address which is enqueued
* Return Value      : R_QUEUE_RESULT_SUCCESS for successfully enqueuing an element,
                      R_QUEUE_RESULT_FULL if the queue is full else R_QUEUE_RESULT_FAILED
******************************************************************************/
/*!
    \fn r_queue_result_t R_QUEUE_Enqueue(r_queue_t* pqueue, const uint8_t* pelement, uint16_t size, uint8_t handle)
    \brief This function enqueues an element to the queue
    \param[in] pqueue Pointer to the queue where the element should be enqueued to
    \param[in] pelement const pointer to an element to be enqueued
    \param[in] size Size of the element at the address which is enqueued
    \param[in] handle Type of the element at the address which is enqueued
    \return R_QUEUE_RESULT_SUCCESS for successfully enqueuing an element,
            R_QUEUE_RESULT_FULL if the queue is full else R_QUEUE_RESULT_FAILED
 */
r_queue_result_t R_QUEUE_Enqueue(r_queue_t* pqueue,
                                 const uint8_t* pelement,
                                 uint16_t size,
                                 uint8_t handle);

/******************************************************************************
* Function Name     : R_QUEUE_Dequeue
* Description       : This function dequeues an element from the queue
* Argument          : pqueue : Pointer to the queue where the element should be dequeued from
*                     pdequeue : Pointer to a r_queue_element_t structure where the dequeued
*                     element will be written to
* Return Value      : R_QUEUE_RESULT_SUCCESS for successfully dequeuing an element,
                      R_QUEUE_RESULT_EMPTY if the queue is empty else R_QUEUE_RESULT_FAILED
******************************************************************************/
/*!
    \fn r_queue_result_t R_QUEUE_Dequeue(r_queue_t* pqueue,
                                         r_queue_element_t* pdequeue)
    \brief This function dequeues an element from the queue
    \param[in] pqueue Pointer to the queue where the element should be dequeued from
    \param[out] pdequeue Pointer to a r_queue_element_t structure where the dequeued
                element will be written to
    \return R_QUEUE_RESULT_SUCCESS for successfully dequeuing an element,
            R_QUEUE_RESULT_EMPTY if the queue is empty else R_QUEUE_RESULT_FAILED
 */
r_queue_result_t R_QUEUE_Dequeue(r_queue_t* pqueue,
                                 r_queue_element_t* pdequeue);

/******************************************************************************
* Function Name     : R_QUEUE_Flush
* Description       : This function flushes a queue
* Argument          : pqueue : Pointer to the queue to be flushed
* Return Value      : R_QUEUE_RESULT_SUCCESS or R_QUEUE_RESULT_FAILED
******************************************************************************/
/*!
    \fn r_queue_result_t R_QUEUE_Flush(r_queue_t* pqueue)
    \brief This function flushes a queue
    \param[in] pqueue Pointer to the queue to be flushed
    \return R_QUEUE_RESULT_SUCCESS or R_QUEUE_RESULT_FAILED
 */
r_queue_result_t R_QUEUE_Flush(r_queue_t* pqueue);


#endif
