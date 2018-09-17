#ifndef STEER_CONTROL_H_
#define STEER_CONTROL_H_
#include "sys.h"

void steerControl_Init(u16 arr,u16 psc);
void steer_control(float torque);


#endif