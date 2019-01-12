#ifndef GLOBAL_PARAMS_H_
#define GLOBAL_PARAMS_H_
#include "sys.h"
#include "navigation2.h"

#define MAX_PATH_VERTEX_NUM 10

#define DMA_DATA_NUM 105  //����ʵ�ʰ���  

extern char g_gps_data_buf[DMA_DATA_NUM];

extern gps_sphere_t g_gps_sphere_now;

extern char g_mode_name[20];

extern int g_int_lon,g_int_lat,g_int_height;
extern s16 g_s16_speed;
extern uint16_t g_u16_yaw;

extern u8 g_gps_status,g_gps_satellites;   //use to can send msg 
extern s16  g_s16_steer_angle;  //��λС��

extern u8 g_actual_path_vertwx_num;

extern u8 g_recordTargetSeq;

extern gps_sphere_t target_point[MAX_PATH_VERTEX_NUM];  

extern u8 g_start_driverless_flag;

extern int16_t g_AngleSensorAdValueOffset; //��������λʱ��ADֵ

extern uint16_t g_AngleSensorMaxAngle ; //һ��Ϊ������
extern uint16_t g_AngleSensorMaxAdValue;
extern int8_t g_roadWheelAngle_dir; //1��תΪ������תΪ����-1 ��תΪ����

extern float g_vehicleSpeed;
#endif

