#ifndef   ISO7816_CONFIG_H
#define   ISO7816_CONFIG_H
#if (MicroController == Micro_V94XX)
#include "V94XX_ISO7816.h"
#else
#include "v85xx_iso7816.h"
#endif


#define     S_MAX_ISO7816_IFC                                   MAX_ISO7816_IFC
#define     S_ISO7816_StructInit(INIT,IFC,EVENT)                ISO7816_StructInit(INIT, IFC, EVENT)
#define     S_ISO7816_Init(INIT_ISO)                            ISO7816_Init(INIT_ISO)
#define     S_ISO7816_DeInit(Init_ISO_arr)                      ISO7816_DeInit(Init_ISO_arr)
#define     S_ISO7816_AbortOperation(IFC)                       ISO7816_AbortOperation(IFC)
#define     S_ISO7816_ReceiveData(IFC,DATA)                     ISO7816_ReceiveData(IFC,DATA)
#define     S_ISO7816_GetRxCount(IFC)                           ISO7816_GetRxCount(IFC)
#define     S_ISO7816_GetTxCount(IFC)                           ISO7816_GetTxCount(IFC)
#define     S_ISO7816_TransmitData(IFC,DATA,LENGTH)             ISO7816_TransmitData(IFC,DATA,LENGTH)
#define     S_ISO7816_BaudrateConfig(IFC,BAUDRATE)              ISO7816_BaudrateConfig(IFC,BAUDRATE)
#define     S_ISO7816_GetBaudrate(IFC,AddressBAUDRATE)          ISO7816_GetBaudrate(IFC,AddressBAUDRATE)

#if (MicroController == Micro_V94XX)
//
#else
#define     S_ISO7816_Disable_ISO78160()                        ISO7816_Cmd(ISO78160,DISABLE)
#define     S_ISO7816_Disable_ISO78161()                        ISO7816_Cmd(ISO78161,DISABLE)
#endif

#endif    /* ISO7816_CONFIG_H */