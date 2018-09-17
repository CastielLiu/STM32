#include "speedDetect.h"
#include "timer.h"
#include "usart.h"
#include "can.h"
#include "delay.h"
#include "param.h"

  
void speedDetect_Init(u16 arr)
{
	RCC->APB1ENR |= 1<<2;//TIM4ʱ��ʹ��
	RCC->APB2ENR |= 1<<6;//PORTEʱ��ʹ��
	
	GPIOE->CRL &= 0xFFFFFFF0;  //PE0�Ĵ�����λ
	GPIOE->CRL |= 0x00000004;  //��������
	
	TIM4 ->ARR = arr;
	TIM4 ->PSC = 0;//����Ƶ
	TIM4 ->CR1 &= 0xFC8F; //ʱ�ӷָ�0�����ϼ���ģʽ
	
	TIM_ETRClockMode2Config(TIM4, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0); 
	TIM4 ->CNT = 0;//����������
	
	TIM4->CR1 |= 0x00000001;//ʹ�ܼ�����
}