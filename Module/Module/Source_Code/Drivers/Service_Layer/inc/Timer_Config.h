
#ifndef TMR_Config_H_
#define TMR_Config_H_




extern volatile uint32_t timer1_tick_ms ;
extern volatile uint64_t timer2_tick_ns ;


#define S_ENABLE_TIMER_0_INT                                                    {TMR_INTConfig(TMR0, ENABLE);CORTEX_SetPriority_ClearPending_EnableIRQ(TMR0_IRQn, 0);}
#define S_DISABLE_TIMER_0_INT                                                   {TMR_INTConfig(TMR0, DISABLE);TMR_INTConfig(TMR0, DISABLE);}
#define S_ENABLE_TIMER(timer_type)                                              TMR_Cmd(timer_type, ENABLE);
#define S_DISABLE_TIMER(timer_type)                                             TMR_Cmd(timer_type, DISABLE);
#define S_ENABLE_TIMER_1_INT                                                    {TMR_INTConfig(TMR1, ENABLE);CORTEX_SetPriority_ClearPending_EnableIRQ(TMR1_IRQn, 0);}
#define S_DISABLE_TIMER_1_INT                                                   {TMR_INTConfig(TMR1, DISABLE);TMR_INTConfig(TMR1, DISABLE);}
#define S_TMR_CLKSRC_INTERNAL                                                   TMR_CLKSRC_INTERNAL
#define S_CLK_GetPCLKFreq                                                       CLK_GetPCLKFreq
#define S_TMR_Init(TIMER_NUMER,INIT_STRUCT)                                     TMR_Init(TIMER_NUMER,INIT_STRUCT)
#define S_TMR_DeInit(TIMER_NUMBER)                                              TMR_DeInit(TIMER_NUMBER)
#define S_TMR0                                                                  TMR0                     
#define S_TMR1                                                                  TMR1
#define S_TMR2                                                                  TMR2
#define S_TMR3                                                                  TMR3
#define S_TMR_EXTGT_DISABLE                                                     TMR_EXTGT_DISABLE
#define S_TMR_INTConfig(TIMER_NUMBER,State)                                     TMR_INTConfig(TIMER_NUMBER,State)
#define S_CORTEX_SetPriority_ClearPending_EnableIRQ(IRQ, Periority)             CORTEX_SetPriority_ClearPending_EnableIRQ(IRQ, Periority)
#endif /* TMR_Config_H_ */
