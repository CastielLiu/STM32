#ifndef PARAM_H_
#define PARAM_H_
#include "sys.h"

#define KEY_CONTROL_DEBUG 1  //利用按键控制eps旋转，用做测试，1使能，0不使能
#define ENCODER_LINE_CNT  30 //编码器线数，即每圈的脉冲个数
#define WHEEL_RADIUS    0.2   //车轮半径 m
#define MATH_PI 3.1415926535

//遥控模式下的三种档位，高，中，低
#define TELECONTROL_HIGH_SPEED   15.0 //遥控模式下的最大速度
#define TELECONTROL_MIDDLE_SPEED 10.0
#define TELECONTROL_LOW_SPEED 	 5.0

#define EPS_ANGLE_VALID 1
#define EPS_ANGLE_INVALID 0


extern float g_speedPid_Kp  ;
extern float g_speedPid_Ki  ;
extern float g_speedPid_Kd  ;

extern float g_steerPid_Kp  ;
extern float g_steerPid_Ki  ;
extern float g_steerPid_Kd  ;

extern float ch1_duty_cycle  ,ch2_duty_cycle ; 

extern float g_eps_can_angle;
extern u8 g_eps_angle_status;

extern float g_requestAngle , g_actualAngle;
extern float g_requestSpeed , g_actualSpeed;

extern float g_maxTorque;

extern float g_vehicleSpeed_LF ;
extern float g_vehicleSpeed_RF ;
extern float g_teleControlMaxSpeed;
extern u8 g_teleSafetyCnt;


#endif

















