#ifndef FUNCTION_H_
#define FUNCTION_H_
#include "sys.h"

typedef struct carControlData
{
	u16 speed_l;
	u16 speed_r;
	u16 angle;
	
}carControlData_t;

carControlData_t dataConvert(u8 *buf);


#endif

