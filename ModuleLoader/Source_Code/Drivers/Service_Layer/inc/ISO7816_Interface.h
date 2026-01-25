#ifndef ISO7816_DRV_H_
#define ISO7816_DRV_H_

#include "config.h"

#if (MicroController == Micro_V94XX)
#include "V94XX_ISO7816.h"
#else
#include "v85xx_iso7816.h"
#endif

#include "ISO7816_Config.h"

void S_ISO7816_INIT(uint8_t ufc, void(*event_cb)(uint32_t event));
void S_ISO7816_DE_INIT(uint8_t ifc);
uint8_t S_ISO7816_RECEIVE(uint8_t ifc, uint8_t *data);
uint32_t S_ISO7816_Received_Data_Count(uint8_t ifc);
uint8_t S_ISO7816_TRANSMIT(uint8_t ifc, uint8_t *data,  uint8_t len);
uint32_t S_ISO7816_Sent_Data_Count(uint8_t ifc);
uint8_t S_ISO7816_SET_Baudrate(uint8_t ifc, uint32_t baudrate);
uint32_t S_ISO7816_GET_Baudrate(uint8_t ifc);
uint8_t S_ISO7816_ABORT_Operation(uint8_t ifc);
#endif /* ISO7816_DRV_H_ */
