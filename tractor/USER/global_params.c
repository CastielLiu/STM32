#include "global_params.h"



u8 gps_data_buf[105];

gps_data_t *gpsPtr = (gps_data_t *)&(gps_data_buf[1]);

gps_sphere_t gps_sphere_now;

char mode_name[20]= "Record Point";

int send_lon,send_lat,send_height;
u16 send_speed,send_yaw;
u8 send_gps_status,send_satellites;   //use to can send msg 
u16  send_steer_angle;   //ԭʼ�Ƕ�����90��  15�ֽ�Ϊ����0��1��


u8 actual_path_vertwx_num; //·���������

u8 start_driverless_flag = 0;

gps_sphere_t target_point[MAX_PATH_VERTEX_NUM]={0.0,0.0,0.0};  


