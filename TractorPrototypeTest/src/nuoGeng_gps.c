#include "nuoGeng_gps.h"
#include "usart.h"

union CON
{
	u8 in[8];
	double out;
}convert;

static gps_data_t *gpsPtr = NULL;

u8 gpsParseNuoGeng(const u8 *gps_data_buf)
{
	int i = 1;
	for(;i<DMA_DATA_NUM-105;i++)
	{
		if(gps_data_buf[i]==0x64 && gps_data_buf[i-1]==0x14)
		{
			gpsPtr = (gps_data_t *)(&g_gps_data_buf[i-2]);
			break;
		}
	}
	
	if(i>=DMA_DATA_NUM-105) return 1;
	
		g_gps_sphere_now.yaw = *(float  *)gpsPtr->yaw;
		
		memcpy(convert.in,gpsPtr->lon,8);
		g_gps_sphere_now.lon = convert.out;
		
		memcpy(convert.in,gpsPtr->lat,8);
		g_gps_sphere_now.lat = convert.out;
		
		g_vehicleSpeed = sqrt(pow(*(float *)gpsPtr->vel_e,2)+pow(*(float *)gpsPtr->vel_n,2));
		
		g_u32_lon = g_gps_sphere_now.lon *180 /pi *10000000;
		g_u32_lat = g_gps_sphere_now.lat *180 /pi *10000000;
		g_u16_yaw = g_gps_sphere_now.yaw *180 /pi *100;
		
		//g_s16_speed = 10;//km/h  ??100?
		//g_int_height = 9.0 *1000 ;
		return 0;		
}

void showBufData( u8* buf,int len)
{
	int i=0;
	for(;i<len;i++)
		printf("%x\t",buf[i]);
	printf("\r\n");
}
