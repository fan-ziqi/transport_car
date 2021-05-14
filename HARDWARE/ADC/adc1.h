#ifndef __ADC1_H
#define	__ADC1_H

#include "stm32f10x.h"

void adc1_Init(void);
u16  Get_adc1(u8 ch); 
u16 Get_adc1_Average(u8 ch,u8 times);

#endif

