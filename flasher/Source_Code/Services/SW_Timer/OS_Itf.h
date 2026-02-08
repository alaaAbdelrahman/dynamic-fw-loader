//=============================================================================
//
//                       FILE NAME : OS_Itf.h
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

#ifndef OS_Itf_H
#define OS_Itf_H
#include "Config.h"


//=============================================================================
//                 TYPES DEFINITION
//=============================================================================
/* timers callback */
typedef void (*TOS_pfTimerCallBack)();

/* timers List */
typedef enum
{
  /* Timer recv RS */
  OS_ePhyRsItf = 0,
  /* Timer recv IR */
  OS_ePhyItf,
  /* Timer recv IP */
  OS_eIpItf,
  /* Pwd Protection */
  OS_eDlmsTimerAsso,
  /* Inactivity Hdlc timer */
  OS_eHdlcTimerInactivity,
  /* inactivity IP timer */
  OS_eIpTimerInactivity,
  OS_eTimerSchdulerRecv,
  OS_eIpTimerCkeckSocket, //added for specific use of ELSEWEDY
#ifdef RS485_NETWORK
  OS_eRS485Network,
#endif  
  OS_eModemTimer,
  OS_eLimiterTimer,
  OS_ePowerQualityTimer,
  OS_eProfiles_Comm_Timer,
  OS_eActivityCalendarTimer,
  OS_eMaxTimer
}tOS_eTimer;


//=============================================================================
//                       DEFINITIONs  
//=============================================================================
#define iSUCCESS            (0)
#define iERROR              (-1)
//=============================================================================
//                 GLOBAL FUNCTIONS DEFINITION  
//=============================================================================
extern int32_t OS_gTimerInitialize(void);
extern int32_t OS_gCreateTimer(tOS_eTimer, uint32_t uiValue, TOS_pfTimerCallBack);
extern int32_t OS_gStartTimer(tOS_eTimer);
extern int32_t OS_gStopTimer(tOS_eTimer);
extern uint8_t OS_gTimerState(tOS_eTimer eTimer);
#ifndef _WIN32_
extern void OS_gTimerManager();
#else
/* Nothing */
#endif
int32_t ComputeTimeDiffrence(TstDateTime MonitoredDateTime, TstDateTime ComparativeDateTime);
extern void OS_gFatalError(uint32_t, int8_t*, int32_t);
extern void OS_gRaiseError(int8_t*, uint32_t, int8_t*, int32_t);
extern void OS_gLog(int8_t*);

#endif /* OS_Itf_H*/
