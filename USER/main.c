#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "sensor.h"
#include "motor.h"
#include "xunji.h"
#include "oled_iic.h"
#include "timer.h"
#include "TCS230.h"
#include "adc1.h"


int Battery=100;
int xssc=0;
int xhzt=0;
int COLOR=0;
int STOP=0;

//��ѹ���
u16 adcx;
float vcc;

//�������ʼ��
void DCT_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //
  GPIO_SetBits(GPIOA,GPIO_Pin_11);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
}

int main(void)
{	
	//��ѹ���

  SystemInit();				//����ϵͳʱ��Ϊ72M   
	adc1_Init();				//ADC1�ĳ�ʼ��  
	
	DCT_init(); //�����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	delay_init();
	
	//��ɫʶ���ʼ��
	TCS230_GPIO_Config();//TCS230��GPIO����
	EXTI_Config(); //�ⲿ�ж�����
  NVIC_Config(); //�ж���������
 	TIM2_config(); //TIM2��ʼ��
	BPH();
	
	OLED_Init();
	TIM3_init(1000-1,72-1);//time
	OLED_CLS();//����
	
//	//��ʾ����ͼƬ
//	show_bmp(1);
//	for(int i=0;i<=5;i++)
//	{
//		delay_ms(500);
//	}
//	OLED_CLS();//����
	
	 
	Sensor_all_init();//��������ʼ
	Motor_all_init(99,71);//�����ʼ��

//	for(int i=0;i<=2;i++) {delay_ms(1000);}
	
	OLED_ShowCN(0 , 0, 0);		     //��
	OLED_ShowCN(16, 0, 1);			 //��
	OLED_ShowCN(32, 0, 2);		     //��
	OLED_ShowCN(48, 0, 3);			 //��
	
	for(int i=0;i<=5;i++) {delay_ms(1000);}
	while(1){

		 
		xunji();
		
//		for(int i=0;i<=2;i++) {delay_ms(1000);}
//		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
//		for(int i=0;i<=2;i++) {delay_ms(1000);}
//		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
//		while(1);
		

		 
	 }
 }
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 


