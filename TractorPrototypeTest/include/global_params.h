#ifndef GLOBAL_PARAMS_H_
#define GLOBAL_PARAMS_H_
#include "sys.h"
#include "navigation2.h"

//#define BEIDOU_GPS
#define NUOGENG_GPS


#define MAX_PATH_VERTEX_NUM 10

#define DMA_DATA_NUM 150  //大于实际包长  

#define DRIVINGMODE_DRIVERLESS 1
#define DRIVINGMODE_PAUSE_DRIVERLESS 2
#define DRIVINGMODE_STOP_DRIVELESS 3
#define DRIVINGMODE_MANUAL 3


extern u8 g_gps_data_buf[DMA_DATA_NUM];

extern gps_sphere_t g_gps_sphere_now;

extern char g_mode_name[20];

extern int g_u32_lon,g_u32_lat,g_int_height;
extern s16 g_s16_speed;
extern uint16_t g_u16_yaw;

extern u8 g_u8_gps_status,g_u8_gps_satellites;   //use to can send msg 
extern s16  g_s16_steer_angle;  //两位小数

extern u8 g_actual_path_vertwx_num;

extern u8 g_recordTargetSeq;

extern gps_sphere_t target_point[MAX_PATH_VERTEX_NUM];  

extern u8 g_drivingMode;

extern int16_t g_AngleSensorAdValueOffset; //方向盘中位时，AD值

extern uint16_t g_AngleSensorMaxAngle ; //一般为整数！
extern uint16_t g_AngleSensorMaxAdValue;
extern int8_t g_roadWheelAngle_dir; //1左转为正，右转为负，-1 左转为负！

extern float g_vehicleSpeed;

extern float g_MinDisBetweenTwoTarget ;
extern float g_debugRoadWheelAngle;
extern uint8_t g_debugEnable;
#endif

