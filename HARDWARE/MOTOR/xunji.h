#ifndef __XUNJI_H
#define __XUNJI_H
#include "sys.h"

#define speed_lim1 150
#define speed_lim2 -150



void read_sensor_values(void);//读取灰度值
void calc_pid(void); //计算PID
void motorWrite(float speedL, float speedR); //正反转向 
void motor_control(void);//限幅
void xunji(void);

#endif
