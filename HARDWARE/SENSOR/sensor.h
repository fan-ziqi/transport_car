#ifndef __SENSOR_H
#define	__SENSOR_H   

/*-------------单路灰度----------------*/

#define Hui1 PBin(1)
#define Hui2 PBin(3)
#define Hui3 PBin(4)
#define Hui4 PBin(5)
#define Hui5 PBin(12)
#define Hui6 PBin(13)
#define Hui7 PBin(14)
#define Hui8 PBin(15)


//传感器初始化
void Hui_Init(void);
void Sensor_all_init(void);

#define BLACK 0
#define WHITE 1

#endif 
