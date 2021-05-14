#include "stm32f10x.h"
#include "motor.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "sensor.h"

//���ȫ����ʼ�����ã�����һ������
void Motor_all_init(u16 arr,u16 psc)
{
	Motor_PWM_Init(arr,psc);
	Motor_Dir_init();
}
//��������ʼ��
void Motor_Dir_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //
  GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //�����
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);		
}


//������PWM
void Motor_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	 
 
  //���ø�����Ϊ�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
  //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC2
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM1_CH1�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM1_CH2�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM1
	TIM_ARRPreloadConfig(TIM4, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
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



