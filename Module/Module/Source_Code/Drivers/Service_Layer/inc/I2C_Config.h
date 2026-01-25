
#ifndef I2C_Config_H_
#define I2C_Config_H_


/********************************** Headers ***********************************/
#define S_FAIL                                       0x00
#define S_SUCCESS                                    0x01
#define I2C_RFID_ADDRESS7                            0x28   /* RFID_ADDRESS*/

#define S_I2C_ASSERTACKNOWLEDGE_DISABLE              I2C_ASSERTACKNOWLEDGE_DISABLE
#define S_I2C_CLOCKSOURCE_APBD                       I2C_CLOCKSOURCE_APBD60
#define S_I2C_GENERALCALLACK_DISABLE                 I2C_GENERALCALLACK_DISABLE
#define S_I2C_SUBADDR_2BYTE                          I2C_SUBADDR_2BYTE
#define S_I2C_SUBADDR_1BYTE                          I2C_SUBADDR_1BYTE
#define S_EEPROM_I2C                                 EEPROM_I2C
#define S_RFID_I2C                                   RFID_I2C

                              
 
#define S_I2C_DeInit()                               I2C_DeInit()
#define S_I2C_Init(Struct)                           I2C_Init(Struct)
#define S_I2C_Dis_EN(State)                          I2C_Cmd(State)
#define S_I2C_MasterWriteBytes(Struct)               I2C_MasterWriteBytes(Struct)
#define S_I2C_MasterReadBytes(Struct)                I2C_MasterReadBytes(Struct)
#define S_I2C_SendStop()                             I2C_SendStop()
 

#endif /* I2C_Config_H_ */
