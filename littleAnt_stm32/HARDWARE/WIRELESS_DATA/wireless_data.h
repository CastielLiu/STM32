#ifndef WIRELESS_DATA_H_
#define WIRELESS_DATA_H_
#include "sys.h"

typedef struct carControlData
{
	u16 speed_l;
	u16 speed_r;
	u16 angle;
	float maxSpeed;
	float maxAngle;
	
}carControlData_t;

carControlData_t dataConvert(u8 *buf);


#endif

