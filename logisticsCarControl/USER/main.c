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
#include "lcd.h"
#include "pstwo.h"


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
/*--ң��������--------*/
//DAT������		PB12
//CMD������	 	PB13
//CS Ƭѡ��		PB14
//CLKʱ����  	PB15
/*--����4�����ſ�����Ϊspi2ͨ������--*/

//LCD     PB0 PD0 PD1 PD4 PD5 PD8 PD9 PD10 PD14 PD15 
//				PE7-15 PG0 PG12

//��������
//key0 ��ʱ����ת
//key1 ˳ʱ����ת
//key2 ת��У��  ->  key2��������15s,��Ҫ�ɿ�������ź��µ�3s���ɿ�key2,����ź��ϵ磬У�����
	
 int main(void)
 {		
	extern u8 Data[9] ;//ң������������
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

/*---------------ϵͳ��ʼ��----------------------------*/	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ� 
	BEEP_Init();		 	//��ʼ��������IO
	EXTIX_Init();         	//��ʼ���ⲿ�ж����� 
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);
	
	PS2_Init();				//ң��������ų�ʼ��
	
	speedDetect_Init(65535);	
	
	steerControl_Init(999,36-1);//2kHZ
	speedControl_Init();
	brakeControl_Init();
	
	TIM6_Init(9999,72-1);//10ms��� 			 //��ʱ���жϷ�����Ϣ
	TIM7_Init(30000-1,720-1);//300ms���
	
	LCD_Init();
	LCD_showName();
	LED0=0;					//�������
	while(1)
	{
		if(DriveringMode == TELECONTROL_MODE)//ң��ģʽ
		{
			if( !PS2_RedLight()) //�ж��ֱ��Ƿ�Ϊ���ģʽ����,ң����Ч
			{
				delay_ms(30);//��ʱ����Ҫ������������ݣ���Ҫ��ʱ�������´�����
				PS2_RequestData();	 //�ֱ�����������
				if(PS2_L2 ==0) //���2����
					g_teleSafetyCnt = 0 ;//g_teleSafetyCnt����50ms�Լ�1��
				if(g_teleSafetyCnt > 10)//500ms��û�м�⵽���2�������£�˵��ң�����źŶ�ʧ�������ƶ�
				{
					speed_control(0);
					brake_control(3.3);//ȫ���ƶ�
					g_teleSafetyCnt =20;//��g_teleSafetyCnt����Ϊ����10�����֣�
										//��ֹ���Լ������С��10��ִ������Ĵ���
					continue;
				}
				//��⵵λ����
				if(PS2_RECTANGLE==0) g_teleControlMaxSpeed = TELECONTROL_LOW_SPEED;
				else if(PS2_TRIANGLE==0)  g_teleControlMaxSpeed = TELECONTROL_MIDDLE_SPEED;
				else if(PS2_CIRCLE ==0) g_teleControlMaxSpeed = TELECONTROL_HIGH_SPEED;
				speed_control(PS2_GenerateSpeed(Data));
				
			}
			else//ң��ģʽ����ң����Ч
			{
				speed_control(0);//�ٶ���0
				brake_control(3.3);//ȫ���ƶ�
			}
		}
		else if(DriveringMode == DRIVERLESS_MODE) //���˼�ʻģʽ
		{
			delay_ms(20);	 
			//speed_control(8.0);
			//brake_control(2.8);
			//steer_control(PID1_realize(&steer_pid,request_angle,g_eps_can_angle));
			
		}	
		
	}
	return 0;	
}
