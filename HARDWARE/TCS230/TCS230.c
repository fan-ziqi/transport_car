#include "stm32f10x.h"
#include "delay.h"
#include "TCS230.h"

int red,green,blue;
u16 rgb_value=0;
char flag=1;
u16 red_bace,green_bace,blue_bace; //��ɫ��׼ֵ������ʵ�ʲ���ȷ��
u16 redm,greenm,bluem;
/**********************************************************
** ������:TIM2_config
** ��������: ��ʱ��2����
** �������: ��
** �������: ��
** ��	 ��: ��
***********************************************************/
void TIM2_config(void)
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʹ��TIM2ʱ��

	/*��������*/
	TIM_TimeBaseStructure.TIM_Period = 200-1;//����ֵ200 
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//������Ƶϵ��0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE); //ʹ�ܸ�������ж�
	TIM_Cmd(TIM2, DISABLE);//ʧ��TIM2
}

/**********************************************************
** ������:TCS230_GPIO_Config
** ��������: TCS230��GPIO��ʼ������
** �������: ��
** �������: ��
** ��	 ��: ��
***********************************************************/
void TCS230_GPIO_Config(void)
{
	//PB10��S2��PB11��S3
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//PA2��OUT����������
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;  //PA2  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA2 ��������  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);		//PA2 ����

}
/**********************************************************
** ������:EXTI_Config
** ��������: �ⲿ�ж�����
** �������: ��
** �������: ��
** ��	 ��: ��
***********************************************************/
void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);	//PA2��TCS230���ж�����

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//�½��ش���
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
/**********************************************************
** ������:NVIC_Config
** ��������: �жϷ��鼰���ȼ�����
** �������: ��
** �������: ��
** ��	 ��: ��
***********************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
												
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;	//�ⲿ�ж���4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	  //
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	//��ʱ��2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/**********************************************************
** ������:RGB565
** ��������: ���ɼ���R��G��B���ַ�����ɫת��ΪRGB565��ʽ��16λ��ɫֵ
** �������: red--�ɼ��ĺ�ɫ��ɫֵ
			 green--�ɼ�����ɫ��ɫֵ
			 blue--�ɼ�����ɫ��ɫֵ
** �������: ��
** ��	 ��: ����RGB565��ʽ��16λ��ɫֵ
***********************************************************/
u16 RGB565(int red,int green,int blue)
{
	int R1,G1,B1;
	unsigned char r,g,b;
	int RGB;
	R1=(int)(255*(float)red/red_bace); //red_bace�ο�ֵ1835
	G1=(int)(255*(float)green/green_bace); //green_bace�ο�ֵ2198
	B1=(int)(255*(float)blue/blue_bace); //blue_bace�ο�ֵ2185	
	RGB=(R1<<16)|(G1<<8)|B1;
	b = ( RGB >> (0+3) ) & 0x1f;		// ȡ��ɫ�ĸ�5λ
  	g = ( RGB >> (8+2) ) & 0x3f;		// ȡ��ɫ�ĸ�6λ
  	r = ( RGB >> (16+3)) & 0x1f;		// ȡ��ɫ�ĸ�5λ
	return( (r<<11) + (g<<5) + (b<<0) );	
}



void BPH(void)
{
	u16 i=0;
	//������й�İ�ƽ����ڣ��õ���ɫ��׼ֵ��Ϊ�ɼ�������ɫ�Ļ�׼
	//�ڲ���ǰ���Ƚ�TCS230��׼��Ϊ��׼�İ�ɫ����ȴ�����
	delay_ms(1000);	//��ʱ���ȴ���ɫ������׼����
	TIM_SetCounter(TIM2,0);//��TIM2������ʼֵ��Ϊ0
	NVIC_EnableIRQ(EXTI2_IRQn);
	TIM_Cmd(TIM2, ENABLE);//ʹ��TIM2
	
	do //�����ɼ�10��ȡƽ��ֵ
	{
		if(flag==4)//������ɫ�ɼ����
		{
			i++;
			if(i>=5) //ȡ�����õ�6������
			{
				redm+=red;
				greenm+=green;
				bluem+=blue;
			}
			delay_ms(100);
			flag=1;//��־��1����ʼ��һ�β���
			red=0; //��ɫ��������ֵ��0
		 	green=0;
		 	blue=0;
			TIM_SetCounter(TIM2,0);//��TIM2������ʼֵ��Ϊ0
			TIM_Cmd(TIM2, ENABLE); //ʹ��TIM2
			
		}
	}while(i<10);
	//�õ���ƽ���������ɫ��ɫ��������Ϊ�������ʱ����ɫ��׼ֵ
	red_bace=redm/6; //��ɫ��׼ֵ
	green_bace=greenm/6; //��ɫ��׼ֵ
	blue_bace=bluem/6;	  //��ɫ��׼ֵ 
	//���������ƽ������õ�����ɫ��ɫ����
	printf(" rb=%d  gb=%d  bb=%d\n",red_bace,green_bace,blue_bace);
}





/**********************************************************
** ������:EXTI2_IRQHandler
** ��������: �ⲿ�ж���2�жϴ������
** �������: ��
** �������: ��
** ��	 ��: ��
***********************************************************/
void EXTI4_IRQHandler(void)
{	
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //������жϲ���
	 {	 
		EXTI_ClearITPendingBit(EXTI_Line4); //����жϱ�־λ 	
		if(flag==1)	  //�����ɫͨ��
		{
		   GPIOB->BRR=GPIO_Pin_10; //PB10=0,��s2=0
		   GPIOB->BRR=GPIO_Pin_11; //PB11=0,s3=0
		   red++; //���Ӻ�ɫ����ֵ
		}
		else if(flag==2)//������ɫͨ�� 
		{
		   GPIOB->BRR=GPIO_Pin_10; //PB10=0,s2=0
		   GPIOB->BSRR=GPIO_Pin_11;//PB11=1,s3=1
		   blue++; //������ɫ����ֵ
		}
		else if(flag==3) //������ɫͨ��
		{
		   GPIOB->BSRR=GPIO_Pin_10; //PB10=1,s2=1
		   GPIOB->BSRR=GPIO_Pin_11; //PB11=1,s3=1
		   green++;	//������ɫ����ֵ
		}
		  
	 }	
	

}
/**********************************************************
** ������:TIM2_IRQHandler
** ��������: ��ʱ��2�жϴ������
** �������: ��
** �������: ��
** ��	 ��: ��
***********************************************************/
void TIM2_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//
	{
//		printf("hello\n");
		/*������ձ�־λ*/
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		flag++;	//��ʱʱ�䵽���л��������ɫֵ
		if(flag>3)//������ɫֵ�Ѳɼ���ɣ��رն�ʱ��������õ���ɫ
		{	
			TIM_Cmd(TIM2, DISABLE);
			flag=4;
		}
	}
}

void Color(void)
{
	
	if(flag==4)	//������ɫ�ɼ����
		{
//			printf("r=%d  g=%d  b=%d\r\n",red,green,blue);
			rgb_value=RGB565(red,green,blue);//���ɼ�����ɫ��ɫֵ����ת��ΪRGB565��ʽ��16λ��ɫֵ
//			printf("rgb_value=%d\r\n",rgb_value);
			delay_ms(100);
			flag=1;//��־��1����ʼ��һ�β���
			red=0;	//��ɫ��������ֵ��0
		 	green=0;
		 	blue=0;
			TIM_SetCounter(TIM2,0);//��TIM2������ʼֵ��Ϊ0
			TIM_Cmd(TIM2, ENABLE); //ʹ��TIM2
		}
}
