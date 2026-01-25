
#ifndef RTC_Config_H_
#define RTC_Config_H_


#define CAL_MIN_VAL     (0x3FB0)
#define CAL_MEAN_VAL    (0x3FDE)//(0x3FBE)
#define CAL_MAX_VAL     (0x3FD9)


#define RTCLIBEXT


// 0:�?����1�?AVCCLV?�?��??? 2�?RCH?��???�??? 3�?S_ANA_ADCCTRL_MTRIG ADC  ??��?��?���???�??? 4:nvr�???�??? 5: PLLL�??? 6: RTC?��??? 7: ?�??�???
#define    succ 0
#define    avccerr 1
#define    rcherr 2
#define    adcerr 3
#define    nvrerr 4
#define    plllerr 5
#define    rtcerr 6


#define S_WDTCLR_KEY                   0x55AAAA55

#define S_RTCPWD_KEY                     0x5AA55AA5
#define S_RTCCE_SETKEY                   0xA55AA55B
#define S_RTCCE_CLRKEY                   0xA55AA55A

#define S_NVR_REALTEMP1                 (__IO uint32_t *)(0x40D70)
#define S_NVR_MEATEMP1                  (__IO uint32_t *)(0x40D74)
#define S_NVR_TEMP_CHECKSUM1            (__IO uint32_t *)(0x40D78)
#define S_NVR_REALTEMP2                 (__IO uint32_t *)(0x40D7C)
#define S_NVR_MEATEMP2                  (__IO uint32_t *)(0x40D80)
#define S_NVR_TEMP_CHECKSUM2            (__IO uint32_t *)(0x40D84)

#define S_NVR_RTC1_P1_P0                (__IO uint32_t *)(0x40D00)
#define S_NVR_RTC1_P2                   (__IO uint32_t *)(0x40D04)
#define S_NVR_RTC1_P5_P4                (__IO uint32_t *)(0x40D08)
#define S_NVR_RTC1_P7_P6                (__IO uint32_t *)(0x40D0C)
#define S_NVR_RTC1_PCHECHSUM            (__IO uint32_t *)(0x40D10)
#define S_NVR_RTC2_P1_P0                (__IO uint32_t *)(0x40D14)
#define S_NVR_RTC2_P2                   (__IO uint32_t *)(0x40D18)
#define S_NVR_RTC2_P5_P4                (__IO uint32_t *)(0x40D1C)
#define S_NVR_RTC2_P7_P6                (__IO uint32_t *)(0x40D20)
#define S_NVR_RTC2_PCHECHSUM            (__IO uint32_t *)(0x40D24)


#define S_RTC_CE_BSY                  RTC_CE_BSY
#define S_ANA_REG3_RCHPD              ANA_REG3_RCHPD


#define S_ANA_ADCCTRL_CLKDIV_1         ANA_ADCCTRL_CLKDIV_1
#define S_ANA_ADCCTRL_CICSKIP_3        ANA_ADCCTRL_CICSKIP_3
#define S_ANA_ADCCTRL_DSRSEL_64        ANA_ADCCTRL_DSRSEL_64
#define S_ANA_ADCCTRL_CLKSEL           ANA_ADCCTRL_CLKSEL
#define S_ANA_COMPOUT_AVCCLV           ANA_COMPOUT_AVCCLV
#define S_ANA_ADCCTRL_MTRIG            ANA_ADCCTRL_MTRIG
#define S_ANA_REG9_PLLLSEL             ANA_REG9_PLLLSEL
#define S_ANA_COMPOUT_LOCKL            ANA_COMPOUT_LOCKL
#define S_ANA_REG3_ADCPDN              ANA_REG3_ADCPDN
#define S_ANA_REG3_PLLLPDN             ANA_REG3_PLLLPDN 
#define S_ANA_ADCCTRL_DSRSEL_512       ANA_ADCCTRL_DSRSEL_512

#define S_RTC_DISABLE_INT()    {RTC_INTConfig(RTC_INT_WKUSEC, DISABLE);CORTEX_NVIC_DisableIRQ(RTC_IRQn);}
#define S_RTC_ENABLE_INT()     {RTC_INTConfig(RTC_INT_WKUSEC, ENABLE);CORTEX_NVIC_EnableIRQ(RTC_IRQn);}



#endif /* RTC_Config_H_ */
