#ifndef __XUNJI_H
#define __XUNJI_H
#include "sys.h"

#define speed_lim1 150
#define speed_lim2 -150



void read_sensor_values(void);//��ȡ�Ҷ�ֵ
void calc_pid(void); //����PID
void motorWrite(float speedL, float speedR); //����ת�� 
void motor_control(void);//�޷�
void xunji(void);

#endif
