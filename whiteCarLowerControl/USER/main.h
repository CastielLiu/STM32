#ifndef MAIN_H_
#define MAIN_H_

#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "wdg.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
#include "exti.h"
#include "direction.h"
#include "can.h"
#include "navigation2.h"
#include "beep.h"
#include "lcd.h"
#include "math.h"
#include "stdlib.h"
#include "capture.h"
 
 
#include "24l01.h"
#include "function.h"
 
 
speedMsg_t g_speedMsg={0,1000000,0,0,0,1000}; //初始化总脉冲数为100万，防止因倒车引起的向下溢出。   
																				//里程计取值范围 -150M ~ 300KM    4倍边缘检测


void system_init()
{
	delay_init();	    	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	//CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);
	uart1_init(115200);	
	uart2_init(115200);	
	uart3_init(115200); 
 	LED_Init();
	BEEP_Init();
	//IWDG_Init(0,700);   //60ms
 	TIM3_PWM_Init(4999, 72-1); //200HZ
	TIM3->CCR2 = 1499; //1.5ms 转向
	//TIM3->CCR1 = 1499; // 速度  初始化不设定速度，防止异常
	
	EXTIX_Init();
	TIM5_Init(50000,72-1);	// 50ms溢出 用于计时
	TIM4_Capture_Init(0xFFFF,0); //脉冲计数
	
	
	LCD_Init();
	printf("初始化完成...\r\n");
}

#endif 
