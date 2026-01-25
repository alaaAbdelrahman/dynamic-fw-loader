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
* File Name   : r_timer_api.h
*    @version
*        $Rev: 911 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2016-02-05 16:39:17 +0900#$
* Description : 
******************************************************************************/

/*
 * Prevent nested inclusions
 */
#ifndef R_APP_TIMER_H
#define R_APP_TIMER_H

/******************************************************************************
Macro definitions
******************************************************************************/
#define R_TIMER_HALF_WRAP_INTERVAL_LENGTH (0x7FFFFFFFul) /* Half the number of ticks until the tick count variable (uint32_t) will wrap around */
#define R_SEC_TO_MILLISEC (1000u)                        /* Conversion factor from seconds to milliseconds */
#define R_MIN_TO_MILLISEC (60000u)                       /* Conversion factor from minutes to milliseconds */

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef void (*r_timer_one_shot_callback_t)(uint8_t handle); // The one shot callback function type

/*!
   \struct r_timer_oneshot_table_t
   \brief One shot timer table structure
 */
typedef struct
{
    r_boolean_t active; // Oneshot timer status, R_TRUE if running
    uint32_t    top;    // Oneshot timer expiry tick count

} r_timer_oneshot_table_t;

/******************************************************************************
Exported global variables
******************************************************************************/

/******************************************************************************
Exported global functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
* Function Name     : R_TIMER_Init
* Description       : The function calls the init function for the module and
*                     stores the tick period locally
* Argument          : timer_tick_period : Period in ms of the hardware timer
* Return Value      : R_RESULT_SUCCESS in case of proper initialization,
*                     R_RESULT_FAILED otherwise
******************************************************************************/
/*!
   \fn       r_result_t R_TIMER_Init(uint32_t timer_tick_period);
   \brief    The function calls the init function for the module and stores
             the tick period locally
   \param[in] timer_tick_period Period in ms of the hardware timer
   \return   R_RESULT_SUCCESS in case of proper initialization,
             R_RESULT_FAILED otherwise
 */
r_result_t R_TIMER_Init(uint32_t timer_tick_period);

/******************************************************************************
* Function Name     : R_TIMER_Reset
* Description       : The function sets the tick count to 0, all table pointers
*                     and callback function pointers to NULL and all table lengths to 0.
* Argument          : None
* Return Value      : R_RESULT_SUCCESS in case of proper resetting,
*                     R_RESULT_FAILED otherwise
******************************************************************************/
/*!
   \fn       r_result_t R_TIMER_Reset(void);
   \brief    The function sets the tick count to 0, all table pointers
             and callback function pointers to NULL and all table lengths to 0.
   \return   R_RESULT_SUCCESS in case of proper resetting,
             R_RESULT_FAILED otherwise
 */
r_result_t R_TIMER_Reset(void);

/******************************************************************************
* Function Name     : R_TIMER_AssignOneShotTimer
* Description       : Configures a one shot timer instance for a speciffic timer Id.
* Argument          : timer_id : Id of the timer to be configured. Takes values from
*                     0 to (R_MAX_TIMER_ONE_SHOT_INSTANCES - 1)
*                     table : Pointer to an external one shot timer table
*                     table_length : Max length of the one shot timer table
*                     pone_shot_cb : Pointer to a callback function for this timer instantiation
* Return Value      : R_RESULT_ILLEGAL_NULL_POINTER if any of the pointers passed to the function
*                     are not set, R_RESULT_BAD_INPUT_ARGUMENTS if any of the other arguments
*                     have values outside of their ranges,
*                     R_RESULT_SUCCESS if the timer is successfully configured
******************************************************************************/
/*!
   \fn    r_result_t R_TIMER_AssignOneShotTimer(const uint8_t timer_id,
                                                r_timer_oneshot_table_t table[],
                                                const uint16_t table_length,
                                                uint8_t active_check[],
                                                const uint16_t active_check_length,
                                                const r_timer_one_shot_callback_t one_shot_cb)
   \brief    Configures a one shot timer instance for a speciffic timer Id.
   \param[in] timer_id Id of the timer to be configured. Takes values from
              0 to (R_MAX_TIMER_ONE_SHOT_INSTANCES - 1)
   \param[in] table Pointer to an external one shot timer table
   \param[in] table_length Max length of the one shot timer table
   \param[in] one_shot_cb pointer to a callback function for this timer instantiation
   \return R_RESULT_ILLEGAL_NULL_POINTER if any of the pointers passed to the function
           are not set, R_RESULT_BAD_INPUT_ARGUMENTS if any of the other arguments
           have values outside of their ranges,
           R_RESULT_SUCCESS if the timer is successfully configured
 */
