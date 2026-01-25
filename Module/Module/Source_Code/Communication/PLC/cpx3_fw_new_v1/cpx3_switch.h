#ifndef CPX3_SWITCH_H
#define CPX3_SWITCH_H

/*Define the states for CPX3 handling process*/
#define CPX3_BOOTING_TASK 0 /* state for CPX3 Booting process*/
#define CPX3_PLC_INIT 1
#define CPX3_APP_INIT 2
#define CPX3_APD_INIT 3
#define CPX3_APP_RESET 4
#define CPX3_APP_WAIT_INSERT 5
#define CPX3_APP_GET_NETWORK_PARM 6
#define CPX3_APP_SCAN_NETWORK 7
#define CPX3_APP_JOIN_NETWORK 10
#define CPX3_APP_SET_IP_CONTENT 8
#define CPX3_APP_PRESET_PROCESS 9

void Rest_flags(void);

#endif 