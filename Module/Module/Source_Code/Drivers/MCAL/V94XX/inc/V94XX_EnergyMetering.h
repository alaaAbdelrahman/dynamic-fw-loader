/**
  ******************************************************************************
  * @file   V94XX_EnergyMetering.h 
  * @author  Application Team
  * @version V2.0.6
  * @date    2022-12-9
  * @brief   Energy metering library
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef __V94XX_ENERGYMETERING_H
#define __V94XX_ENERGYMETERING_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "config.h"
#ifdef V9381_ENABLE
#include "V94XX.h"

#define EM_ANA_CTRL0            0x00
#define EM_ANA_CTRL1            0x01
#define EM_CTRL0                0x02
#define EM_CTRL1                0x03
#define EM_CTRL2                0x04
#define EM_CTRL3                0x05
#define EM_CTRL4                0x06
#define EM_CTRL5                0x07
#define EM_DAT_PA               0x08
#define EM_DAT_QA               0x09
#define EM_DAT_SA               0x0a
#define EM_DAT_PB               0x0b
#define EM_DAT_QB               0x0c
#define EM_DAT_SB               0x0d
#define EM_DAT_RMS0UA           0x0e
#define EM_DAT_RMS0IA           0x0f
#define EM_DAT_RMS0IB           0x10
#define EM_DAT_CH1              0x11
#define EM_DAT_CH2              0x12
#define EM_DAT_PA1              0x13
#define EM_DAT_QA1              0x14
#define EM_DAT_SA1              0x15
#define EM_DAT_PB1              0x16
#define EM_DAT_QB1              0x17
#define EM_DAT_SB1              0x18
#define EM_DAT_RMS1U            0x19
#define EM_DAT_RMS1IA           0x1a
#define EM_DAT_RMS1IB           0x1b
#define EM_DAT_CH1_AVG          0x1c
#define EM_DAT_CH2_AVG          0x1d
#define EM_DAT_RMSU_AVG         0x1e
#define EM_DAT_RMSIA_AVG        0x1f
#define EM_DAT_RMSIB_AVG        0x20
#define EM_DAT_FRQ              0x21
#define EM_DAT_DCU              0x22
#define EM_DAT_DCIA             0x23
#define EM_DAT_DCIB             0x24
#define EM_CFG_CALI_PA          0x25
#define EM_CFG_DC_PA            0x26
#define EM_CFG_CALI_QA          0x27
#define EM_CFG_DC_QA            0x28
#define EM_CFG_CALI_PB          0x29
#define EM_CFG_DC_PB            0x2a
#define EM_CFG_CALI_QB          0x2b
#define EM_CFG_DC_QB            0x2c
#define EM_CFG_CALI_RMSU        0x2d
#define EM_CFG_RMS_DCU          0x2e
#define EM_CFG_CALI_RMSIA       0x2f
#define EM_CFG_RMS_DCIA         0x30
#define EM_CFG_CALI_RMSIB       0x31
#define EM_CFG_RMS_DCIB         0x32
#define EM_CFG_PHC              0x33
#define EM_CFG_DCU              0x34
#define EM_CFG_DCIA             0x35
#define EM_CFG_DCIB             0x36
#define EM_CFG_BPF              0x37
#define EM_CFG_CKSUM            0x38
#define EM_EGY_CRPTH            0x39
#define EM_EGY_PWRTH            0x3a
#define EM_EGY_CONST1           0x3b
#define EM_EGY_OUT1L            0x3c
#define EM_EGY_OUT1H            0x3d
#define EM_EGY_CFCNT1           0x3e
#define EM_EGY_CONST2           0x3f
#define EM_EGY_OUT2L            0x40
#define EM_EGY_OUT2H            0x41
#define EM_EGY_CFCNT2           0x42
#define EM_EGY_CONST3           0x43
#define EM_EGY_OUT3             0x44
#define EM_EGY_CFCNT3           0x45
#define EM_EGY_CONST4           0x46
#define EM_EGY_OUT4             0x47
#define EM_EGY_CFCNT4           0x48
#define EM_EGY_CONST5           0x49
#define EM_EGY_OUT5             0x4a
#define EM_EGY_CFCNT5           0x4b
#define EM_EGY_CONST6           0x4c
#define EM_EGY_OUT6             0x4d
#define EM_EGY_CFCNT6           0x4e
#define EM_EGY_CONST7           0x4f
#define EM_EGY_OUT7             0x50
#define EM_EGY_CFCNT7           0x51
#define EM_EGY_CONST8           0x52
#define EM_EGY_OUT8             0x53
#define EM_EGY_CFCNT8           0x54
#define EM_OV_THL               0x55
#define EM_OV_THH               0x56
#define EM_SWELL_THL            0x57
#define EM_SWELL_THH            0x58
#define EM_DIP_THL              0x59
#define EM_DIP_THH              0x5a
#define EM_FD_OVTH              0x5b
#define EM_FD_LVTH              0x5c
#define EM_FD_IA_OCTH           0x5d
#define EM_FD_IA_LCTH           0x5e
#define EM_FD_IB_OCTH           0x5f
#define EM_FD_IB_LCTH           0x60
#define EM_PHS_STT              0x61
#define EM_PHS_U                0x62
#define EM_PHS_UN               0x63
#define EM_PHS_UP               0x64
#define EM_PHS_I                0x65
#define EM_PHS_IN               0x66
#define EM_PHS_IP               0x67
#define EM_DAT_WAVE             0x69
#define EM_DAT_SWELL_CNT        0x6a
#define EM_DAT_DIP_CNT          0x6b
#define EM_SYS_INTSTS           0x72
#define EM_SYS_INTEN            0x73
#define EM_SYS_STS              0x74
#define EM_SYS_MISC             0x75
#define EM_SYS_IOCFG0           0x7d
#define EM_SYS_IOCFG1           0x7e

/* EM clock source */
#define EM_PCLKDIV              0
#define EM_RCH                  1

