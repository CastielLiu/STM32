#include "global_params.h"



u8 g_gps_data_buf[DMA_DATA_NUM];


gps_sphere_t g_gps_sphere_now;

char g_mode_name[20]= "Record Point";

int g_int_lon,g_int_lat,g_int_height;

s16 g_s16_speed;
uint16_t g_u16_yaw;

u8 g_gps_status,g_gps_satellites;   //use to can send msg 
s16  g_s16_steer_angle;   

u8 g_actual_path_vertwx_num=0; //路径顶点个数
u8 g_recordTargetSeq = 0;//务必初始化为0

u8 g_start_driverless_flag = 0;

gps_sphere_t target_point[MAX_PATH_VERTEX_NUM]={0.0,0.0,0.0};  

int16_t g_AngleSensorAdValueOffset = 0;  //方向盘中位时的前轮转角 AD值

uint16_t g_AngleSensorMaxAdValue = 4096; //前轮转角传感器最大AD值 

uint16_t g_AngleSensorMaxAngle = 360; //一般为整数！

int8_t g_roadWheelAngle_dir = 1; //1左转为正，右转为负，-1 左转为负！

float g_vehicleSpeed = 0.0;

float g_MinDisBetweenTwoTarget = 1.5;//m

float g_debugRoadWheelAngle=0.0;

uint8_t g_debugEnable = 0;
 
