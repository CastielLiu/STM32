#include "speedDetect.h"
#include "timer.h"
#include "usart.h"
#include "can.h"
#include "delay.h"
#include "param.h"

  
void speedDetect_Init(u16 arr)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms arr
	TIM_TimeBaseStructure.TIM_Prescaler =0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //��0��ʱ�ӷָ�:TDTS = Tck_tim	   psc
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0); //����Ϊ�ⲿ����ģʽ
	TIM_SetCounter(TIM2, 0);					 //����������
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
	
	
	
//����ΪTIM4��ʼ��Ϊ����ģʽ	
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


