//=============================================================================
//
//                       FILE NAME : OS_Timer.c
//                       
//                       MODULE    : OS 
//                       
//                       PROJECT   : Dlms Cosem STACK
//
//=============================================================================
//  
//    This is a TELNET's document. Reproduction and Disclosure are forbidden
//    
//=============================================================================

//=============================================================================
//                 MACROS DEFINITION
//=============================================================================
#include "config.h"
#include "OS_Itf.h"
#include "string.h"
#include "RTC_Interface.h"
#include "OS_Itf.h"
#include <time.h>
#ifdef _WIN32_
#define _WIN32_WINNT 0x0400
#include <windows.h>
#include <winbase.h>
#endif

//=============================================================================
//                 TYPES DEFINITION
//=============================================================================
__packed typedef struct
{
  uint8_t bStarted;
  uint32_t uiValue;
  uint32_t uiTempo;
  TOS_pfTimerCallBack pfCallback;
}TOS_stTimer;

//=============================================================================
//                 LOCAL VARIABLES DEFINITION
//=============================================================================
TOS_stTimer OS_tstTimer[OS_eMaxTimer];

#ifdef _WIN32_
static HANDLE OSi_hTimer = INVALID_HANDLE_VALUE;
#endif

//=============================================================================
//                 GLOBAL FUNCTIONS DEFINITION  
//=============================================================================

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : OS_gTimerManager
// Description    : Timer manager
//-----------------------------------------------------------------------------
// Author         : HBM  
// Date           : 20/01/2011       
//=============================================================================
#ifdef _WIN32_
VOID APIENTRY OSi_Timer(PVOID lpArg, DWORD dwTL, DWORD dwTH)
#else
void OS_gTimerManager()
#endif
{
  uint8_t uii;
  for(uii = 0; uii < OS_eMaxTimer; uii++)
  {
    if(OS_tstTimer[uii].bStarted)
    { /* managed timer */
      if(OS_tstTimer[uii].uiTempo)
      {
        OS_tstTimer[uii].uiTempo--;
      }
      if(OS_tstTimer[uii].uiTempo == 0)
      { 
        OS_tstTimer[uii].bStarted = FALSE;
        if(OS_tstTimer[uii].pfCallback)
        {
          OS_tstTimer[uii].pfCallback();
        }
      }
    }
  }
}

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : OS_gTimerInitialize
// Description    : Timer initialization
//-----------------------------------------------------------------------------
// Author         : HBM  
// Date           : 20/01/2011       
//=============================================================================
int32_t OS_gTimerInitialize(void)
{
  int32_t iRetVal = iERROR;

#ifdef _WIN32_
  LARGE_INTEGER li;
  LONG lPeriod;
  lPeriod = Dlms_mOsTimeSlice;
  li.QuadPart = -(LONGLONG)lPeriod * lPeriod;

  memset(OS_tstTimer, 0x00, sizeof(OS_tstTimer));
  OSi_hTimer = CreateWaitableTimer(NULL, true, NULL);
  if(OSi_hTimer != INVALID_HANDLE_VALUE)
  {
    CancelWaitableTimer(OSi_hTimer);
    SetWaitableTimer(OSi_hTimer, &li, lPeriod, OSi_Timer, NULL, FALSE);
    iRetVal = iSUCCESS;
  }
#else
  memset(OS_tstTimer, 0x00, sizeof(OS_tstTimer));
  iRetVal = iSUCCESS;
#endif
  return iRetVal;
}

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : OS_gCreateTimer
// Description    : Create Timer 
//-----------------------------------------------------------------------------
// Author         : HBM  
// Date           : 20/01/2011       
//=============================================================================
int32_t OS_gCreateTimer(tOS_eTimer eTimer,
                    uint32_t uiValue,
                    TOS_pfTimerCallBack pfCallback)
{
  int32_t iRetVal = iERROR;
#ifdef _WIN32_
  if((eTimer < OS_eMaxTimer) && (OSi_hTimer != INVALID_HANDLE_VALUE))
#else
  if(eTimer < OS_eMaxTimer)
#endif
  {
    iRetVal = iSUCCESS;
    OS_tstTimer[eTimer].bStarted = FALSE;
    OS_tstTimer[eTimer].uiValue = uiValue;
    OS_tstTimer[eTimer].uiTempo = uiValue;
    OS_tstTimer[eTimer].pfCallback = pfCallback;
  }

  return(iRetVal);
}
uint8_t OS_gTimerState(tOS_eTimer eTimer)
{
    return OS_tstTimer[eTimer].bStarted;
}
//=============================================================================
// FUNCTION
//=============================================================================
// Name           : OS_gStartTimer
// Description    : Start Timer 
//-----------------------------------------------------------------------------
// Author         : HBM  
// Date           : 20/01/2011       
//=============================================================================
int32_t OS_gStartTimer(tOS_eTimer eTimer)
{
  int32_t iRetVal = iERROR;
#ifdef _WIN32_
  if((eTimer < OS_eMaxTimer) && (OSi_hTimer != INVALID_HANDLE_VALUE))
#else
  if(eTimer < OS_eMaxTimer)
#endif
  {
    iRetVal = iSUCCESS;
    OS_tstTimer[eTimer].bStarted = TRUE;
    OS_tstTimer[eTimer].uiTempo = OS_tstTimer[eTimer].uiValue;
  }
  return iRetVal;
}

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : OS_gStopTimer
// Description    : Stop Timer 
//-----------------------------------------------------------------------------
// Author         : HBM  
// Date           : 20/01/2011       
//=============================================================================
int32_t OS_gStopTimer(tOS_eTimer eTimer)
{
  int32_t iRetVal = iERROR;
#ifdef _WIN32_
  if((eTimer < OS_eMaxTimer) && (OSi_hTimer != INVALID_HANDLE_VALUE))
#else
  if(eTimer < OS_eMaxTimer)
#endif
  {
    iRetVal = iSUCCESS;
    OS_tstTimer[eTimer].bStarted = FALSE;
    OS_tstTimer[eTimer].uiTempo = OS_tstTimer[eTimer].uiValue;
  }
  return iRetVal;
}

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : Get_LastDayOfMonth
// Description    : Get last day of month - last Monday ... last Sunday
//-----------------------------------------------------------------------------
// Author         : JM
// Date           : 28/04/2011
//=============================================================================
uint8_t Get_LastDayOfMonth(uint8_t DM, uint8_t DW, uint8_t LD, uint8_t TD)
{
    //DM = day of month from RTC (1..31)
	//DW = day of week from RTC (1 .. 7)
	//LD = last day of week in month (last Monday .... last Sunday, 1 .. 7)
	//TD = Total number of days in months (28, 29, 30, 31)
    TS08 temp;
    temp = (DM + (LD - DW));
    temp = 7 * (uint8_t)((TD - temp) / 7) + temp;
    if(temp > TD) temp = temp - 7;
    return((uint8_t)temp);
}


