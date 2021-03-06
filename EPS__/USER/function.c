#include "function.h"
#include "param.h"

//通过pwm捕获转角传感器信号占空比计算转角。
//目前使用Can总线读取转角信号，2018.9.1
float cal_angle(float duty_cycle1,float duty_cycle2)
{
	float A_AS = 4.8*(duty_cycle1-12.5);
	float A_RS = 4.8*(duty_cycle2-12.5);
	float temp = (fun_mod(A_AS+180,360.0)-14*A_RS)/360.0;
	int temp_K = fun_round(temp);
	float A_US =  6.0/17 * ( 360 * fun_mod( 13.0 * temp_K , 14.0 ) + fun_mod(A_AS+180,360.0) )- 15120.0/17 -60; //减去60，缩小AS与US的起始误差
	float RS_angle = 23.2*(duty_cycle2-12.5)-870;//RS角度值
	
	//printf("\r\nA_US = %.2f\r\n",A_US);
	if((A_US-RS_angle)>70)
		A_US = A_US-127.1;
	else if((A_US-RS_angle)<-70)
		A_US = A_US +127.1;
		
	//printf("AS=%.1f\tRS=%.1f\ttemp=%f\tK=%d\tUS=%.1f\tRS_angle=%.1f\r\n",A_AS,A_RS,temp,temp_K,A_US,RS_angle);
	return A_US;
}

float fun_mod(float x,float y)
{
	return x-floor(x/y)*y;
}

int fun_round(float x)
{
	return x>0?floor(x+0.5):ceil(x-0.5);
}

//转向控制
//模拟转矩信号，两路pwm输出
void steer_control(float torque)
{
	u16 ccr = 499;
	float duty_cyc = 0.5;
	
	if(torque>g_maxTorque)torque = g_maxTorque;
	else if(torque<-g_maxTorque) torque = -g_maxTorque;
	
	duty_cyc = (4.6875 * torque +50.0)/100;  //根据比例曲线计算占空比
	ccr = duty_cyc*1000-1;
	TIM3->CCR1 = ccr; 
	TIM3->CCR2 = 998-ccr;
	
	//printf("ccr = %d\r\n",ccr);
}

//速度控制，模拟信号0-3.3v 
//stm32只能输出0-3.3v模拟信号。电机驱动器控制信号为0-5v
//如需更大速度，可使用电压放大器。
void speed_control(float set_speed)
{
	u16 dac_val;
	float voltage = set_speed/8;  //假定0-5V对应0-40km/h
	
	if (voltage>3.3) voltage = 3.3;
	
	dac_val = voltage/3.3 * 4096;
	
	DAC->DHR12R1 = dac_val;  //12位右对齐通道1寄存器
}

//制动控制，需要正反两个方向。
void brake_control(float set_brake_voltage)
{
	u16 dac_val=0;
	float voltage = fabs(set_brake_voltage);
	
	if (voltage>3.3) voltage = 3.3;
	
	dac_val = voltage/3.3 * 4096;
	
	DAC->DHR12R2 = dac_val;  //12位右对齐通道2寄存器
}
