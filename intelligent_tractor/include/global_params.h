#ifndef GLOBAL_PARAMS_H_
#define GLOBAL_PARAMS_H_
#include "sys.h"
#include "navigation2.h"

#define MAX_PATH_VERTEX_NUM 10

extern u8 gps_data_buf[105];

extern gps_sphere_t gps_sphere_now;
extern gps_data_t *gpsPtr;

extern char mode_name[20];

extern int send_lon,send_lat,send_height;
extern u16 send_speed,send_yaw;
extern u8 send_gps_status,send_satellites;   //use to can send msg 
extern u16  send_steer_angle;   //原始角度扩大90倍  15位为方向，0左，1右

extern u8 actual_path_vertwx_num;

extern gps_sphere_t target_point[MAX_PATH_VERTEX_NUM];  

extern u8 start_driverless_flag;


#endif

