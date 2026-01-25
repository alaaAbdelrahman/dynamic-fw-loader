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
* File Name   : r_app_timer.c
*    @version
*        $Rev: 4180 $
*    @last editor
*        $Author: a5089763 $
*    @date  
*        $Date:: 2018-06-06 13:13:00 +0900#$
* Description : 
******************************************************************************/
/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <stdlib.h>
#include "r_typedefs.h"
#include "r_config.h"
#include "r_app_timer.h"
#include "r_bsp_api.h"
/******************************************************************************
   Defines
******************************************************************************/

/* R_MAX_TIMER_ONE_SHOT_INSTANCES must be defined */
#if !defined (R_MAX_TIMER_ONE_SHOT_INSTANCES)
    #error "ERROR !!! You must define R_MAX_TIMER_ONE_SHOT_INSTANCES in the configuration file of your project !";
#endif

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/
/*!
   \struct r_timer_oneshot_class_t
   \brief One shot timer class
 */
typedef struct
{
    r_timer_oneshot_table_t*    ptable;
    r_timer_one_shot_callback_t one_shot_cb;
    uint16_t                    table_length;
    uint8_t*                    pactive_check;
    uint16_t                    active_check_length;

} r_timer_oneshot_class_t;

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
/******************************************************************************
* Static variables
******************************************************************************/
uint32_t       tick_count          = 0u;
static r_timer_oneshot_class_t shot_timer[R_MAX_TIMER_ONE_SHOT_INSTANCES]; // Table containing information for each timer instance, no initialization required
static uint32_t                hwtimer_tick_period = 1u;
static volatile uint8_t        timer_int           = R_FALSE;

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
* Function Name:R_TIMER_Init
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_TIMER_Init(uint32_t timer_tick_period)
{
    if (0UL == timer_tick_period)
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    else
    {
        /* do nothing */
    }
    
    /* Configure oneshot timer */
    if (R_BSP_ConfigureTimer(R_BSP_TIMER_ID_0, R_HW_TIMER_TICK_PERIOD,&R_TIMER_Handle) != R_RESULT_SUCCESS)
    {
        return R_RESULT_FAILED; /* if already in use */
    }
    
    R_BSP_TimerOn(R_BSP_TIMER_ID_0);
    
    hwtimer_tick_period = timer_tick_period;
    
    R_TIMER_Reset();
    
    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_TIMER_Init
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_Reset
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_TIMER_Reset(void)
{
    uint16_t i;

    tick_count = 0u;

    for (i = 0u ; i < R_MAX_TIMER_ONE_SHOT_INSTANCES ; i++)
    {
        shot_timer[i].ptable              = NULL;
        shot_timer[i].one_shot_cb         = NULL;
        shot_timer[i].table_length        = 0u;
        shot_timer[i].pactive_check       = NULL;
        shot_timer[i].active_check_length = 0u;
    }

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_TIMER_Reset
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_AssignOneShotTimer
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_TIMER_AssignOneShotTimer(const uint8_t timer_id,
                                      r_timer_oneshot_table_t table[],
                                      const uint16_t table_length,
                                      uint8_t active_check[],
                                      const uint16_t active_check_length,
                                      const r_timer_one_shot_callback_t one_shot_cb)
{
    uint16_t i;

    /* parameter checks */
    if ((NULL == table) || (NULL == one_shot_cb))
    {
        return R_RESULT_ILLEGAL_NULL_POINTER;
    }
    else if ((0u == table_length) || (timer_id >= R_MAX_TIMER_ONE_SHOT_INSTANCES))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    else if (NULL != shot_timer[timer_id].ptable)
    {
        return R_RESULT_FAILED; /* if already in use */
    }
    else
    {
        shot_timer[timer_id].ptable              = table;
        shot_timer[timer_id].table_length        = table_length;
        shot_timer[timer_id].pactive_check       = active_check;
        shot_timer[timer_id].active_check_length = active_check_length;
        shot_timer[timer_id].one_shot_cb         = one_shot_cb;

        /* reset all values */
        for (i = 0 ; i < table_length ; i++)
        {
            table[i].active = R_FALSE;
            table[i].top    = 0u;
        }
        for (i = 0u ; i < active_check_length ; i++)
        {
            active_check[i] = R_FALSE;
        }

        return R_RESULT_SUCCESS;
    }
}
/******************************************************************************
   End of function  R_TIMER_AssignOneShotTimer
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_ReleaseOneShotTimer
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_TIMER_ReleaseOneShotTimer(const uint8_t timer_id)
{
    if ((timer_id >= R_MAX_TIMER_ONE_SHOT_INSTANCES) ||
        (NULL == shot_timer[timer_id].ptable))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    else
    {
        shot_timer[timer_id].ptable              = NULL;
        shot_timer[timer_id].table_length        = 0u;
        shot_timer[timer_id].one_shot_cb         = NULL;
        shot_timer[timer_id].pactive_check       = NULL;
        shot_timer[timer_id].active_check_length = 0u;
        return R_RESULT_SUCCESS;
    }
}
/******************************************************************************
   End of function  R_TIMER_ReleaseOneShotTimer
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_TimerOneShotOn
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_TIMER_TimerOneShotOn(const uint8_t timer_id,
                                  const uint32_t timeout,
                                  const uint8_t handle)
{
    r_result_t result;
    uint32_t   count = 0u;

    /* Parameter check */
    if (((timer_id >= R_MAX_TIMER_ONE_SHOT_INSTANCES)   ||
         (handle >= shot_timer[timer_id].table_length)) ||
         (NULL   == shot_timer[timer_id].ptable      ))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    else
    {
        /* do nothing */
    }

    if (0UL == hwtimer_tick_period)
    {
        return R_RESULT_FAILED;
    }

    /* round up to the next possible interval */
    else
    {
        /* Calculate oneshot timeout in tick units */
        count = timeout / hwtimer_tick_period;

        /* If not multiple of hwtimer_tick_period, round up. */
        if ((timeout % hwtimer_tick_period) != 0uL)
        {
            count++;
        }
        else
        {
            /* do nothing */
        }
    }

    if (0uL == count)
    {
        /* Call user callback */
        shot_timer[timer_id].one_shot_cb(handle);

        result = R_RESULT_SUCCESS;
    }
    else
    {
        if (R_FALSE == shot_timer[timer_id].ptable[handle].active)
        {
            /* Set up counters and flag */
            shot_timer[timer_id].ptable[handle].active = R_TRUE;
            shot_timer[timer_id].ptable[handle].top    = tick_count + count; /* depends upon the hwtimer_tick_period, will wrap arround  */
            {
                uint8_t cnt    = (uint8_t)(handle / 8u);
                uint8_t offset = (uint8_t)(handle % 8u);

                shot_timer[timer_id].pactive_check[cnt] |= (uint8_t)(1u << offset);
            }
            result = R_RESULT_SUCCESS;
        }
        else
        {
            /* Oneshot was already running or tick count is zero, do nothing */
            result = R_RESULT_FAILED;
        }
    }

    return result;
}
/******************************************************************************
   End of function  R_TIMER_TimerOneShotOn
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_TimerOneShotOff
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_TIMER_TimerOneShotOff(const uint8_t timer_id,
                                   const uint8_t handle)
{
    if (((timer_id >= R_MAX_TIMER_ONE_SHOT_INSTANCES)   ||
         (handle >= shot_timer[timer_id].table_length)) ||
         (NULL   == shot_timer[timer_id].ptable      ))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    else
    {
        /* do nothing */
    }

    /* Stop one shot (via flag), reset counters */
    shot_timer[timer_id].ptable[handle].active = R_FALSE;
    shot_timer[timer_id].ptable[handle].top    = 0u;
    {
        uint8_t cnt    = (uint8_t)(handle / 8u);
        uint8_t offset = (uint8_t)(handle % 8u);

        shot_timer[timer_id].pactive_check[cnt] &= (uint8_t)(~(1u << offset));
    }

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_TIMER_TimerOneShotOff
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_TimerOneShotOffAll
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_TIMER_TimerOneShotOffAll(const uint8_t timer_id)
{
    uint16_t i;

    if ((timer_id >= R_MAX_TIMER_ONE_SHOT_INSTANCES) ||
        (NULL == shot_timer[timer_id].ptable))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    else
    {
        /* do nothing */
    }

    for (i = 0u ; i < shot_timer[timer_id].table_length ; i++)
    {
        shot_timer[timer_id].ptable[i].active = R_FALSE;
        shot_timer[timer_id].ptable[i].top    = 0u;
    }
    for (i = 0u ; i < shot_timer[timer_id].active_check_length ; i++)
    {
        shot_timer[timer_id].pactive_check[i] = 0u;
    }

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_TIMER_TimerOneShotOffAll
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_TimerOneShotOffRange
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_TIMER_TimerOneShotOffRange(const uint8_t timer_id,
                                        const uint8_t lower_idx,
                                        const uint8_t upper_idx)
{
    uint16_t i;

    if ((((timer_id >= R_MAX_TIMER_ONE_SHOT_INSTANCES)      ||
          (lower_idx > upper_idx))                          ||
          (upper_idx >= shot_timer[timer_id].table_length)) ||
          (NULL      == shot_timer[timer_id].ptable      ))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    else
    {
        /* do nothing */
    }

    for (i = lower_idx ; i <= upper_idx ; i++)
    {
        shot_timer[timer_id].ptable[i].active = R_FALSE;
        shot_timer[timer_id].ptable[i].top    = 0u;
        {
            uint8_t cnt    = (uint8_t)(i / 8u);
            uint8_t offset = (uint8_t)(i % 8u);

            shot_timer[timer_id].pactive_check[cnt] &= (uint8_t)(~(1u << offset));
        }
    }

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_TIMER_TimerOneShotOffRange
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_TimerOneShotIsRunning
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_TIMER_TimerOneShotIsRunning(const uint8_t timer_id,
                                         const uint8_t handle,
                                         r_boolean_t* pactive)
{
    if ((((timer_id >= R_MAX_TIMER_ONE_SHOT_INSTANCES)      ||
          (handle >= shot_timer[timer_id].table_length))    ||
          (NULL   == shot_timer[timer_id].ptable      ))    ||
          (NULL   == pactive))
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }
    else
    {
        /* do nothing */
    }

    *pactive = shot_timer[timer_id].ptable[handle].active;

    return R_RESULT_SUCCESS;
}
/******************************************************************************
   End of function  R_TIMER_TimerOneShotIsRunning
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_GetExpireTickCount
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_result_t R_TIMER_GetExpireTickCount(uint32_t* pexpire_tick_count,
                                      const uint32_t delta)
{
    uint32_t delta_ticks;

    if (hwtimer_tick_period > 0UL)
    {
        /* Calculate oneshot timeout in tick units */
        delta_ticks = delta / hwtimer_tick_period;

        /* If not multiple of hwtimer_tick_period, round up. */
        if ((delta % hwtimer_tick_period) != 0uL)
        {
            delta_ticks++;
        }
        else
        {
            /* do nothing */
        }
    }
    else
    {
        return R_RESULT_BAD_INPUT_ARGUMENTS;
    }

    if (NULL == pexpire_tick_count)
    {
        return R_RESULT_ILLEGAL_NULL_POINTER;
    }
    else if (delta_ticks >= R_TIMER_HALF_WRAP_INTERVAL_LENGTH)
    {
        /* If larger than maximum, set to maximum. */
        *pexpire_tick_count = tick_count + (R_TIMER_HALF_WRAP_INTERVAL_LENGTH - 1uL);

        return R_RESULT_SUCCESS;
    }
    else
    {
        *pexpire_tick_count = tick_count + delta_ticks; /* time stamp is output in ticks */

        return R_RESULT_SUCCESS;
    }
}
/******************************************************************************
   End of function  R_TIMER_GetExpireTickCount
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_GetRemainingTime
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
uint32_t R_TIMER_GetRemainingTime(const uint32_t expire_tick_count)
{
    uint32_t tick_difference;

    if (tick_count < expire_tick_count)
    {
        tick_difference = expire_tick_count - tick_count;

        if (tick_difference <= R_TIMER_HALF_WRAP_INTERVAL_LENGTH)
        {
            return tick_difference * hwtimer_tick_period; /* number of ms until it will expire */
        }
        else
        {
            return 0uL; /* time has expired */
        }
    }

    /* tick_count >= expire_tick_count */
    else
    {
        tick_difference = tick_count - expire_tick_count;

        if (tick_difference <= R_TIMER_HALF_WRAP_INTERVAL_LENGTH)
        {
            return 0uL; /* time has expired */
        }
        else
        {
            return (expire_tick_count + ((0xFFFFFFFFu - tick_count) + 1u)) * hwtimer_tick_period; /* number of ms until it will expire */
        }
    }
}
/******************************************************************************
   End of function  R_TIMER_GetRemainingTime
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_TickCountExpired
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
r_boolean_t R_TIMER_TickCountExpired(const uint32_t expire_tick_count)
{
    uint32_t tick_difference;

    /* if current tick count smaller than expire tick count time has not yet expired */
    if (tick_count < expire_tick_count)
    {
        tick_difference = expire_tick_count - tick_count;

        if (tick_difference <= R_TIMER_HALF_WRAP_INTERVAL_LENGTH)
        {
            return R_FALSE; /* time has not expired */
        }
        else
        {
            return R_TRUE; /* time has expired */
        }
    }

    /* tick_count >= expire_tick_count */
    else
    {
        tick_difference = tick_count - expire_tick_count;

        if (tick_difference <= R_TIMER_HALF_WRAP_INTERVAL_LENGTH)
        {
            return R_TRUE; /* time has expired */
        }
        else
        {
            return R_FALSE; /* time stamp taken before the wrap around */
        }
    }
}
/******************************************************************************
   End of function  R_TIMER_TickCountExpired
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_BusyWait
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void R_TIMER_BusyWait(uint32_t time_span)
{
    uint32_t expire_tick_count;

    R_TIMER_GetExpireTickCount(&expire_tick_count, time_span);

    while (R_TIMER_TickCountExpired(expire_tick_count) == R_FALSE)
    {
        /* do nothing */
    }
}
/******************************************************************************
   End of function  R_TIMER_BusyWait
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_GetFreerunTime
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
uint32_t R_TIMER_GetFreerunTime(void)
{
    return (tick_count * R_HW_TIMER_TICK_PERIOD);
}
/******************************************************************************
   End of function  R_TIMER_GetFreerunTime
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_Handle
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void R_TIMER_Handle(void)
{
    /* this is the general tick count incremented by one tick by the hardare timer */
    tick_count++;

    timer_int = R_TRUE;
}
/******************************************************************************
   End of function  R_TIMER_Handle
******************************************************************************/

