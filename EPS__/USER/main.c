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

//TIM5_CH1 ���� PA0  ��AS 3�����ţ�  �߾���С��Χ�Ƕ�����
//TIM5_CH2 ���� PA1	 ��RS 6�����ţ�  �;��ȴ�Χ�Ƕ�����
//TIM3_CH1 PWM  PA6  ��TS 4�����ţ�  ģ��Ť���ź�
//TIM3_CH2 PWM  PA7  ��TS 5�����ţ�  ģ��Ť���ź�
	
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
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 
	BEEP_Init();		 	//��ʼ��������IO
	EXTIX_Init();         	//��ʼ���ⲿ�ж����� 
	 
	TIM2_Init(9999,72-1);//10ms��� 			 //��ʱ���жϷ�����Ϣ
	TIM5_Cap_Init(65535,72-1);//ÿ����һ��1us    //����Ƕ�pwm�ź�
	TIM3_PWM_Init(999,36-1); // ÿ����һ��0.5us  //ģ��Ť��pwm�ź�
	TIM3->CCR1 = 499;
	TIM3->CCR2 = 499;

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);
	
	LED0=0;					//�ȵ������
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
