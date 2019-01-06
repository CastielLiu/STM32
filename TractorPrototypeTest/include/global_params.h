#ifndef GLOBAL_PARAMS_H_
#define GLOBAL_PARAMS_H_
#include "sys.h"
#include "navigation2.h"

#define MAX_PATH_VERTEX_NUM 10

#define DMA_DATA_NUM 105

extern char gps_data_buf[105];

extern gps_sphere_t gps_sphere_now;

extern char mode_name[20];

extern int send_lon,send_lat,send_height;
extern u16 send_speed,send_yaw;
extern u8 send_gps_status,send_satellites;   //use to can send msg 
extern u16  send_steer_angle;   //ԭʼ�Ƕ�����90��  15λΪ����0��1��

extern u8 actual_path_vertwx_num;

extern gps_sphere_t target_point[MAX_PATH_VERTEX_NUM];  

extern u8 start_driverless_flag;

extern int16_t g_AngleSensorAdValueOffset; //��������λʱ��ADֵ

extern uint16_t g_AngleSensorMaxAngle ; //һ��Ϊ������
extern uint16_t g_AngleSensorMaxAdValue;
extern int8_t g_roadWheelAngle_dir; //1��תΪ������תΪ����-1 ��תΪ����

extern float g_vehicleSpeed;
#endif

