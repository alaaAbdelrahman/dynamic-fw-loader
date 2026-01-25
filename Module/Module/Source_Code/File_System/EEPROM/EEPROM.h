#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "config.h"
#include "EEPROM_Addresses.h"


#define  TBool uint8_t 

extern volatile uint16_t eeprom_load_errors_register; // indicate if we load wrong data from the eeprom 
__packed struct Eeprom_check_t
{
  uint8_t eeprom_check_arr[30];
  uint8_t eeprom_crc;

};

void EEPROM_Initialize(void);
void EEPROM_PowerDown(void);
uint8_t EEPROM_write(uint8_t EEPROM_Add ,  uint8_t* TxData,uint32_t Address, uint32_t numBytes);
uint8_t EEPROM_read(uint8_t EEPROM_Add ,  uint8_t* RxData,uint32_t Address, uint32_t numBytes);
void EEPROM_Failure_Handler(void);
uint8_t EEPROM_Erease(uint8_t EEPROM_Add);
uint8_t EEPROM_test(uint8_t EEPROM_Add);
void eeprom_check_init(void);
void check_Eeprom_Fail(void);


#endif