r_result_t R_TIMER_AssignOneShotTimer(const uint8_t timer_id,
                                      r_timer_oneshot_table_t table[],
                                      const uint16_t table_length,
                                      uint8_t active_check[],
                                      const uint16_t active_check_length,
                                      const r_timer_one_shot_callback_t one_shot_cb);

/******************************************************************************
* Function Name     : R_TIMER_ReleaseOneShotTimer
* Description       : Releases a one shot timer instance for a speciffic timer Id. This is the
*                     inverse function to R_TIMER_AssignOneShotTimer
* Argument          : timer_id : Id of the timer to be unconfigured. Takes values from
*                     0 to (R_MAX_TIMER_ONE_SHOT_INSTANCES - 1)
* Return Value      : R_RESULT_BAD_INPUT_ARGUMENTS if the timer id is out of range or the
*                     timer for this id is not configured, else R_RESULT_SUCCESS
******************************************************************************/
/*!
   \fn    r_result_t R_TIMER_ReleaseOneShotTimer(const uint8_t timer_id);
   \brief Releases a one shot timer instance for a speciffic timer Id. This is the
          inverse function to R_TIMER_AssignOneShotTimer
   \param[in] timer_id Id of the timer to be unconfigured. Takes values from
              0 to (R_MAX_TIMER_ONE_SHOT_INSTANCES - 1)
   \return R_RESULT_BAD_INPUT_ARGUMENTS if the timer id is out of range or the
           timer for this id is not configured, else R_RESULT_SUCCESS
 */
r_result_t R_TIMER_ReleaseOneShotTimer(const uint8_t timer_id);

/******************************************************************************
* Function Name     : R_TIMER_TimerOneShotOn
* Description       : Starts a one shot timer for a timer id and timeout value speciffied in ms.
*                     The function computes the tick count for this timeout event and sets it in
*                     the timer table for the index indicated by handle.
* Argument          : timer_id : Id of the timer instance for which this one shot timer is set
*                     timeout : Timeout value in ms for this timer
*                     handle Handle used for this timer event, will be indicated in
*                     the callback function
* Return Value      : R_RESULT_BAD_INPUT_ARGUMENTS if the timer id is out of range or the
*                     timer for this id is not configured, else R_RESULT_SUCCESS
******************************************************************************/
/*!
   \fn r_result_t R_TIMER_TimerOneShotOn(const uint8_t timer_id,
                                         const uint32_t timeout,
                                         const uint8_t handle);
   \brief Starts a one shot timer for a timer id and timeout value speciffied in ms.
          The function computes the tick count for this timeout event and sets it in
          the timer table for the index indicated by handle.
   \param[in] timer_id Id of the timer instance for which this one shot timer is set
   \param[in] timeout Timeout value in ms for this timer
   \param[in] handle Handle used for this timer event, will be indicated in
              the callback function
   \return R_RESULT_BAD_INPUT_ARGUMENTS if the timer_id or the handle are out of range
           or if the timer is not configured, R_RESULT_FAILED if the timer is already
           running for this handle, else R_RESULT_SUCCESS
 */
r_result_t R_TIMER_TimerOneShotOn(const uint8_t timer_id,
                                  const uint32_t timeout,
                                  const uint8_t handle);

/******************************************************************************
* Function Name     : R_TIMER_TimerOneShotOff
* Description       : Stops a one shot timer started using R_TIMER_TimerOneShotOn
* Argument          : timer_id : Id of the timer instance used when starting the one shot timer
*                     handle : Handle used for the one shot timer
* Return Value      : R_RESULT_BAD_INPUT_ARGUMENTS if the timer id or the handle are out of
*                     range or if the timer instance is not configured, else R_RESULT_SUCCESS
******************************************************************************/
/*!
   \fn r_result_t R_TIMER_TimerOneShotOff(const uint8_t timer_id,
                                          const uint8_t handle);
   \brief Stops a one shot timer started using R_TIMER_TimerOneShotOn
   \param[in] timer_id Id of the timer instance used when starting the one shot timer
   \param[in] handle Handle used for the one shot timer
   \return R_RESULT_BAD_INPUT_ARGUMENTS if the timer id or the handle are out of
           range or if the timer instance is not configured, else R_RESULT_SUCCESS
 */
r_result_t R_TIMER_TimerOneShotOff(const uint8_t timer_id,
                                   const uint8_t handle);

