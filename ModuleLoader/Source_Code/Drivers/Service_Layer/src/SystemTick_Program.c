/****************************************************************************************/
/*************************************** Guide ******************************************/
/****************************************************************************************/
/* Author : Hamd Mubarak                                                                */
/*                                                                                      */
/* Functionality :		        						*/
/* ================									*/
/* used to measure execution time between 2 or more points inside you code 		*/
/*											*/
/* How to use :										*/ 
/* ================     			        				*/
/* 1) make sure that SysTick_Handler is proberly activated in the bootloader 		*/
/*	running on the meter							        */
/* 2) put the "S_SysTick_ISR_Cb" in the ISR handler					*/
/* 3) use "S_Start_Systick_Time_Measure" to configure start systick module in the         */
/*	beginning of measurement		        			        */
/* 4) use "S_Get_Systick_Time_Measure" to get the relative or absolute time measure 	*/
/* 5) use "S_Capture_Systick_Time_Point" for capturing multiple points of time (not	*/
/*      greater than the configured S_MAX_TIME_POINT_NUM)	        			*/
/* 6) use "S_Stop_Systick_Time_Measure" at the end if measurements			*/
/*											*/
/* Constraints :									*/ 
/* ================		        						*/
/* 1) don't try to change the CPU clock frequency after starting systick and	        */
/*      before stopping it (during measurement) 					*/
/*		        								*/
/****************************************************************************************/
/****************************************************************************************/



#include "SystemTick_Interface.h"




static uint32_t systick_count = 0;
static uint32_t time_points [S_MAX_TIME_POINT_NUM]={0};
static uint8_t current_index = 0;
static TIME_UNIT current_time_unit = milli_sec;
static uint16_t current_time_res = 0;	//time resolution


/*!
* @brief S_Start_Systick_Time_Measure
*
* @par Description:
* initiailize and configure systick peripheral
* @param[in] time_res : the time resolution of each tick
* @param[in] unit : the time unit of the counted duration
* @return 1 in case of success , 0 else
* @note
* - This function shall be called directly before the starting points of measurements
*/
uint8_t S_Start_Systick_Time_Measure(uint16_t time_res, TIME_UNIT unit){
	uint32_t ticks = 0;
	//uint32_t clk_freq = 0;
	uint8_t ret = 0;
	if(time_res){
		//clk_freq = S_CLK_GetHCLKFreq();
		switch(unit){
			case milli_sec:
				ticks = time_res * (S_CLK_GetHCLKFreq()/1000);
			break;
			case micro_sec:
				ticks = time_res * (S_CLK_GetHCLKFreq()/1000000);
			break;
			default:
			break;
		}
	}
	if(ticks){
		S_CORTEX_SetPriority_ClearPending_Enable(S_SysTick_IRQn, 0);
		if(S_CORTEX_SystemTick_Config(ticks)==0){
			memset(time_points, 0, S_MAX_TIME_POINT_NUM*sizeof(time_points[0]));
			current_index = 0;
			systick_count = 0;
			current_time_res = time_res;
         current_time_unit = unit;
			ret=1;
		}
	}
	return ret;
}


/*!
* @brief S_Stop_Systick_Time_Measure
*
* @par Description:
* Stops the systick peripheral
* @return 1 in case of success , 0 else
* @note
* - This function should be called immediately after finishing measurements
*/
uint8_t S_Stop_Systick_Time_Measure(void){
	uint8_t ret = 0;
	S_CORTEX_NVIC_DisableIRQ(S_SysTick_IRQn);
	ret = 1;
	return ret;
}


/*!
* @brief S_Capture_Systick_Time_Point
*
* @par Description:
* captures and stores a point of time during systick operation
* @param[out] index : the index of the captured point of time
* @return 1 in case of success , 0 else
* @note
* - This function shall be called in case of the need of multiple measurements in the same sys_tick operation 
* - This function wil fail to store the current point od time in case we have already stored "S_MAX_TIME_POINT_NUM" of time points
*/
uint8_t S_Capture_Systick_Time_Point(uint8_t volatile *index){

	uint8_t  ret = 0;
	if(current_index < S_MAX_TIME_POINT_NUM){
		*index = current_index;
		time_points[current_index] = systick_count;
		current_index++;
		ret = 1;
	}
	return ret;
}


/*!
* @brief S_Get_Systick_Time_Measure
*
* @par Description:
* calculates the time measurement between 2 points
* @param[in] start_index : the index of the captured starting time point
* @param[in] stop_index : the index of the captured stopping time point
* @param[out] res : the resulting difference between the 2 input time points
* @return 1 in case of success , 0 else
* @note
* - This function shall be called in case of the need of multiple measurements in the same sys_tick operation 
* - if start_index = stop_index = 0, the result would be the absolute duration since the call of S_Start_Systick_Time_Measure
*/
uint8_t S_Get_Systick_Time_Measure(uint8_t start_index, uint8_t stop_index, SYS_TICK_TIME_STD *res){
	uint8_t ret = 0;
        if(start_index< S_MAX_TIME_POINT_NUM && stop_index< S_MAX_TIME_POINT_NUM){
           if(time_points[stop_index] > time_points[start_index]){
              res->time = current_time_res*(time_points[stop_index]-time_points[start_index]);
              res->unit = current_time_unit;
              ret = 1;
           }
           else if (start_index == 0 && stop_index == 0){
           		res->time = current_time_res * systick_count;
           		res->unit = current_time_unit; 
           		ret = 1;
           }
           else{

           }
        }

	return ret;
}


/*!
* @brief S_SysTick_ISR_Cb
*
* @par Description:
* this function shal be called in the ISR handler for systick interrrupts
*/
void S_SysTick_ISR_Cb(void){
	systick_count++;
}


/*!
* @brief S_Get_SysTick_Counts
*
* @return the number of systick ticks since the start of systick operation
*/
uint32_t S_Get_SysTick_Counts(void){
	return systick_count;
}
