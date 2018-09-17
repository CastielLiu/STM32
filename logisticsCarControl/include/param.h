#ifndef PARAM_H_
#define PARAM_H_
#include "sys.h"

#define KEY_CONTROL_DEBUG 1  //利用按键控制eps旋转，用做测试，1使能，0不使能
#define ENCODER_LINE_CNT  30 //编码器线数，即每圈的脉冲个数
#define WHEEL_RADIUS    0.2   //车轮半径 m
#define MATH_PI 3.1415926535



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

extern float g_vehicleSpeed ;

#endif