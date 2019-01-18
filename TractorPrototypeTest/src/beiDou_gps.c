#include "beiDou_gps.h"

void GetComma(const u8 *str,u8 *commaLocation_)
{
	u8 i = 0,j=1;
	for(;i<strlen(str);i++)
	{
		if(*(str+i)==',')
		{
			commaLocation_[j] = i;
			j++;
			if(j>MaxCommaNumInGpsData-1) break;
			
		}
	}
	
}

//ok·µ»Ø0
//Ê§°Ü·µ»Ø!0
u8 gpsParseBeiDou(const u8 *buf)
{
	u8 commaLocation[MaxCommaNumInGpsData];
	char tempBuf[20];
	double tempLat,tempLon,tempYaw;
	if(NULL == strstr(buf,"$GPRMC"))
		return 2;
	
	GetComma(buf,commaLocation);
	strncpy(tempBuf,&buf[commaLocation[2]+1],commaLocation[3]-commaLocation[2]-1);
	g_u8_gps_status = tempBuf[0];
	//printf("%s\r\n",buf);
	if('A' == g_u8_gps_status) 
	{
		strncpy(tempBuf,&buf[commaLocation[3]+1],commaLocation[4]-commaLocation[3]-1);
		tempLat = atof(tempBuf);
		strncpy(tempBuf,&buf[commaLocation[5]+1],commaLocation[6]-commaLocation[5]-1);
		tempLon = atof(tempBuf);
		
		tempLat = (tempLat - (int)(tempLat /100 )*100 )/60 + (int)(tempLat /100 ) ;
		tempLon = (tempLon - (int)(tempLon /100 )*100 )/60 + (int)(tempLon /100 ) ;
		
		if(tempLat >= 90 || tempLat<= 0 || tempLon>=180 || tempLon<=0 ) return 3;
		
		strncpy(tempBuf,&buf[commaLocation[8]+1],commaLocation[9]-commaLocation[8]-1);
		tempYaw = atof(tempBuf);
		
		//printf("yaw = %f\r\n",tempYaw);
		if(tempYaw >360.0 || tempYaw<0.0) return 3;
		
		strncpy(tempBuf,&buf[commaLocation[7]+1],commaLocation[8]-commaLocation[7]-1);
		
		
		g_vehicleSpeed = atof(tempBuf)* 1.85  ; 
		
		g_u32_lon = tempLon *10000000;
		g_u32_lat = tempLat *10000000;
		g_u16_yaw = tempYaw *100;
		//g_s16_speed = 10;//km/h  ??100?
		//g_int_height = 9.0 *1000 ;	
		
		
		g_gps_sphere_now.lat = tempLat*pi/180.0;
		g_gps_sphere_now.lon = tempLon*pi/180.0;
		g_gps_sphere_now.yaw = tempYaw*pi/180.0;
 
		
	//	printf("lat=%f\tlon=%fyaw=%f\r\n",g_gps_sphere_now.lat,g_gps_sphere_now.lon,g_gps_sphere_now.yaw);
		return 0;
	}
	else
		return 1;
}