/******************************************************************************
* Function Name     : R_TIMER_TimerOneShotOffAll
* Description       : Stops all one shot timers for a timer id
* Argument          : timer_id : Id of the timer instance
* Return Value      : R_RESULT_BAD_INPUT_ARGUMENTS if the timer id is out of range or if
*                     the timer instance is not configured, else R_RESULT_SUCCESS
******************************************************************************/
/*!
   \fn r_result_t R_TIMER_TimerOneShotOffAll(const uint8_t timer_id);
   \brief Stops all one shot timers for a timer id
   \param[in] timer_id Id of the timer instance
   \return R_RESULT_BAD_INPUT_ARGUMENTS if the timer id is out of range or if
           the timer instance is not configured, else R_RESULT_SUCCESS
 */
r_result_t R_TIMER_TimerOneShotOffAll(const uint8_t timer_id);

/******************************************************************************
* Function Name     : R_TIMER_TimerOneShotOffRange
* Description       : Stops a range of one shot timers for a timer_id
* Argument          : timer_id : Id of the timer insntace
*                     lower_idx : Start index in the one shot table
*                     upper_idx : End index in the one shot table
* Return Value      : R_RESULT_BAD_INPUT_ARGUMENTS for a timer id which is out of range or
*                     is not configured or for lower_idx and upper_idx values which are out of range,
*                     else R_RESULT_SUCCESS
******************************************************************************/
/*!
   \fn r_result_t R_TIMER_TimerOneShotOffRange(const uint8_t timer_id,
                                               const uint8_t lower_idx,
                                               const uint8_t upper_idx);
   \brief Stops a range of one shot timers for a timer_id
   \param[in] timer_id Id of the timer insntace
   \param[in] lower_idx Start index in the one shot table
   \param[in] upper_idx End index in the one shot table
   \return R_RESULT_BAD_INPUT_ARGUMENTS for a timer id which is out of range or
           is not configured or for lower_idx and upper_idx values which are out of range,
           else R_RESULT_SUCCESS
 */
r_result_t R_TIMER_TimerOneShotOffRange(const uint8_t timer_id,
                                        const uint8_t lower_idx,
                                        const uint8_t upper_idx);

/******************************************************************************
* Function Name     : R_TIMER_TimerOneShotIsRunning
* Description       : Stops a range of one shot timers for a timer_id
* Argument          : timer_id : Id of the timer instance
*                     handle : Handle used for this timer event
*                     active : Status for the current tiemrId and handle
* Return Value      : R_RESULT_BAD_INPUT_ARGUMENTS if the timer id is out of range or if
*                     the timer instance is not configured, else R_RESULT_SUCCESS
******************************************************************************/
/*!
   \fn r_result_t R_TIMER_TimerOneShotIsRunning(const uint8_t timer_id,
                                                const uint8_t handle,
                                                r_boolean_t* pactive);
   \brief Read the status of a specific timer started using R_TIMER_TimerOneShotOn
   \param[in] timer_id Id of the timer instance
   \param[in] handle Handle used for this timer event
   \param[out] pactive Status for the current tiemrId and handle
   \return R_RESULT_BAD_INPUT_ARGUMENTS if the timer id is out of range or if
           the timer instance is not configured, else R_RESULT_SUCCESS
 */
r_result_t R_TIMER_TimerOneShotIsRunning(const uint8_t timer_id,
                                         const uint8_t handle,
                                         r_boolean_t* pactive);

/******************************************************************************
* Function Name     : R_TIMER_GetExpireTickCount
* Description       : Returns the expire tick count for a given delta in the future expressed in ms
* Argument          : expire_tick_count : The tick count measured in ticks when this time stamp will expire
*                     delta : Time in ms. If 0, the function returns the current tick count.
* Return Value      : R_RESULT_ILLEGAL_NULL_POINTER if any of the pointers passed are NULL,
*                     R_RESULT_BAD_INPUT_ARGUMENTS if the computed ticks for the given delta are greater than
*                     half of the maximum number of ticks that can be counted by the timer module,
*                     else R_RESULT_SUCCESS
******************************************************************************/
/*!
   \fn r_result_t R_TIMER_GetExpireTickCount(uint32_t* pexpire_tick_count,
                                             const uint32_t delta);
   \brief Returns the expire tick count for a given delta in the future expressed in ms
   \param[out] pexpire_tick_count The tick count measured in ticks when this time stamp will expire
   \param[in] delta Time in ms. If 0, the function returns the current tick count.
   \return R_RESULT_ILLEGAL_NULL_POINTER if any of the pointers passed are NULL,
           R_RESULT_BAD_INPUT_ARGUMENTS if the computed ticks for the given delta are greater than
           half of the maximum number of ticks that can be counted by the timer module,
           else R_RESULT_SUCCESS
 */
