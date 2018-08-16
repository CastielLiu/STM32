#include "delay.h"
#include "can.h"
#include "timer.h"
#include "usart.h"
#include "exti.h"
#include "function.h"

int main()
{
	extern float ch1_duty_cycle  ,ch2_duty_cycle ; 
	extern u16 eps_steer_angle;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	delay_init();
	delay_ms(500);
	uart_init(115200);
	EXTIX_Init();
	TIM2_Init(9999,72-1);//10ms溢出
	TIM5_Cap_Init(65535,72-1);//每计数一次1us
	TIM3_PWM_Init(999,36-1); // 每计数一次0.5us
	TIM3->CCR1 = 499;
	TIM3->CCR2 = 499;
	delay_ms(500);
	
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);
	
	while(1)
	{
		delay_ms(100);
		//printf("duty_cycle1 = %f\tduty_cycle2 = %f\r\n",ch1_duty_cycle,ch2_duty_cycle);

		//printf("eps_steer_angle = %3.1f\r\n",eps_steer_angle*0.1/870);
		//printf("angle = %3.1f\r\n",cal_angle(ch1_duty_cycle,ch2_duty_cycle));
		cal_angle(ch1_duty_cycle,ch2_duty_cycle);
	}
}


 