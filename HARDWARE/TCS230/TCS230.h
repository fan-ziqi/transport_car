#ifndef __TCS230_H
#define __TCS230_H	
#include "sys.h"
void TIM2_config(void);
void TCS230_GPIO_Config(void);
void EXTI_Config(void);
void NVIC_Config(void);
u16 RGB565(int red,int green,int blue);
void BPH(void);
void Color(void);

#endif
