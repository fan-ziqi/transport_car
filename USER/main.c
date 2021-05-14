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

//电压检测
u16 adcx;
float vcc;

//电磁铁初始化
void DCT_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //
  GPIO_SetBits(GPIOA,GPIO_Pin_11);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
}

int main(void)
{	
	//电压检测

  SystemInit();				//配置系统时钟为72M   
	adc1_Init();				//ADC1的初始化  
	
	DCT_init(); //电磁铁
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	delay_init();
	
	//颜色识别初始化
	TCS230_GPIO_Config();//TCS230的GPIO配置
	EXTI_Config(); //外部中断配置
  NVIC_Config(); //中断向量配置
 	TIM2_config(); //TIM2初始化
	BPH();
	
	OLED_Init();
	TIM3_init(1000-1,72-1);//time
	OLED_CLS();//清屏
	
//	//显示封面图片
//	show_bmp(1);
//	for(int i=0;i<=5;i++)
//	{
//		delay_ms(500);
//	}
//	OLED_CLS();//清屏
	
	 
	Sensor_all_init();//传感器初始
	Motor_all_init(99,71);//电机初始化

//	for(int i=0;i<=2;i++) {delay_ms(1000);}
	
	OLED_ShowCN(0 , 0, 0);		     //电
	OLED_ShowCN(16, 0, 1);			 //池
	OLED_ShowCN(32, 0, 2);		     //电
	OLED_ShowCN(48, 0, 3);			 //量
	
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
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 


