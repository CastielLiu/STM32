#ifndef NUOGENG_GPS_H_
#define NUOGENG_GPS_H_
#include <sys.h>
#include <string.h>
#include "global_params.h"
#include "navigation2.h"

typedef struct
{
	u8 dummyData;
    u8 header[4];//14 64 ** **
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

u8 gpsParseNuoGeng(const uint8_t *buf);

void showBufData( u8* buf,int len);

 
 


#endif