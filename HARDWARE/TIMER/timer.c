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
	
	
//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		
	if(TIM3->SR&0X0001)//����ж�
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
			printf("\r\n��ʻʱ��:%d s \r\n",xssc);
			     if(xhzt==0) printf("ж��״̬:�ȴ�ж��\r\n");
			else if(xhzt==1) printf("ж��״̬:��ɫж��\r\n");
			else if(xhzt==2) printf("ж��״̬:��ɫж��\r\n");
			printf("��ص���:%d %%\r\n",Battery);
			printf("r=%d  g=%d  b=%d\r\n",R,G,B);
//			printf("rgb_value=%d\r\n",rgb_value);
			     if(COLOR==0) printf("��ɫ����ɫ\r\n");
			else if(COLOR==1) printf("��ɫ����ɫ\r\n");
			else if(COLOR==2) printf("��ɫ����ɫ\r\n");
			else if(COLOR==3) printf("��ɫ����ɫ\r\n");

		}
		
		if(time >= 100)
		{

			time = 0;
			adcx=Get_adc1_Average(ADC_Channel_5,10);  //��ȡadc��ֵ
			vcc=(float)adcx*(3.3*11/4096);     				//��ǰ��ѹ
//			printf("��ǰ��ѹ=%6.2f V \r\n",vcc);				//��ӡ��ǰ��ѹ������С�������λ	
			Battery=((vcc-11.3)/(12.4-11.3))*100;		if(Battery<0)Battery=0;if(Battery>100)Battery=100;   //�Ե�ѹ���ݽ��д���
			sprintf((char *)table0,":%d %%", (int)Battery);
			OLED_ShowStr(64, 0, table5, 2); 
			OLED_ShowStr(64, 0, table0, 2);  //��ʾ��Ӧ����ֵ
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
			if(flag==4)	//������ɫ�ɼ����
			{
				R=(int)(255*(float)red/red_bace); //red_bace�ο�ֵ1835
				G=(int)(255*(float)green/green_bace); //green_bace�ο�ֵ2198
				B=(int)(255*(float)blue/blue_bace); //blue_bace�ο�ֵ2185	
				
				if(COLOR==1||COLOR==2||COLOR==3) STOP=1;
				
			rgb_value=RGB565(red,green,blue);//���ɼ�����ɫ��ɫֵ����ת��ΪRGB565��ʽ��16λ��ɫֵ
//			printf("r=%d  g=%d  b=%d\r\n",red,green,blue);
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
		
//			flag_10ms = 1;//���Ҫ����ʱ������װ��ֵ��Ԥ��Ƶֵ�ж�
		
	}	
		
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_init(u16 arr,u16 psc)
{
	*( volatile unsigned int *) 0x4002101C |= 1<<1;  
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM3->PSC=psc;  	//Ԥ��Ƶ������
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
	MY_NVIC_Init(1,3,TIM3_IRQn,2);//��ռ1�������ȼ�3����2									 
}