/* EM read write clock divider */
#define EM_RWCLKDIV_2          0
#define EM_RWCLKDIV_4          1
#define EM_RWCLKDIV_8          2
#define EM_RWCLKDIV_16         3
#define EM_RWCLKDIV_32         4
#define EM_RWCLKDIV_64         5
#define EM_RWCLKDIV_128        6

/*EMINT*/
#define EMINT_MASK        0x0404
#define EMINT0            0x04
#define EMINT1            0x0400

/*Wakeup_Event*/
#define EMINT_DISABLE     (0)
#define EMINT_RISING      (1)
#define EMINT_FALLING     (2)
#define EMINT_HIGH        (3)
#define EMINT_LOW         (4)
#define EMINT_EDGEBOTH    (5)

/*********Deprecated macro defines, for compatibility purposes*****************/
#define PM_ANA_CTRL0            0x00
#define PM_ANA_CTRL1            0x01
#define DSP_CTRL0               0x02
#define DSP_CTRL1               0x03
#define DSP_CTRL2               0x04
#define DSP_CTRL3               0x05
#define DSP_CTRL4               0x06
#define DSP_CTRL5               0x07
#define DSP_DAT_PA              0x08
#define DSP_DAT_QA              0x09
#define DSP_DAT_SA              0x0a
#define DSP_DAT_PB              0x0b
#define DSP_DAT_QB              0x0c
#define DSP_DAT_SB              0x0d
#define DSP_DAT_RMS0UA          0x0e
#define DSP_DAT_RMS0IA          0x0f
#define DSP_DAT_RMS0IB          0x10
#define DSP_DAT_CH1             0x11
#define DSP_DAT_CH2             0x12
#define DSP_DAT_PA1             0x13
#define DSP_DAT_QA1             0x14
#define DSP_DAT_SA1             0x15
#define DSP_DAT_PB1             0x16
#define DSP_DAT_QB1             0x17
#define DSP_DAT_SB1             0x18
#define DSP_DAT_RMS1U           0x19
#define DSP_DAT_RMS1IA          0x1a
#define DSP_DAT_RMS1IB          0x1b
#define DSP_DAT_CH1_AVG         0x1c
#define DSP_DAT_CH2_AVG         0x1d
#define DSP_DAT_RMSU_AVG        0x1e
#define DSP_DAT_RMSIA_AVG       0x1f
#define DSP_DAT_RMSIB_AVG       0x20
#define DSP_DAT_FRQ             0x21
#define DSP_DAT_DCU             0x22
#define DSP_DAT_DCIA            0x23
#define DSP_DAT_DCIB            0x24
#define DSP_CFG_CALI_PA         0x25
#define DSP_CFG_DC_PA           0x26
#define DSP_CFG_CALI_QA         0x27
#define DSP_CFG_DC_QA           0x28
#define DSP_CFG_CALI_PB         0x29
#define DSP_CFG_DC_PB           0x2a
#define DSP_CFG_CALI_QB         0x2b
#define DSP_CFG_DC_QB           0x2c
#define DSP_CFG_CALI_RMSU       0x2d
#define DSP_CFG_RMS_DCU         0x2e
#define DSP_CFG_CALI_RMSIA      0x2f
#define DSP_CFG_RMS_DCIA        0x30
#define DSP_CFG_CALI_RMSIB      0x31
#define DSP_CFG_RMS_DCIB        0x32
#define DSP_CFG_PHC             0x33
#define DSP_CFG_DCU             0x34
#define DSP_CFG_DCIA            0x35
#define DSP_CFG_DCIB            0x36
#define DSP_CFG_BPF             0x37
#define DSP_CFG_CKSUM           0x38
#define EGY_CRPTH               0x39
#define EGY_PWRTH               0x3a
#define EGY_CONST1              0x3b
#define EGY_OUT1L               0x3c
#define EGY_OUT1H               0x3d
#define EGY_CFCNT1              0x3e
#define EGY_CONST2              0x3f
#define EGY_OUT2L               0x40
#define EGY_OUT2H               0x41
#define EGY_CFCNT2              0x42
#define EGY_CONST3              0x43
#define EGY_OUT3                0x44
#define EGY_CFCNT3              0x45
#define EGY_CONST4              0x46
#define EGY_OUT4                0x47
#define EGY_CFCNT4              0x48
#define EGY_CONST5              0x49
#define EGY_OUT5                0x4a
#define EGY_CFCNT5              0x4b
#define EGY_CONST6              0x4c
#define EGY_OUT6                0x4d
#define EGY_CFCNT6              0x4e
#define EGY_CONST7              0x4f
#define EGY_OUT7                0x50
#define EGY_CFCNT7              0x51
#define EGY_CONST8              0x52
#define EGY_OUT8                0x53
#define EGY_CFCNT8              0x54
#define DSP_OV_THL              0x55
#define DSP_OV_THH              0x56
#define DSP_SWELL_THL           0x57
#define DSP_SWELL_THH           0x58
#define DSP_DIP_THL             0x59
#define DSP_DIP_THH             0x5a
#define FD_OVTH                 0x5b
#define FD_LVTH                 0x5c
#define FD_IA_OCTH              0x5d
#define FD_IA_LCTH              0x5e
#define FD_IB_OCTH              0x5f
#define FD_IB_LCTH              0x60
#define DSP_PHS_STT             0x61
#define DSP_PHS_U               0x62
#define DSP_PHS_UN              0x63
#define DSP_PHS_UP              0x64
#define DSP_PHS_I               0x65
#define DSP_PHS_IN              0x66
#define DSP_PHS_IP              0x67
#define DAT_WAVE                0x69
#define DAT_SWELL_CNT           0x6a
#define DAT_DIP_CNT             0x6b
#define PM_SYS_INTSTS           0x72
#define PM_SYS_INTEN            0x73
#define PM_SYS_STS              0x74
#define PM_SYS_MISC             0x75
#define PM_SYS_IOCFG0           0x7d
#define PM_SYS_IOCFG1           0x7e
/*********End of deprecated macro defines, for compatibility purposes**********/

