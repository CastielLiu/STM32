#include "nuoGeng_gps.h"

union CON
{
	u8 in[8];
	double out;
}convert;

static gps_data_t *gpsPtr = (gps_data_t *)g_gps_data_buf;

u8 gpsParseNuoGeng(const u8 *gps_data_buf)
{
	if(gpsPtr->header[0]==0x14 && gpsPtr->header[1]==0x64)
	{
		g_gps_sphere_now.yaw = *(float  *)gpsPtr->yaw;
		
		memcpy(convert.in,gpsPtr->lon,8);
		g_gps_sphere_now.lon = convert.out;
		
		memcpy(convert.in,gpsPtr->lat,8);
		g_gps_sphere_now.lat = convert.out;
		
		g_vehicleSpeed = sqrt(pow(*(float *)gpsPtr->vel_e,2)+pow(*(float *)gpsPtr->vel_n,2));
		
		g_int_lon = g_gps_sphere_now.lon *180 /pi *10000000;
		g_int_lat = g_gps_sphere_now.lat *180 /pi *10000000;
		g_u16_yaw = g_gps_sphere_now.yaw *180 /pi *100;
		
		//g_s16_speed = 10;//km/h  ??100?
		//g_int_height = 9.0 *1000 ;
		return 0;
	}
	else 
		return 1;
					
}
