#include "timer.h"
#include "usart.h"
#include "can.h"
#include "delay.h"
#include "param.h"
  

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

u8 engine_status[]={0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00};
u8 engine_speed[]= {0x00,0x03,0xE8,0x00,0x00,0x00,0x00,0x00};
//u8 car_speed[]=    {0x00,0x00,0x00,0x00,0x00,0x03,0xe8,0x00};
u8 car_speed[]=    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//TIM2 用于计时
void TIM2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM2
		TIM_IT_Update ,   //TIM 更新中断源
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设					 
}

void TIM5_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM5, //TIM2
		TIM_IT_Update ,   //TIM 更新中断源
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM5, ENABLE);  //使能TIMx外
						 
}


void TIM2_IRQHandler(void)   //TIM2中断
{
	static u16 cnt = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		//printf("  %d\r\n",cnt);
		Can_Send_Msg(0x280,engine_status,8);//发动机状态ID 10ms
		
		Can_Send_Msg(0x1A0,engine_speed,8);//发动机转速ID  10ms
		if(cnt%2==0)
		{
		//	car_speed
			Can_Send_Msg(0x318,car_speed,8);//车速ID 20ms
		}
		cnt++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}

void TIM5_IRQHandler(void)   //TIM2中断
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 
	{
		g_vehicleSpeed = TIM4->CNT * MATH_PI * 2* WHEEL_RADIUS /(0.05 * ENCODER_LINE_CNT);
		printf("speed = %f\t%d\r\n",g_vehicleSpeed,TIM4->CNT);
		TIM4 ->CNT = 0;
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}