//=============================================================================
// FUNCTION
//=============================================================================
// Name           : LeapYear
// Description    : return value > 0 - leap year
//                               = 0 - no leap year
//-----------------------------------------------------------------------------
// Author         : JM
// Date           : 28/04/2011
//=============================================================================
//uint8_t LeapYear(uint16_t year)
//{
//    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
//}
//=============================================================================
// FUNCTION
//=============================================================================
// Name           : Get_TotalNumberOfDays
// Description    : Get total number of days in month
//-----------------------------------------------------------------------------
// Author         : JM
// Date           : 28/04/2011
//=============================================================================
//uint8_t Get_TotalNumberOfDays(uint8_t month, uint8_t year)
//{
//    if(month == 2)
//    {
//        if(LeapYear(year)) return(29);
//        else return (28);
//    }
//    if((month >= 1) && (month <= 7))
//    {
//        if(month % 2 == 0) return(30);
//        else return (31);
//    }
//    else
//    {
//        if(month % 2 == 0) return(31);
//        else return (30);
//    }
//}
//=============================================================================
// FUNCTION
//=============================================================================
// Name           : ComputeTimeDiffrence
// Description    : compute time diffrence in second - return value
//                 dt = MonitoredDateTime - ComparativeDateTime
//                 dt can be positive or negative
//                 MonitoredDateTime is without DLMS wildcard values (value from RTC)
//                 day of week is neccessary

//                 ComparativeDateTime can contain DLMS wildcard values
//                 (value from activation time of limiter, daylight savings end...)
//                 day of week is ignored
//-----------------------------------------------------------------------------
// Author         : JM
// Date           : 28/04/2011
//=============================================================================

