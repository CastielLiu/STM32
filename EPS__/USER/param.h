#ifndef PARAM_H_
#define PARAM_H_
#include "sys.h"

#define KEY_CONTROL_DEBUG 1  //利用按键控制eps旋转，用做测试，1使能，0不使能



extern float g_speedPid_Kp  ;
extern float g_speedPid_Ki  ;
extern float g_speedPid_Kd  ;

extern float g_steerPid_Kp  ;
extern float g_steerPid_Ki  ;
extern float g_steerPid_Kd  ;

extern float ch1_duty_cycle  ,ch2_duty_cycle ; 

extern u16 g_eps_can_angle;

extern float g_requestAngle , g_actualAngle;
extern float g_requestSpeed , g_actualSpeed;

extern float g_maxTorque;

#endif