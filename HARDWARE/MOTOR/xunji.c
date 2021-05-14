#include "xunji.h"
#include "sys.h"
#include "motor.h"
#include "sensor.h"

extern int STOP;
extern int COLOR;
extern int xhzt;
int state=0;

//读取灰度值
void read_sensor_values(void)
{

	     if(Hui1==BLACK&&Hui2==BLACK&&Hui3==BLACK && Hui6==WHITE&&Hui7==WHITE&&Hui8==WHITE) state=-3;
	else if(Hui1==BLACK&&Hui2==BLACK&&Hui3==WHITE && Hui6==WHITE&&Hui7==WHITE&&Hui8==WHITE) state=-3;
	else if(Hui1==BLACK&&Hui2==WHITE&&Hui3==WHITE && Hui6==WHITE&&Hui7==WHITE&&Hui8==WHITE) state=-3;
	else if(Hui1==WHITE&&Hui2==BLACK&&Hui3==WHITE && Hui6==WHITE&&Hui7==WHITE&&Hui8==WHITE) state=-2;
	else if(Hui1==WHITE&&Hui2==WHITE&&Hui3==BLACK && Hui6==WHITE&&Hui7==WHITE&&Hui8==WHITE) state=-1;
  else if(Hui1==WHITE&&Hui2==WHITE&&Hui3==WHITE && Hui6==WHITE&&Hui7==WHITE&&Hui8==WHITE && Hui4==BLACK&&Hui5==BLACK) state=0;
	else if(Hui1==WHITE&&Hui2==WHITE&&Hui3==WHITE && Hui6==WHITE&&Hui7==WHITE&&Hui8==WHITE && Hui4==WHITE&&Hui5==BLACK) state=0;
	else if(Hui1==WHITE&&Hui2==WHITE&&Hui3==WHITE && Hui6==WHITE&&Hui7==WHITE&&Hui8==WHITE && Hui4==BLACK&&Hui5==WHITE) state=0;
	else if(Hui1==WHITE&&Hui2==WHITE&&Hui3==WHITE && Hui6==BLACK&&Hui7==WHITE&&Hui8==WHITE) state= 1;
	else if(Hui1==WHITE&&Hui2==WHITE&&Hui3==WHITE && Hui6==WHITE&&Hui7==BLACK&&Hui8==WHITE) state= 2;
	else if(Hui1==WHITE&&Hui2==WHITE&&Hui3==WHITE && Hui6==WHITE&&Hui7==WHITE&&Hui8==BLACK) state= 3;
	else if(Hui1==WHITE&&Hui2==WHITE&&Hui3==WHITE && Hui6==WHITE&&Hui7==BLACK&&Hui8==BLACK) state= 3;
	else if(Hui1==WHITE&&Hui2==WHITE&&Hui3==WHITE && Hui6==BLACK&&Hui7==BLACK&&Hui8==BLACK) state= 3;
		
	else if(Hui1==WHITE&&Hui2==WHITE&&Hui3==WHITE && Hui6==WHITE&&Hui7==WHITE&&Hui8==WHITE)
	{
		if(state==-3) state=-4;
		if(state== 3) state= 4;
//		else state=0;
	}

//	else state=0;
//	printf("%d",state);
}


void motor_control(void)
{
	if(STOP==0)
	{
				 if(state== 4) Move(-10,-10);
		else if(state== 3) Move( 50,-30);
		else if(state== 2) Move( 40,-20);
		else if(state== 1) Move( 25,-10);
		else if(state== 0) Move( 20, 20);
		else if(state==-1) Move(-10, 25);
		else if(state==-2) Move(-20, 40);
		else if(state==-3) Move(-30, 50);
		else if(state==-4) Move(-10,-10);
//		else Move(20,20);
	}
	else if(STOP==1)
	{
		//电磁铁任务
		Move(0,0);
		     if(COLOR==1) 
		{
			xhzt=1;
					 delay_ms(1000);
//					 Move(20,20);
//					 for(int i=0;i<=2;i++) {delay_ms(700);}
//					 Move(0,0);
					 GPIO_ResetBits(GPIOA,GPIO_Pin_11);
					 delay_ms(1000);
					 Move(20,20);
					 for(int i=0;i<=2;i++) {delay_ms(1000);}
					 Move(0,0);
					 STOP=0;
					 xhzt=0;
		}
		else if(COLOR==2) 
		{
			xhzt=1;
					 delay_ms(1000);
//			     Move(20,20);
//					 for(int i=0;i<=2;i++) {delay_ms(700);}
//					 Move(0,0);
					 GPIO_ResetBits(GPIOA,GPIO_Pin_12);
					 Move(20,20);
					 delay_ms(1000);
					 Move(0,0);
					 STOP=0;
			xhzt=0;
		}
		else if(COLOR==3) while(1);
	}


}

void xunji(void)
{
	read_sensor_values();
//	calc_pid();
	motor_control();
}

