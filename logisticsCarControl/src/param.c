#include "param.h"
//�ڴ��ļ��ж�������Ҫ��ȫ�ֱ��������ڶ�Ӧ��ͷ�ļ��н�������

float g_speedPid_Kp = 1.0;
float g_speedPid_Ki = 0.0;
float g_speedPid_Kd = 0.0;

float g_steerPid_Kp = 1.0;
float g_steerPid_Ki = 0.0;
float g_steerPid_Kd = 0.0;

float ch1_duty_cycle =0.0 ,ch2_duty_cycle =0.0; 

float g_eps_can_angle=0.0;  
u8 g_eps_angle_status = EPS_ANGLE_VALID;//��Ч

float g_requestAngle , g_actualAngle;
float g_requestSpeed , g_actualSpeed;

float g_maxTorque = 5.0;//5.0Nm   �����õ����Ť��Ϊ8Nm����

float g_vehicleSpeed_LF =0.0;
float g_vehicleSpeed_RF =0.0;

float g_teleControlMaxSpeed= TELECONTROL_LOW_SPEED;//ң��ģʽ�µ�Ĭ������ٶ�Ϊ����

u8 g_teleSafetyCnt = 0;//ң������ȫ����������ֹ��ң�����źŶ�ʧ�������⡣



