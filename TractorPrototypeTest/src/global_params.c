#include "global_params.h"



u8 g_gps_data_buf[DMA_DATA_NUM];


gps_sphere_t g_gps_sphere_now={0.0,0.0,0.0};

char g_mode_name[20]= "Record Point";

int g_u32_lon,g_u32_lat,g_int_height;

s16 g_s16_speed;
uint16_t g_u16_yaw;

u8 g_u8_gps_status,g_u8_gps_satellites;   //use to can send msg 
s16  g_s16_steer_angle;   

u8 g_actual_path_vertwx_num=0; //·���������
u8 g_recordTargetSeq = 0;//��س�ʼ��Ϊ0

u8 g_drivingMode = DRIVINGMODE_MANUAL;

gps_sphere_t target_point[MAX_PATH_VERTEX_NUM]={0.0,0.0,0.0};  

int16_t g_AngleSensorAdValueOffset = 0;  //��������λʱ��ǰ��ת�� ADֵ

uint16_t g_AngleSensorMaxAdValue = 4096; //ǰ��ת�Ǵ��������ADֵ 

uint16_t g_AngleSensorMaxAngle = 360; //һ��Ϊ������

int8_t g_roadWheelAngle_dir = 1; //1��תΪ������תΪ����-1 ��תΪ����

float g_vehicleSpeed = 0.0;

float g_MinDisBetweenTwoTarget = 1.5;//m

float g_debugRoadWheelAngle=0.0;

uint8_t g_debugEnable = 0;
 
