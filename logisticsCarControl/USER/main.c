#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "beep.h"
#include "timer.h"
#include "can.h"
#include "pid.h"
#include "speedDetect.h"
#include "speedControl.h"
#include "steerControl.h"
#include "brakeControl.h"
#include "param.h"


//TIM3_CH1 PWM  PA6  （TS 4号引脚）  模拟扭矩信号
//TIM3_CH2 PWM  PA7  （TS 5号引脚）  模拟扭矩信号
//TIM6     计时   		10ms溢出一次 定时发送消息等，对时间精度要求不高
//TIM7     计时			定时溢出，仅用于计算轮速传感器脉冲，时间精度要求高。
//TIM4_CNT 计数 PE0   接轮速编码器
//TIM2_CNT 计数 PA15  接轮速编码器
//KEY_UP 		PA0
//KEY0-2 		PE4, PE3, PE2
//DAC  CH1 AIN	PA4   车辆速度信号，首先假定模拟量与速度正相关，再利用pid调节
//DAC__CH2 AIN  PA5	  制动模拟信号，
//IO       		PF0   前进0  后退1
//IO			PF1   制动1  取消制动0

//按键功能
//key0 逆时针旋转
//key1 顺时针旋转
//key2 转角校零  ->  key2持续按下15s,不要松开，点火信号下电3s，松开key2,点火信号上电，校零完成
	
 int main(void)
 {		
	float angle_err=0.0;
	 
	pid_t_ steer_pid,speed_pid;
	 
	PID_init(&steer_pid);//转向控制pid初始化
	steer_pid.Kp =g_steerPid_Kp ;
	steer_pid.Ki =g_steerPid_Ki ;
	steer_pid.Kd =g_steerPid_Kd ;

	PID_init(&speed_pid);//速度控制pid初始化 	 
	speed_pid.Kp =g_speedPid_Kp ;
	speed_pid.Ki =g_speedPid_Ki ;
	speed_pid.Kd =g_speedPid_Kd ;
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口 
	BEEP_Init();		 	//初始化蜂鸣器IO
	EXTIX_Init();         	//初始化外部中断输入 
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);
	
	speedDetect_Init(65535);	
	
	
	steerControl_Init(999,36-1);//2kHZ
	speedControl_Init();
	brakeControl_Init();
	
	TIM6_Init(9999,72-1);//10ms溢出 			 //定时器中断发送消息
	TIM7_Init(30000-1,720-1);//300ms溢出
	
	
	LED0=0;					//点亮红灯
	while(1)
	{	    	
		delay_ms(20);	 
		speed_control(8.0);
		brake_control(2.8);
		
		//steer_control(PID1_realize(&steer_pid,request_angle,eps_pwm_angle));
		
	
		
	}	 
}
