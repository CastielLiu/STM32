#include "speedDetect.h"
#include "timer.h"
#include "usart.h"
#include "can.h"
#include "delay.h"
#include "param.h"

  
void speedDetect_Init(u16 arr)
{
	RCC->APB1ENR |= 1<<2;//TIM4时钟使能
	RCC->APB2ENR |= 1<<6;//PORTE时钟使能
	
	GPIOE->CRL &= 0xFFFFFFF0;  //PE0寄存器复位
	GPIOE->CRL |= 0x00000004;  //浮空输入
	
	TIM4 ->ARR = arr;
	TIM4 ->PSC = 0;//不分频
	TIM4 ->CR1 &= 0xFC8F; //时钟分割0，向上计数模式
	
	TIM_ETRClockMode2Config(TIM4, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0); 
	TIM4 ->CNT = 0;//计数器清零
	
	TIM4->CR1 |= 0x00000001;//使能计数器
}