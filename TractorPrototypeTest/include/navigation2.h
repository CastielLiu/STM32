#ifndef NAVIGATION2_H_
#define NAVIGATION2_H_

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



void relative_XY_dis_yaw(gps_sphere_t  gps_base,gps_sphere_t  gps,gps_rect_t *rectangular);
float  point2point_dis(gps_sphere_t  gps_base,gps_sphere_t  gps);
	
float get_t_yaw(gps_sphere_t gps_base,gps_sphere_t gps );

float LateralError(double t_yaw_start,double t_yaw_now,float dis2end);
int gps_check(gps_sphere_t *gps);

#endif