/**
  * @brief  Configures divider for EMCLK, set EMCLK to fixed 6.5536MHz. If PCLK 
  *         changes this function should be called again.
  * @param  None
  * @retval 0: Function successed.
  *         1: Set divider failed , PCLK frequecy is not expected.
  */
uint8_t EM_EMCLKConfig(void);

/**
  * @brief  Select EM clock source.
  * @param  ClockSource:
  *           EM_PCLKDIV:Select EM_PCLKDIV as EM clock source.
  *           EM_RCH:Select EM_RCH as EM clock source.
  * @retval None.
  */
void EM_CLKSel(uint32_t ClockSource);

/**
  * @brief  Enables or disables the power of EM module.
  * @param  NewState:
  *             ENABLE
  *             DISABLE
  * @retval None.
  */
void EM_PWRCmd(uint32_t NewState);


/**
  * @brief  Reset EM module.
  * @param  None
  * @retval None.
  */
void EM_Reset(void);

/**
  * @brief  Configures EM read write clock. If PCLK or EM_CTRL0.EM_MODE be 
  *         changed, this function should be called again.The Maximum value of 
  *         EM read write clock is 800Kbps.
  *         EM_CTRL0.EM_MODE = 0/1/2:
  *           The maximum clock frequency of read write class I register is 
  *           PCLK/(EM system clock/8).
  *           The maximum clock frequency of read write class II register is 
  *           PCLK/(EM system clock/16).
  *         EM_CTRL0.EM_MODE = =6/7/8/9:
  *           The maximum clock frequency of read write class I register is 
  *           PCLK/(EM system clock/4).
  *           The maximum clock frequency of read write class II register is 
  *           PCLK/(EM system clock/16).
  *         For example:
  *        ---------------------------------------------------------------------
  *        |EM_CTRL0.EM_MODE | PCLK     |ClockDiv(class I) |ClockDiv(class II) |
  *        ---------------------------------------------------------------------
  *        |                 | 26214400 |       32         |      64           |
  *        |                 | 13107200 |       16         |      32           |
  *        |      0/1/2      | 6553600  |       8          |      16           |
  *        |                 | 3276800  |       4          |      8            |
  *        |                 | 1638400  |       2          |      4            |
  *        ---------------------------------------------------------------------
  *        |                 | 26214400 |       32         |      128          |
  *        |                 | 13107200 |       16         |      64           |
  *        |      6/7        | 6553600  |       8          |      32           |
  *        |                 | 3276800  |       4          |      16           |
  *        |                 | 1638400  |       2          |      8            |
  *        ---------------------------------------------------------------------
  *        |                 | 6553600  |       32         |      128          |
  *        |                 | 3276800  |       16         |      64           |
  *        |      8          | 1638400  |       8          |      32           |
  *        |                 | 819200   |       4          |      16           |
  *        |                 | 409600   |       2          |      8            |
  *        ---------------------------------------------------------------------
  *        |                 | 3276800  |       32         |      128          |
  *        |                 | 1638400  |       16         |      64           |
  *        |      9          | 819200   |       8          |      32           |
  *        |                 | 409600   |       4          |      16           |
  *        |                 | 204800   |       2          |      8            |
  *        ---------------------------------------------------------------------
  * @param  ClockDiv: EM read write clock divider, this parameter can 
  *         be
  *           EM_RWCLKDIV_2    :PCLK/2
  *           EM_RWCLKDIV_4    :PCLK/4
  *           EM_RWCLKDIV_8    :PCLK/8
  *           EM_RWCLKDIV_16   :PCLK/16
  *           EM_RWCLKDIV_32   :PCLK/32
  *           EM_RWCLKDIV_64   :PCLK/64
  *           EM_RWCLKDIV_128  :PCLK/128
  * @retval 0: Function successed.
  *         1: Wrong ClockDiv parameter.
  */
