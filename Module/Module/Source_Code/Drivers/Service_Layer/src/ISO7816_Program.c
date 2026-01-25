#include "ISO7816_Interface.h"
#include "basic_srv.h"

ISO7816_InitType Init_ISO7816[S_MAX_ISO7816_IFC];

void S_ISO7816_INIT(uint8_t ifc,  void(*event_cb)(uint32_t event)){
	S_ISO7816_DE_INIT(ifc);
	S_ISO7816_StructInit((ISO7816_InitType *)&Init_ISO7816[ifc], ifc, event_cb);
	S_ISO7816_Init((ISO7816_InitType *)&Init_ISO7816[ifc]);
}

void S_ISO7816_DE_INIT(uint8_t ifc){
		S_ISO7816_DeInit(&Init_ISO7816[ifc]);
}

uint8_t S_ISO7816_RECEIVE(uint8_t ifc, uint8_t *data){
	uint8_t res = 0;
	res = S_ISO7816_AbortOperation(ifc);
	res = (res?S_ISO7816_ReceiveData(ifc, data):0);
	return res;
}

uint32_t S_ISO7816_Received_Data_Count(uint8_t ifc){
	return S_ISO7816_GetRxCount(ifc);
}

uint32_t S_ISO7816_Sent_Data_Count(uint8_t ifc){
	return S_ISO7816_GetTxCount(ifc);
}

uint8_t S_ISO7816_TRANSMIT(uint8_t ifc, uint8_t *data,  uint8_t len){
	uint8_t res = 0;
	res = S_ISO7816_AbortOperation(ifc);
	res = (res?S_ISO7816_TransmitData(ifc, data, len):0);
	return res;
}

uint8_t S_ISO7816_SET_Baudrate(uint8_t ifc, uint32_t baudrate){
	uint8_t res = 0;
	res = S_ISO7816_AbortOperation(ifc);
	res = (res?S_ISO7816_BaudrateConfig(ifc, baudrate):0);
	return res;
}

uint32_t S_ISO7816_GET_Baudrate(uint8_t ifc){
	uint32_t baudrate = 0;
	S_ISO7816_GetBaudrate(ifc, &baudrate);
	return baudrate;
}


uint8_t S_ISO7816_ABORT_Operation(uint8_t ifc){
	uint8_t ret = 0;
	ret = S_ISO7816_AbortOperation(ifc);
	return ret;
}
