#include "global_params.h"



char gps_data_buf[DMA_DATA_NUM];


gps_sphere_t gps_sphere_now;

char mode_name[20]= "Record Point";

int send_lon,send_lat,send_height;
u16 send_speed,send_yaw;
u8 send_gps_status,send_satellites;   //use to can send msg 
u16  send_steer_angle;   //原始角度扩大90倍  15字节为方向，0左，1右


u8 actual_path_vertwx_num; //路径顶点个数

u8 start_driverless_flag = 0;

gps_sphere_t target_point[MAX_PATH_VERTEX_NUM]={0.0,0.0,0.0};  

int16_t g_AngleSensorAdValueOffset = 0;  //方向盘中位时的前轮转角 AD值

uint16_t g_AngleSensorMaxAdValue = 4096; //前轮转角传感器最大AD值 

uint16_t g_AngleSensorMaxAngle = 360; //一般为整数！

int8_t g_roadWheelAngle_dir = 1; //1左转为正，右转为负，-1 左转为负！

float g_vehicleSpeed = 0.0;
 