uint8_t EM_RWCLKConfig(uint32_t ClockDiv);

/**
  * @brief  Initializes EM status. If HCLK changes this function should be 
  *         called again.
  * @param  None
  * @retval 0: Function successed.
  *         1: Write register timeout.
  */
uint8_t EM_StatusInit(void);

/**
  * @brief  Read a register of the EM module.
  * @param  Addr: specifies the address to be read.
            Data: specify the location of the read data to store.
  * @retval 0: Function successed.
            1: Read register failed.
            2: Read register timeout.
  */
uint8_t EM_ReadReg(uint8_t Addr, uint32_t *Data);

/**
  * @brief  Write a register of the EM module.
  * @param  addr: specifies the address to be written.
            data: specifies the data to be written.
  * @retval 0: Function successed.
  *         1: Write register timeout.
  */
uint8_t EM_WriteReg(uint8_t Addr,uint32_t Data);

/**
  * @brief  Get all EMINT interrupt status.
  * @param  None
  * @retval EMINT interrupt status.
  */
uint16_t EM_GetAllINTStatus(void);

/**
  * @brief  Get EMINT interrupt status.
  * @param  INTMask:
  *             EMINT0 ~ EMINT1
  * @retval 1:status set
            0:status reset
  */
uint16_t EM_GetEMINTStatus(uint16_t INTMask);

