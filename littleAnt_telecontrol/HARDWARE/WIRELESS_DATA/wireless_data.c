#include "wireless_data.h"

carControlData_t dataConvert(u8 *buf)
{
	carControlData_t car_data;

	car_data.speed_l = (u16)buf[0] << 8 ;
	car_data.speed_l += buf[1];
	
	car_data.speed_r = (u16)buf[2] << 8 ;
	car_data.speed_r += buf[3];
	
	car_data.angle = (u16)buf[4] << 8;
	car_data.angle += buf[5];
	
	return car_data;
}


