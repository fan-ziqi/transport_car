#include "stm32f10x.h"
#include "delay.h"
#include "TCS230.h"

int red,green,blue;
u16 rgb_value=0;
char flag=1;
u16 red_bace,green_bace,blue_bace; //三色基准值，根据实际测量确定
u16 redm,greenm,bluem;
/**********************************************************
** 函数名:TIM2_config
** 功能描述: 定时器2配置
** 输入参数: 无
** 输出参数: 无
** 返	 回: 无
***********************************************************/
void TIM2_config(void)
{

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能TIM2时钟

	/*基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 200-1;//计数值200 
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//预分频,此值+1为分频的除数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//采样分频系数0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE); //使能更新溢出中断
	TIM_Cmd(TIM2, DISABLE);//失能TIM2
}

/**********************************************************
** 函数名:TCS230_GPIO_Config
** 功能描述: TCS230的GPIO初始化配置
** 输入参数: 无
** 输出参数: 无
** 返	 回: 无
***********************************************************/
void TCS230_GPIO_Config(void)
{
	//PB10接S2，PB11接S3
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;			
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//PA2接OUT，上拉输入
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;  //PA2  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA2 上拉输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);		//PA2 上拉

}
/**********************************************************
** 函数名:EXTI_Config
** 功能描述: 外部中断配置
** 输入参数: 无
** 输出参数: 无
** 返	 回: 无
***********************************************************/
void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);	//PA2接TCS230的中断输入

	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下降沿触发
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
/**********************************************************
** 函数名:NVIC_Config
** 功能描述: 中断分组及优先级配置
** 输入参数: 无
** 输出参数: 无
** 返	 回: 无
***********************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
												
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;	//外部中断线4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	  //
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	//定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/**********************************************************
** 函数名:RGB565
** 功能描述: 将采集的R、G、B三种分量颜色转换为RGB565格式的16位颜色值
** 输入参数: red--采集的红色颜色值
			 green--采集的绿色颜色值
			 blue--采集的蓝色颜色值
** 输出参数: 无
** 返	 回: 返回RGB565格式的16位颜色值
***********************************************************/
u16 RGB565(int red,int green,int blue)
{
	int R1,G1,B1;
	unsigned char r,g,b;
	int RGB;
	R1=(int)(255*(float)red/red_bace); //red_bace参考值1835
	G1=(int)(255*(float)green/green_bace); //green_bace参考值2198
	B1=(int)(255*(float)blue/blue_bace); //blue_bace参考值2185	
	RGB=(R1<<16)|(G1<<8)|B1;
	b = ( RGB >> (0+3) ) & 0x1f;		// 取蓝色的高5位
  	g = ( RGB >> (8+2) ) & 0x3f;		// 取绿色的高6位
  	r = ( RGB >> (16+3)) & 0x1f;		// 取红色的高5位
	return( (r<<11) + (g<<5) + (b<<0) );	
}



void BPH(void)
{
	u16 i=0;
	//下面进行光的白平衡调节，得到三色基准值作为采集其它颜色的基准
	//在测量前，先将TCS230对准作为基准的白色物体等待测量
	delay_ms(1000);	//延时，等待颜色传感器准备好
	TIM_SetCounter(TIM2,0);//将TIM2计数初始值设为0
	NVIC_EnableIRQ(EXTI2_IRQn);
	TIM_Cmd(TIM2, ENABLE);//使能TIM2
	
	do //连续采集10次取平均值
	{
		if(flag==4)//三种颜色采集完成
		{
			i++;
			if(i>=5) //取后面测得的6个数据
			{
				redm+=red;
				greenm+=green;
				bluem+=blue;
			}
			delay_ms(100);
			flag=1;//标志置1，开始下一次测量
			red=0; //颜色分量计数值清0
		 	green=0;
		 	blue=0;
			TIM_SetCounter(TIM2,0);//将TIM2计数初始值设为0
			TIM_Cmd(TIM2, ENABLE); //使能TIM2
			
		}
	}while(i<10);
	//得到白平衡调整的三色颜色分量，作为后面测量时的颜色基准值
	red_bace=redm/6; //红色基准值
	green_bace=greenm/6; //绿色基准值
	blue_bace=bluem/6;	  //蓝色基准值 
	//串口输出白平衡调整得到的三色颜色分量
	printf(" rb=%d  gb=%d  bb=%d\n",red_bace,green_bace,blue_bace);
}





/**********************************************************
** 函数名:EXTI2_IRQHandler
** 功能描述: 外部中断线2中断处理程序
** 输入参数: 无
** 输出参数: 无
** 返	 回: 无
***********************************************************/
void EXTI4_IRQHandler(void)
{	
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //如果有中断产生
	 {	 
		EXTI_ClearITPendingBit(EXTI_Line4); //清除中断标志位 	
		if(flag==1)	  //允许红色通过
		{
		   GPIOB->BRR=GPIO_Pin_10; //PB10=0,即s2=0
		   GPIOB->BRR=GPIO_Pin_11; //PB11=0,s3=0
		   red++; //增加红色分量值
		}
		else if(flag==2)//允许蓝色通过 
		{
		   GPIOB->BRR=GPIO_Pin_10; //PB10=0,s2=0
		   GPIOB->BSRR=GPIO_Pin_11;//PB11=1,s3=1
		   blue++; //增加蓝色分量值
		}
		else if(flag==3) //允许绿色通过
		{
		   GPIOB->BSRR=GPIO_Pin_10; //PB10=1,s2=1
		   GPIOB->BSRR=GPIO_Pin_11; //PB11=1,s3=1
		   green++;	//增加绿色分量值
		}
		  
	 }	
	

}
/**********************************************************
** 函数名:TIM2_IRQHandler
** 功能描述: 定时器2中断处理程序
** 输入参数: 无
** 输出参数: 无
** 返	 回: 无
***********************************************************/
void TIM2_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//
	{
//		printf("hello\n");
		/*必须清空标志位*/
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		flag++;	//定时时间到，切换允许的颜色值
		if(flag>3)//三种颜色值已采集完成，关闭定时器，计算得到颜色
		{	
			TIM_Cmd(TIM2, DISABLE);
			flag=4;
		}
	}
}

void Color(void)
{
	
	if(flag==4)	//三种颜色采集完成
		{
//			printf("r=%d  g=%d  b=%d\r\n",red,green,blue);
			rgb_value=RGB565(red,green,blue);//将采集的三色颜色值分量转换为RGB565格式的16位颜色值
//			printf("rgb_value=%d\r\n",rgb_value);
			delay_ms(100);
			flag=1;//标志置1，开始下一次测量
			red=0;	//颜色分量计数值清0
		 	green=0;
		 	blue=0;
			TIM_SetCounter(TIM2,0);//将TIM2计数初始值设为0
			TIM_Cmd(TIM2, ENABLE); //使能TIM2
		}
}
