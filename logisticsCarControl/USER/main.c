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


//TIM3_CH1 PWM  PA6  ��TS 4�����ţ�  ģ��Ť���ź�
//TIM3_CH2 PWM  PA7  ��TS 5�����ţ�  ģ��Ť���ź�
//TIM6     ��ʱ   		10ms���һ�� ��ʱ������Ϣ�ȣ���ʱ�侫��Ҫ�󲻸�
//TIM7     ��ʱ			��ʱ����������ڼ������ٴ��������壬ʱ�侫��Ҫ��ߡ�
//TIM4_CNT ���� PE0   �����ٱ�����
//TIM2_CNT ���� PA15  �����ٱ�����
//KEY_UP 		PA0
//KEY0-2 		PE4, PE3, PE2
//DAC  CH1 AIN	PA4   �����ٶ��źţ����ȼٶ�ģ�������ٶ�����أ�������pid����
//DAC__CH2 AIN  PA5	  �ƶ�ģ���źţ�
//IO       		PF0   ǰ��0  ����1
//IO			PF1   �ƶ�1  ȡ���ƶ�0

//��������
//key0 ��ʱ����ת
//key1 ˳ʱ����ת
//key2 ת��У��  ->  key2��������15s,��Ҫ�ɿ�������ź��µ�3s���ɿ�key2,����ź��ϵ磬У�����
	
 int main(void)
 {		
	float angle_err=0.0;
	 
	pid_t_ steer_pid,speed_pid;
	 
	PID_init(&steer_pid);//ת�����pid��ʼ��
	steer_pid.Kp =g_steerPid_Kp ;
	steer_pid.Ki =g_steerPid_Ki ;
	steer_pid.Kd =g_steerPid_Kd ;

	PID_init(&speed_pid);//�ٶȿ���pid��ʼ�� 	 
	speed_pid.Kp =g_speedPid_Kp ;
	speed_pid.Ki =g_speedPid_Ki ;
	speed_pid.Kd =g_speedPid_Kd ;
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 
	BEEP_Init();		 	//��ʼ��������IO
	EXTIX_Init();         	//��ʼ���ⲿ�ж����� 
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);
	
	speedDetect_Init(65535);	
	
	
	steerControl_Init(999,36-1);//2kHZ
	speedControl_Init();
	brakeControl_Init();
	
	TIM6_Init(9999,72-1);//10ms��� 			 //��ʱ���жϷ�����Ϣ
	TIM7_Init(30000-1,720-1);//300ms���
	
	
	LED0=0;					//�������
	while(1)
	{	    	
		delay_ms(20);	 
		speed_control(8.0);
		brake_control(2.8);
		
		//steer_control(PID1_realize(&steer_pid,request_angle,eps_pwm_angle));
		
	
		
	}	 
}
