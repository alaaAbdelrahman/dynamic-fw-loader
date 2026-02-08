/*
 * task_priority.h
 *
 *  Created on: May 21, 2018
 *      Author: islam haggag
 */

#ifndef SOURCE_CODE_TASK_PRIORITY_TASK_PRIORITY_H_
#define SOURCE_CODE_TASK_PRIORITY_TASK_PRIORITY_H_

#include "config.h"

typedef enum
{
  PRIORITY_FOR_METER_TASK         = (uint16_t)0x2000,
  PRIORITY_FOR_COMM_TASK          = (uint16_t)/*0x10*/0x1000,
  PRIORITY_FOR_CONTROL_TASK       = (uint16_t)/*0x08*/0x800,
  PRIORITY_FOR_Test_Mode_TASK     = (uint16_t)/*0x07*/0x400,
  PRIORITY_FOR_lpm_task           = (uint16_t)/*0x06*/0x200,
  PRIORITY_FOR_Optical_task       = (uint16_t)0x100,
  PRIORITY_FOR_meter_modes_task   = (uint16_t)0x80,
  PRIORITY_FOR_alarms_task        = (uint16_t)0x40,
  PRIORITY_FOR_calender_task      = (uint16_t)0x20,
  PRIORITY_FOR_TARRIF_TASK        = (uint16_t)0x10,
  PRIORITY_FOR_BillingProfile_task = (uint16_t)0x08,
  PRIORITY_FOR_DISPLAY_TASK       = (uint16_t)0x04,
  PRIORITY_FOR_SELF_TEST_TASK     = (uint16_t)0x01,
  PRIORITY_FOR_NO_TASK            = (uint16_t)0x00,
} TaskPriorityLevels_t;



uint8_t task_priority_get(TaskPriorityLevels_t TaskPriorityLevels);


void task_priority_set(TaskPriorityLevels_t TaskPriorityLevels);


void task_priority_release(TaskPriorityLevels_t TaskPriorityLevels);



#endif /* SOURCE_CODE_TASK_PRIORITY_TASK_PRIORITY_H_ */
