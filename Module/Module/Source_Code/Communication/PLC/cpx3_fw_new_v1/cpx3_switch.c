

#include "config.h"

#include "CPX3_booting.h"
#include "cpx3_switch.h"
#include "Flash.h"
#include "r_typedefs.h"
#include "r_config.h"
#include "r_stdio_api.h"
#if !defined(R_DEFINE_OPEN_RX)
#include "r_bsp_api.h"
#include "r_byte_swap.h"
#include "r_app_timer.h"
#include "r_mem_tools.h"
#include "r_fw_download.h"
#endif /* R_DEFINE_OPEN_ARM */

/* g3 part */
#include "r_c3sap_api.h"

/* app part */
#include "r_demo_tools.h"
#include "r_demo_app.h"
#include "r_demo_sys.h"
#include "r_demo_app_thread.h"
#include "r_demo_app_eap.h"

#include "r_demo_nvm_process.h"
#include "r_demo_main.h"
extern uint8_t start_config_set;
extern uint8_t config_monitor;
extern uint8_t get_handle_state;
extern uint8_t current_AppSetIpContent;
extern uint8_t flag_G3_Init_Send;
extern uint8_t current_AppResetTask;
extern uint8_t current_AppPresetProcess;
extern uint8_t current_AppJoinNetwork;

uint8_t cpx3_current_task = CPX3_BOOTING_TASK; /* variable hold current task */

void Rest_flags(void)
{
	cpx3_current_task = CPX3_BOOTING_TASK;
	start_config_set = 0;
	config_monitor = 0;
	get_handle_state = 1;
	flag_G3_Init_Send = 1;
	current_AppSetIpContent = CPX3_TX_GAIN;
	current_AppResetTask = CPX3_ADP_SET_CONFIG;
	current_AppPresetProcess = CPX3_SET_IB_FRAME_COUNTER;
	current_AppJoinNetwork = CPX3_GET_R_ADP_IB_METRIC_TYPE;
}

Error_State CPX3_Handle(void)
{

	Error_State booting_state = SUCCESS;

	switch (cpx3_current_task)
	{
	case CPX3_BOOTING_TASK:

		booting_state = CPX3_Booting();

		if (booting_state == UART_SEND_BUSY)
		{
			cpx3_current_task = CPX3_BOOTING_TASK;
		}
		else if (booting_state == MODE_BOOT_END)
		{
			cpx3_current_task = CPX3_PLC_INIT;
		}
		else if (booting_state == SUCCESS)
		{
			cpx3_current_task = CPX3_BOOTING_TASK;
		}
		else
		{
			return booting_state;
		}

		break;

	case CPX3_PLC_INIT:
		plc_init();
		break;
	case CPX3_APD_INIT:
		CPX3_ADP_Init();
		break;
	case CPX3_APP_RESET:
		CPX3_AppResetDevice();
		break;
	case CPX3_APP_SET_IP_CONTENT:
		CPX3_AppSetIpContent();
		break;
	case CPX3_APP_PRESET_PROCESS:
		CPX3_AppPresetProcess();
		break;
	case CPX3_APP_WAIT_INSERT:
		CPX3_AppWaitInsert();
		break;
	case CPX3_APP_GET_NETWORK_PARM:
		CPX3_AppGetNewtorkParm();
		break;
	case CPX3_APP_SCAN_NETWORK:
		CPX3_AppScantNewtork();
		break;
	case CPX3_APP_JOIN_NETWORK:
		CPX3_AppJointNewtork();
		break;

	default:
		break;
	}
}
