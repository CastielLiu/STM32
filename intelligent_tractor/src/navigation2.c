#include "navigation2.h"
#include "usart.h"


int gps_check( gps_sphere_t *gps)
{
	if((abs(gps->lat) < 100*pi/180 && abs(gps->lon) < 200*pi/180))
		return 1;
	else
		return 0;
}


gps_sphere_t get_gps_sphere(gps_data_t *gps)
{
	gps_sphere_t gps_sphere={0.0,0.0,0.0};
	printf("%f %f %f \r\n",(*(double*)gps->lat),(*(double*)gps->lon),(*(double*)gps->yaw));
	gps_sphere.lat = (*(double*)gps->lat);
	gps_sphere.lon = (*(double*)gps->lon);
	gps_sphere.yaw = (*(float*)gps->yaw);
	return gps_sphere;
}

//两点之间的距离
float  point2point_dis(gps_sphere_t  gps_base,gps_sphere_t  gps)
{
	double x = (gps.lon -gps_base.lon)*111000*cos(gps.lat)*180/pi;
	double y = (gps.lat -gps_base.lat ) *111000*180/pi;
	return  sqrt(x * x + y * y);
}

void relative_XY_dis_yaw(gps_sphere_t  gps_base,gps_sphere_t  gps,gps_rect_t *rectangular)
{
	rectangular->x = (gps.lon -gps_base.lon)*111000*cos(gps.lat)*180/pi;
	rectangular->y = (gps.lat -gps_base.lat ) *111000*180/pi;

	rectangular->distance = sqrt(rectangular->x * rectangular->x + rectangular->y * rectangular->y);
	rectangular->yaw = atan2(rectangular->x,rectangular->y);
}
//右偏为正，左偏为负
float LateralError(double t_yaw_start,double t_yaw_now,float dis2end)
{
    return (sin((t_yaw_start - t_yaw_now))) *dis2end;
}

float get_t_yaw(gps_sphere_t gps_base,gps_sphere_t gps )
{
	gps_rect_t gps_rect;
	relative_XY_dis_yaw(gps_base,gps,&gps_rect);
	return gps_rect.yaw;
}

