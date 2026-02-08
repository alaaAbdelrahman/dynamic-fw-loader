/*
 * task_priority.c
 *
 *  Created on: May 21, 2018
 *      Author: islam haggag
 */

#include "task_priority.h"
__packed typedef struct
{
  uint16_t current_priority;
} TaskPriority_t;



static TaskPriority_t TaskPriority;



uint8_t task_priority_get(TaskPriorityLevels_t TaskPriorityLevels)
{
  if((!TaskPriority.current_priority))
  {
    return TRUE;
  }
  else if(TaskPriority.current_priority < (TaskPriorityLevels << (uint16_t)0x01))
  {
    return TRUE;
  }
  return FALSE;
}


void task_priority_set(TaskPriorityLevels_t TaskPriorityLevels)
{
  TaskPriority.current_priority |= (uint16_t)TaskPriorityLevels;
}


void task_priority_release(TaskPriorityLevels_t TaskPriorityLevels)
{
  TaskPriority.current_priority &= ~(uint16_t)TaskPriorityLevels;
}


