#ifndef NAVIGATION2_H_
#define NAVIGATION2_H_
#endif
#include "stdlib.h"
#include "math.h"
#include"sys.h"
#ifndef pi
#define pi 3.141592653589
#endif


typedef struct
{
    double lon;
    double lat;
	float  yaw;
} gps_sphere_t;

typedef struct 
{
    double x;
    double y;
    double distance;
    double yaw;
}gps_rect_t;

typedef struct
{
    u8 head_[4];//14 64 ** **
    u8 a[4];  //system status 2×Ö½Ú  filter status 2×Ö½Ú
    u8 time[8];
    u8 lat[8];
    u8 lon[8];
    u8 height[8];
    u8 vel_n[4];
    u8 vel_e[4];
    u8 down_velocity[4];
    u8 Body_acceleration_x[4];
    u8 Body_acceleration_y[4];
    u8 Body_acceleration_z[4];
    u8 G_force[4];
    u8 Roll[4];
    u8 Pitch[4];
    u8 yaw[4];
    u8 angle_velocity_x[4];
    u8 angle_velocity_y[4];
    u8 angle_velocity_z[4];
    u8 latitude_std_deviation[4];
    u8 longtitude_std_deviation[4];
    u8 height_std_deviation[4];
    
}gps_data_t;

void relative_XY_dis_yaw(gps_sphere_t  gps_base,gps_sphere_t  gps,gps_rect_t *rectangular);
float  point2point_dis(gps_sphere_t  gps_base,gps_sphere_t  gps);
	
float get_t_yaw(gps_sphere_t gps_base,gps_sphere_t gps );

gps_sphere_t get_gps_sphere(gps_data_t *gps);
float LateralError(double t_yaw_start,double t_yaw_now,float dis2end);
int gps_check(gps_sphere_t *gps);