/******************************************************************************
* Function Name:R_TIMER_EventProcess
* Description :
* Arguments : 
* Return Value : 
******************************************************************************/
void R_TIMER_EventProcess(void)
{
    uint16_t i;
    uint16_t j;
    uint16_t k;
    uint16_t m;
    uint16_t offset;

    R_BSP_DI();    // Disable interrupts

    if (R_TRUE == timer_int)
    {
        timer_int = R_FALSE;

        R_BSP_EI(); // Enable interrupts

        /* for all one shot instances */
        for (i = 0u ; i < R_MAX_TIMER_ONE_SHOT_INSTANCES ; i++)
        {
            /* if active */
            if (NULL != shot_timer[i].ptable)
            {
                for (j = 0u ; j < shot_timer[i].active_check_length ; j++)
                {
                    if (0u != shot_timer[i].pactive_check[j])
                    {
                        for (k = 0u ; k < 2 ; k++)
                        {
                            if (((0u == k) && ((shot_timer[i].pactive_check[j] & 0x0f) != 0u)) ||
                                ((1u == k) && ((shot_timer[i].pactive_check[j] & 0xf0) != 0u)))
                            {
                                offset = (uint16_t)((j * 8u) + (k * 4u));
                                for (m = 0u ; m < 4u ; m++, offset++)
                                {
                                    /* Perform oneshot timer control */
                                    if (R_TRUE == shot_timer[i].ptable[offset].active)
                                    {
                                        if (R_TIMER_TickCountExpired(shot_timer[i].ptable[offset].top) == R_TRUE)
                                        {
                                            /* Oneshot timer elapsed */
                                            shot_timer[i].ptable[offset].active = R_FALSE;
                                            shot_timer[i].ptable[offset].top    = 0u;
                                            {
                                                uint8_t byte = (uint8_t)(offset / 8u);
                                                uint8_t bits = (uint8_t)(offset % 8u);
                                                shot_timer[i].pactive_check[byte] &= (uint8_t)(~(1u << bits));
                                            }
                                            if (NULL != shot_timer[i].one_shot_cb)
                                            {
                                                /* Call user callback */
                                                shot_timer[i].one_shot_cb((uint8_t)offset);
                                            }
                                            else
                                            {
                                                /* do nothing */
                                            }
                                        }
                                        else
                                        {
                                            /* do nothing */
                                        }
                                    }
                                    else
                                    {
                                        /* do nothing */
                                    }
                                    if ((shot_timer[i].table_length - 1u) == offset)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        /* do nothing */
                                    }
                                }
                            }
                            else
                            {
                                /* do nothing */
                            }
                        }
                    }
                    else
                    {
                        /* do nothing */
                    }
                }
            }
            else
            {
                /* timer not active */
            }
        }
    }
    else
    {
        R_BSP_EI(); // Enable interrupts
    }

}
/******************************************************************************
   End of function  R_TIMER_EventProcess
******************************************************************************/
