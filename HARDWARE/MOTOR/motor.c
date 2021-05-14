#include "stm32f10x.h"
#include "motor.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "sensor.h"

//电机全部初始化设置，调用一个即可
void Motor_all_init(u16 arr,u16 psc)
{
	Motor_PWM_Init(arr,psc);
	Motor_Dir_init();
}
//电机方向初始化
void Motor_Dir_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //
  GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //输出低
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);		
}


//电机输出PWM
void Motor_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	 
 
  //设置该引脚为复用输出功能
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
  //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC1
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC2
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM1_CH1上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM1_CH2上的预装载寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIM1
	TIM_ARRPreloadConfig(TIM4, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_SetCompare3(TIM4,0);
	TIM_SetCompare4(TIM4,0);
}


void Move(int v_l,int v_r)
{
	if(v_l>0&&v_r>0)
	{
	  m1_1(0);
	  m1_2(1);
	  m2_1(1);
	  m2_2(0);
		v_l=v_l;
		v_r=v_r;
	}
	if(v_l<0&&v_r<0)
	{
	  m1_1(1);
	  m1_2(0);
	  m2_1(0);
	  m2_2(1);
		v_l=v_l*(-1);
		v_r=v_r*(-1);
	}
	if(v_l>0&&v_r<0)
	{
	  m1_1(0);
	  m1_2(1);
	  m2_1(0);
	  m2_2(1);
		v_l=v_l;
		v_r=v_r*(-1);
	}
	if(v_l<0&&v_r>0)
	{
	  m1_1(1);
	  m1_2(0);
	  m2_1(1);
	  m2_2(0);
		v_l=v_l*(-1);
		v_r=v_r;
	}
	TIM_SetCompare3(TIM4,v_l);
	TIM_SetCompare4(TIM4,v_r);
}

void Stop(void)
{
	m1_1(0);
	m1_2(0);
	m2_1(0);
	m2_2(0);
	TIM_SetCompare3(TIM4,0);
	TIM_SetCompare4(TIM4,0);
}



