#include "param.h"
//在此文件中定义所需要的全局变量，并在对应的头文件中进行声明

float g_speedPid_Kp = 1.0;
float g_speedPid_Ki = 0.0;
float g_speedPid_Kd = 0.0;

float g_steerPid_Kp = 1.0;
float g_steerPid_Ki = 0.0;
float g_steerPid_Kd = 0.0;

float ch1_duty_cycle =0.0 ,ch2_duty_cycle =0.0; 

u16 g_eps_can_angle=0;  //true_angle = g_eps_can_angle *0.1

float g_requestAngle , g_actualAngle;
float g_requestSpeed , g_actualSpeed;

float g_maxTorque = 5.0;//5.0Nm   可设置的最大扭矩为8Nm！！