int32_t ComputeTimeDiffrence(TstDateTime MonitoredDateTime, TstDateTime ComparativeDateTime)
{
    TstDateTime TempDateTime;
    TU08 DM;  //DM = day of month from RTC
    TU08 DW; //DW = day of week from RTC
    TU08 LD; //LD = last day of week in month (last Monday .... last Sunday)
    TU08 TD; //TD = Total number of days in months
    struct tm time1_struct;
    struct tm time2_struct;
    time_t time1_sec;
    time_t time2_sec;
    //year
    if(ComparativeDateTime.stDate.uiYear == 0xFFFF)
        TempDateTime.stDate.uiYear =  MonitoredDateTime.stDate.uiYear;
    else
        TempDateTime.stDate.uiYear =  ComparativeDateTime.stDate.uiYear;
    //month
    switch(ComparativeDateTime.stDate.uiMonth)
    {
    case 0xFD: //daylight savings end
        //TempDateTime.stDate.uiMonth = month of daylight savings end
        break;
    case 0xFE: //daylight saving begin
        //TempDateTime.stDate.uiMonth = month of daylight savings begin
        break;
    case 0xFF: // not specified
        TempDateTime.stDate.uiMonth =  MonitoredDateTime.stDate.uiMonth;
        break;
    default: // exactly specified
        TempDateTime.stDate.uiMonth =  ComparativeDateTime.stDate.uiMonth;
        break;
    }
    //day of month
    switch(ComparativeDateTime.stDate.uiDay)
    {
    case 0xFD: // 2nd last day of month
        //TD = Total number of days in months
        TD = Get_TotalNumberOfDays(TempDateTime.stDate.uiMonth, TempDateTime.stDate.uiYear);
        if(ComparativeDateTime.stDate.uiDayOfWeek == 0xFF)
            TempDateTime.stDate.uiDay = TD - 1;
        else
        {
            LD = ComparativeDateTime.stDate.uiDayOfWeek; //LD = last day of week in month (last Monday .... last Sunday)
            DM = MonitoredDateTime.stDate.uiDay; //DM = day of month from RTC
            DW = MonitoredDateTime.stDate.uiDayOfWeek; //DW = day of week from RTC
            TempDateTime.stDate.uiDay = Get_LastDayOfMonth(DM, DW, LD, TD) - 7;
        }
        break;
    case 0xFE: // last day of month
        //TD = Total number of days in months
        TD = Get_TotalNumberOfDays(TempDateTime.stDate.uiMonth, TempDateTime.stDate.uiYear);
        if(ComparativeDateTime.stDate.uiDayOfWeek == 0xFF)
            TempDateTime.stDate.uiDay = TD;
        else
        {
            LD = ComparativeDateTime.stDate.uiDayOfWeek; //LD = last day of week in month (last Monday .... last Sunday)
            DM = MonitoredDateTime.stDate.uiDay; //DM = day of month from RTC
            DW = MonitoredDateTime.stDate.uiDayOfWeek; //DW = day of week from RTC
            TempDateTime.stDate.uiDay = Get_LastDayOfMonth(DM, DW, LD, TD);
        }
        break;
    case 0xFF: // not specified
        TempDateTime.stDate.uiDay =  MonitoredDateTime.stDate.uiDay;
        break;
    default: // exactly specified
        TempDateTime.stDate.uiDay =  ComparativeDateTime.stDate.uiDay;
        break;
    }
    //hours
    if(ComparativeDateTime.stTime.uiHour == 0xFF)
        TempDateTime.stTime.uiHour =  MonitoredDateTime.stTime.uiHour;
    else
        TempDateTime.stTime.uiHour =  ComparativeDateTime.stTime.uiHour;
    // minutes
    if(ComparativeDateTime.stTime.uiMin == 0xFF)
        TempDateTime.stTime.uiMin =  MonitoredDateTime.stTime.uiMin;
    else
        TempDateTime.stTime.uiMin =  ComparativeDateTime.stTime.uiMin;
    // seconds
    if(ComparativeDateTime.stTime.uiSec == 0xFF)
        TempDateTime.stTime.uiSec =  MonitoredDateTime.stTime.uiSec;
    else
        TempDateTime.stTime.uiSec =  ComparativeDateTime.stTime.uiSec;
    /*
        printf ("year = %d\n", TempDateTime.stDate.uiYear);
        printf ("month = %d\n", TempDateTime.stDate.uiMonth);
        printf ("day of month = %d\n", TempDateTime.stDate.uiDay);
        printf ("day of week = %d\n", TempDateTime.stDate.uiDayOfWeek);
        printf ("hour = %d\n", TempDateTime.stTime.uiHour);
        printf ("minute = %d\n", TempDateTime.stTime.uiMin);
        printf ("second = %d\n\n", TempDateTime.stTime.uiSec);
    */
    time1_struct.tm_sec = MonitoredDateTime.stTime.uiSec;
    time1_struct.tm_min = MonitoredDateTime.stTime.uiMin;
    time1_struct.tm_hour = MonitoredDateTime.stTime.uiHour;
    time1_struct.tm_mday = MonitoredDateTime.stDate.uiDay; /* days of month 1-31 */
    time1_struct.tm_mon = MonitoredDateTime.stDate.uiMonth - 1; /* month 0-11 */
    time1_struct.tm_year = MonitoredDateTime.stDate.uiYear - 2000; /* year from 1900 */
    time1_struct.tm_wday = 0;               /* den v t�dnu 0-6, 0 je nedele */
    time1_struct.tm_yday = 0;               /* den v roce 0-365 */
    time1_struct.tm_isdst = 0;              /* p��znak letn�ho �asu*/
    time2_struct.tm_sec = TempDateTime.stTime.uiSec;
    time2_struct.tm_min = TempDateTime.stTime.uiMin;
    time2_struct.tm_hour = TempDateTime.stTime.uiHour;
    time2_struct.tm_mday = TempDateTime.stDate.uiDay; /* days of month 1-31 */
    time2_struct.tm_mon = TempDateTime.stDate.uiMonth - 1; /* month 0-11 */
    time2_struct.tm_year = TempDateTime.stDate.uiYear - 2000;  /* year from 1900 */
    time2_struct.tm_wday = 0;               /* den v t�dnu 0-6, 0 je nedele */
    time2_struct.tm_yday = 0;               /* den v roce 0-365 */
    time2_struct.tm_isdst = 0;              /* p��znak letn�ho �asu*/
    time1_sec = mktime(&time1_struct);
    time2_sec = mktime(&time2_struct);
    return ((TS32)difftime(time1_sec, time2_sec));
}