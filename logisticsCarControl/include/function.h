#ifndef FUNCTION_H_
#define FUNCTION_H_
#include "math.h"
#include "usart.h"
#include "stm32f10x.h"

float fun_mod(float x,float y);
float cal_angle(float duty_cycle1,float duty_cycle2);
int fun_round(float x);

void steer_control(float torque);
void speed_control(float set_speed);
void brake_control(float set_brake_voltage);

#endif