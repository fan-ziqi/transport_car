#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

void Motor_all_init(u16 arr,u16 psc);//电机全部初始化 调用一个即可
void Motor_PWM_Init(u16 arr,u16 psc);
void Motor_Dir_init(void);

void Move(int v_l,int v_r);
void Stop(void);

#define PA0(a) if (a) \
     GPIO_SetBits(GPIOA,GPIO_Pin_0);\
     else  \
     GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define PA1(a) if (a) \
     GPIO_SetBits(GPIOA,GPIO_Pin_1);\
     else  \
     GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define PA2(a) if (a) \
     GPIO_SetBits(GPIOA,GPIO_Pin_2);\
     else  \
     GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define PA3(a) if (a) \
     GPIO_SetBits(GPIOA,GPIO_Pin_3);\
     else  \
     GPIO_ResetBits(GPIOA,GPIO_Pin_3)

		 
#define m1_1(a) PA0(a)
#define m1_2(a) PA1(a)
#define m2_1(a) PA2(a)
#define m2_2(a) PA3(a)
		 


#endif
