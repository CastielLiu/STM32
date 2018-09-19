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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50M时钟速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms arr
	TIM_TimeBaseStructure.TIM_Prescaler =0; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设0置时钟分割:TDTS = Tck_tim	   psc
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0); //设置为外部计数模式
	TIM_SetCounter(TIM2, 0);					 //计数器清零
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
	
	
	
//以下为TIM4初始化为计数模式	
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


