#include "function.h"

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