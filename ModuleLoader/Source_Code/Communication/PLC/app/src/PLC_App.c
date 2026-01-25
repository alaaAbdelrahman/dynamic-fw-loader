#include "stdint.h"
#include "r_typedefs.h"
#include "r_c3sap_api.h"
#include "r_g3_param2text.h"

#include "r_demo_app.h"
#include "r_demo_sys.h"
#include "r_demo_app_eap.h"
#include "r_demo_nvm_process.h"
#include "r_demo_print.h"

#include "r_ipv6_headers.h"
#include "r_udp_headers.h"
#include "r_demo_tools.h"
#include "r_demo_api.h"
#include "r_demo_common.h"
#include "r_demo_config.h"
#include "r_demo_statistics.h"
#include "r_demo_ib_access.h"
#include "r_demo_app_ver.h"
#include "r_demo_parameters.h"
#include "r_demo_os_wrapper.h"
#include "r_bsp_api.h"
#include "PLC_App.h"
extern const r_demo_config_t       g_cdemo_config;
extern r_demo_config_t             g_demo_config;
extern r_demo_entity_t             g_demo_entity;
extern r_demo_buff_t               g_demo_buff;


void PLC_Send(uint8_t* msg, uint16_t length){
  for(uint16_t i = length ; i > 0 ;){
    if(i < 300){
      R_DEMO_SendUdpFrame (i,
                       g_demo_entity.panId,
                       g_demo_entity.shortAddress,
                       0xFFFF,
                       (msg) + (length) - i);
      i = 0;
    }
    else{
      R_DEMO_SendUdpFrame (300,
                       g_demo_entity.panId,
                       g_demo_entity.shortAddress,
                       0xFFFF,
                       (msg) + (length) - i);
      i -= 300;
    }
  }
}
