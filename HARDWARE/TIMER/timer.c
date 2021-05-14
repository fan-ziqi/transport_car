#include "timer.h"
#include "oled_iic.h"
#include "TCS230.h"
#include "adc1.h"

extern int red,green,blue;
extern u16 rgb_value;
extern char flag;
extern u16 red_bace,green_bace,blue_bace;
extern int COLOR;
int R,G,B;
int num=0;

extern u16 adcx;
extern float vcc;

uint16_t time = 0;
uint16_t time0 = 0;
uint16_t time1 = 0;
uint16_t time2 = 0;
uint16_t time4 = 0;
uint16_t time5 = 0;
uint16_t time6 = 0;
uint16_t time7 = 0;
uint16_t time8 = 0;
uint16_t time9 = 0;
uint16_t time10 = 0;
uint16_t time11 = 0;
uint16_t time12 = 0;
uint16_t time13 = 0;
u8 flag_1ms;
//u8 flag_10ms;


extern int Battery;
extern int xssc;
extern int xhzt;
extern int STOP;
unsigned char table0[20] = ":?  ";
unsigned char table5[20] = ":       ";
	
	
//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		
	if(TIM3->SR&0X0001)//溢出中断
	{	    
			
			time++;
			time0++;
			time1++;
		  time2++;
			time4++;
			time5++;
			time6++;
			time7++;
			time8++;
			time9++;
			time10++;
			time11++;
			time12++;
			time13++;
			flag_1ms = 1;
		if(time7>=100)
		{
			time7=0;
			  if(R>200&&G>200&&B>200) COLOR=0;
				if(R<200||G<200||B<200){
				     if((R>G&&R>B)&&(R<200||G<200||B<200)) COLOR=1;
				else if((G>R&&G>B)&&(R<200||G<200||B<200)) COLOR=2;
				else if((B>R&&B>G)&&(R<200||G<200||B<200)) COLOR=3;
				}
		}
		
		if(time2>=500)
		{
			time2=0;
			xssc++;
			printf("\r\n行驶时长:%d s \r\n",xssc);
			     if(xhzt==0) printf("卸货状态:等待卸货\r\n");
			else if(xhzt==1) printf("卸货状态:红色卸货\r\n");
			else if(xhzt==2) printf("卸货状态:绿色卸货\r\n");
			printf("电池电量:%d %%\r\n",Battery);
			printf("r=%d  g=%d  b=%d\r\n",R,G,B);
//			printf("rgb_value=%d\r\n",rgb_value);
			     if(COLOR==0) printf("颜色：白色\r\n");
			else if(COLOR==1) printf("颜色：红色\r\n");
			else if(COLOR==2) printf("颜色：绿色\r\n");
			else if(COLOR==3) printf("颜色：蓝色\r\n");

		}
		
		if(time >= 100)
		{

			time = 0;
			adcx=Get_adc1_Average(ADC_Channel_5,10);  //获取adc的值
			vcc=(float)adcx*(3.3*11/4096);     				//求当前电压
//			printf("当前电压=%6.2f V \r\n",vcc);				//打印当前电压，保留小数点后两位	
			Battery=((vcc-11.3)/(12.4-11.3))*100;		if(Battery<0)Battery=0;if(Battery>100)Battery=100;   //对电压数据进行处理
			sprintf((char *)table0,":%d %%", (int)Battery);
			OLED_ShowStr(64, 0, table5, 2); 
			OLED_ShowStr(64, 0, table0, 2);  //显示对应的数值
					 if(Battery>=100 && Battery<=110) show_bmp(100);
			else if(Battery>= 90 && Battery<=100) show_bmp( 90);
			else if(Battery>= 80 && Battery<= 90) show_bmp( 80);
			else if(Battery>= 70 && Battery<= 80) show_bmp( 70);
			else if(Battery>= 60 && Battery<= 70) show_bmp( 60);
			else if(Battery>= 50 && Battery<= 60) show_bmp( 50);
			else if(Battery>= 40 && Battery<= 50) show_bmp( 40);
			else if(Battery>= 30 && Battery<= 40) show_bmp( 30);
			else if(Battery>= 20 && Battery<= 30) show_bmp( 20);
			else if(Battery>= 10 && Battery<= 20) show_bmp( 10);
			else if(Battery>=  0 && Battery<=  1) show_bmp(  0);
			
			     if(COLOR==0) OLED_ShowCN(0,4,7);
			else if(COLOR==1) OLED_ShowCN(0,4,4);		 
			else if(COLOR==2) OLED_ShowCN(0,4,5);
			else if(COLOR==3) OLED_ShowCN(0,4,6);
			
		}
		
		
		if(time4>=10)
		{
			time4 = 0;
			if(flag==4)	//三种颜色采集完成
			{
				R=(int)(255*(float)red/red_bace); //red_bace参考值1835
				G=(int)(255*(float)green/green_bace); //green_bace参考值2198
				B=(int)(255*(float)blue/blue_bace); //blue_bace参考值2185	
				
				if(COLOR==1||COLOR==2||COLOR==3) STOP=1;
				
			rgb_value=RGB565(red,green,blue);//将采集的三色颜色值分量转换为RGB565格式的16位颜色值
//			printf("r=%d  g=%d  b=%d\r\n",red,green,blue);
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
		
//			flag_10ms = 1;//这个要看定时器的重装载值和预分频值判断
		
	}	
		
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_init(u16 arr,u16 psc)
{
	*( volatile unsigned int *) 0x4002101C |= 1<<1;  
 	TIM3->ARR=arr;  	//设定计数器自动重装值 
	TIM3->PSC=psc;  	//预分频器设置
	TIM3->DIER|=1<<0;   //允许更新中断				
	TIM3->CR1|=0x01;    //使能定时器3
	MY_NVIC_Init(1,3,TIM3_IRQn,2);//抢占1，子优先级3，组2									 
}