/**
  * @brief  Clear EMINT interrupt status.
  * @param  INTMask:
                This parameter can be any combination of the following values
                EMINT0 ~ EMINT1
  * @retval None
  */
void EM_ClearEMINTStatus(uint16_t INTMask);

/**
  * @brief  Wake-up EMINT sources configuration. This function only set the 
  *         wake-up event in PMU_IOAWKUEN register, PMU_CONTROL. INT_IOA_EN 
  *         must be set as well to use the EMINT interrupt.
  * @param  EMINTx:   EMINT0 ~ EMINT1
  *         Wakeup_Event:
  *                 EMINT_DISABLE
  *                 EMINT_RISING
  *                 EMINT_FALLING
  *                 EMINT_HIGH
  *                 EMINT_LOW
  *                 EMINT_EDGEBOTH
  * @retval None
  */
void EM_WakeUpINTConfig(uint32_t EMINTx, uint32_t Wakeup_Event);

/**
  * @brief  Read EMINT interrupt level.
  * @param  EMINTx:
  *             EMINT0 ~ EMINT1
  * @retval EMINT interrupt level.
  */
uint8_t EM_ReadEMINTLevel(uint16_t EMINTx);

/*********Deprecated function defines, for compatibility purposes**************/
/**
  * @brief  Initializes EM clk.
  * @param  None
  * @retval 0: Function successed.
            1: The APB clock is lower than 6553600(Hz).
  */
uint8_t PM_ClkInit(void);

/**
  * @brief  Initializes energy metering. Power up the power of the energy 
  *         metering.Turn on the clock of the energy metering.
  * @param  None
  * @retval 0: Function successed.
            1: The APB clock is lower than 6553600(Hz).
            2: The status of the energy metering module is inconsistent with 
            expectations.The status value is obtained by reading PM_SYS_STS.
  */
uint8_t PM_Init(void);

/**
  * @brief  Initializes energy metering to default state.Power down the power of
  *         the energy metering.Turn off the clock of the energy metering.
  * @param  None
  * @retval 0: Function successed.
  */
uint8_t PM_DeInit(void);

/**
  * @brief  Read a register of the energy metering.
  * @param  Addr: specifies the address to be read.
            Data: specify the location of the read data to store.
  * @retval 0: Function successed.
            1: Read failed.
  */
uint8_t PM_ReadReg(uint8_t Addr, uint32_t *Data);

/**
  * @brief  Write a register of the energy metering.
  * @param  Addr: specifies the address to be written.
            Data: specifies the data to be written.
  * @retval 0: Function successed.
  */
uint8_t PM_WriteReg(uint8_t Addr,uint32_t Data);
/*********End of deprecated function defines, for compatibility purposes*******/

#ifdef __cplusplus
}
#endif
#endif //V9381_ENABLE
#endif  /* __V94XX_ENERGYMETERING_H */

/*********************************** END OF FILE ******************************/
