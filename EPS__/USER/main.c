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
#include "function.h"

//TIM5_CH1 捕获 PA0  （AS 3号引脚）  高精度小范围角度引脚
//TIM5_CH2 捕获 PA1	 （RS 6号引脚）  低精度大范围角度引脚
//TIM3_CH1 PWM  PA6  （TS 4号引脚）  模拟扭矩信号
//TIM3_CH2 PWM  PA7  （TS 5号引脚）  模拟扭矩信号
	
 int main(void)
 {		
	extern float ch1_duty_cycle  ,ch2_duty_cycle ; 
	extern u16 eps_can_angle;
	extern u8 usart2_angle_buf[4];
	 float request_angle=0.0;
	float eps_pwm_angle=0.0;
	 float angle_err=0.0;
	 pid_t_ steer_pid;
	 PID_init(&steer_pid);
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口 
	BEEP_Init();		 	//初始化蜂鸣器IO
	EXTIX_Init();         	//初始化外部中断输入 
	 
	TIM2_Init(9999,72-1);//10ms溢出 			 //定时器中断发送消息
	TIM5_Cap_Init(65535,72-1);//每计数一次1us    //捕获角度pwm信号
	TIM3_PWM_Init(999,36-1); // 每计数一次0.5us  //模拟扭矩pwm信号
	TIM3->CCR1 = 499;
	TIM3->CCR2 = 499;

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);
	
	LED0=0;					//先点亮红灯
	while(1)
	{	    	
		delay_ms(20);	 
		request_angle = *(u16*)&usart2_angle_buf[2]*0.1;
		eps_pwm_angle = cal_angle(ch1_duty_cycle,ch2_duty_cycle);
		//angle_err = request_angle- eps_pwm_angle;
		
		steer_control(PID1_realize(&steer_pid,request_angle,eps_pwm_angle));
		printf("request=%.1f\tcurrent=%.1f\r\n",request_angle,eps_pwm_angle);
	}	 
}
