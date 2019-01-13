#include "main.h"

//KEY_UP  					   PA0 ��¼Ŀ���
//KEY0  						PE4 
//KEY1  						PE3 ��ʼ��ȡ�����˼�ʻ


//����ת�Ǵ�����    PA1   ADC1 CH1
//PWMת�����				PA7  	TIM3_CH2
//�������					PB2
//���ʹ��  				PF11

float g_requestAngle = 0.0;
int main(void)
{	

 	u16 Adc_value = 0 ;
	float current_angle = 0;
	float  angle_sensor_voltage;
	
	float  mid_voltage= 1.6; //ת���е��ѹֵ �궨
	float  angle_increment = 360.0/3.3; //������ÿ�仯 1V �Ƕ�����
	
	float angle_differ = 0.0;

	LED0 = 0;
	system_init();

  while(1)//25ms
	{
		Adc_value = Get_Adc_Average(ADC_Channel_1,5);
		angle_sensor_voltage = Adc_value/4095 *3.3;
		current_angle = (angle_sensor_voltage - mid_voltage) * angle_increment;
	
		angle_differ = g_requestAngle - current_angle;
		steer_control(angle_differ);
		sendCurrentAngleToUsart(current_angle);
		printf("requestAngle = %.2f\t currentAngle= %.2f\r\n",g_requestAngle,current_angle);
	}			
 }