r_result_t R_TIMER_GetExpireTickCount(uint32_t* pexpire_tick_count,
                                      const uint32_t delta);

/******************************************************************************
* Function Name     : R_TIMER_GetRemainingTime
* Description       : Returns the number of ms till expire_tick_count. The function takes into
*                     account the wrap around of the tick counter.
* Argument          : expire_tick_count : Tick count value read using R_TIMER_GetTimeStamp
* Return Value      : Number of ms till expire_tick_count, 0 if already expired
******************************************************************************/
/*!
   \fn uint32_t R_TIMER_GetRemainingTime(const uint32_t expire_tick_count);
   \brief Returns the number of ms till expire_tick_count. The function takes into
          account the wrap around of the tick counter.
   \param[in] expire_tick_count Tick count value read using R_TIMER_GetTimeStamp
   \return Number of ms till expire_tick_count, 0 if already expired
 */
uint32_t R_TIMER_GetRemainingTime(const uint32_t expire_tick_count);

/******************************************************************************
* Function Name     : R_TIMER_TickCountExpired
* Description       : Check the expiry status of a time stamp read using R_TIMER_GetTickCount
* Argument          : expire_tick_count : Tick count value read using R_TIMER_GetTimeStamp
* Return Value      : R_TRUE if the time has expired, else R_FALSE
******************************************************************************/
/*!
   \fn r_boolean_t R_TIMER_TickCountExpired(const uint32_t expire_tick_count);
   \brief Check the expiry status of a time stamp read using R_TIMER_GetTickCount
   \details The function takes into account the wrap around of the tick counter.
            Due to this a time only works for half the maximum number of ticks that can be counted.
            For a hardware tick period of 1 ms and for a size of the uin32_t data type of 4 bytes,
            the half tick count is 0x7FFFFFFF which corresponds to 24 days.
   \param[in] expire_tick_count Tick count value read using R_TIMER_GetTimeStamp
   \return R_TRUE if the time has expired, else R_FALSE
 */
r_boolean_t R_TIMER_TickCountExpired(const uint32_t expire_tick_count);

/******************************************************************************
* Function Name     : R_TIMER_BusyWait
* Description       : Busy wait for timeSpan ms
* Argument          : timeSpan : Number of ms to busy wait
* Return Value      : None
******************************************************************************/
/*!
   \fn void R_TIMER_BusyWait(uint32_t timeSpan);
   \brief Busy wait for timeSpan ms
   \param[in] timeSpan Number of ms to busy wait
 */
void R_TIMER_BusyWait(uint32_t time_span);

/******************************************************************************
* Function Name     : R_TIMER_GetFreerunTime
* Description       : Get free run timer counter
* Argument          : None
* Return Value      : Free run timer counter
******************************************************************************/
/*!
   \fn void R_TIMER_GetFreerunTime(void);
   \brief Get free run timer counter
   \return Free run timer counter
 */
uint32_t R_TIMER_GetFreerunTime(void);

/******************************************************************************
* Function Name     : R_TIMER_Handle
* Description       : Function periodically called by a HW timer every R_TIMER_TICK_PERIOD ms
* Argument          : None
* Return Value      : None
******************************************************************************/
/*!
   \fn void R_TIMER_Handle(void);
   \brief Function periodically called by a HW timer every R_TIMER_TICK_PERIOD ms
   \details The function should be called from an external hardware timer every R_TIMER_TICK_PERIOD ms.
            The function increments the tick count and checks all active timer instances for expiry of
            their one shot timers and calls the callback function for those timers. If the callback
            is called the timer is also stopped.
 */
void R_TIMER_Handle(void);

/******************************************************************************
* Function Name     : R_TIMER_EventProcess
* Description       : Function periodically called by a Processing Thread
* Argument          : None
* Return Value      : None
******************************************************************************/
/*!
   \fn void R_TIMER_EventProcess(void);
   \brief Function periodically called by a Processing Thread
   \details The function increments the tick count and checks all active timer instances for expiry of
            their one shot timers and calls the callback function for those timers. If the callback
            is called the timer is also stopped.
 */
void R_TIMER_EventProcess(void);

#endif /* R_APP_TIMER_H */
