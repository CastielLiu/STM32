#include "timer.h"
#include "usart.h"
#include "can.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK 精英 STM32开发板
//PWM  驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

u8 engine_status[]={0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00};
u8 engine_speed[]= {0x00,0x03,0xE8,0x00,0x00,0x00,0x00,0x00};
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
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
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

void TIM2_IRQHandler(void)   //TIM2中断
{
	static u16 cnt = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		Can_Send_Msg(0x280,engine_status,8);//发动机状态ID 10ms
		
		Can_Send_Msg(0x1A0,engine_speed,8);//发动机转速ID  10ms
		if(cnt%2==0)
			Can_Send_Msg(0x318,car_speed,8);//车速ID 20ms
		cnt++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
	}
}




//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
TIM_OCInitTypeDef  TIM_OCInitStructure;
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA	| RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5                                                                       	 //用于TIM3的CH2输出的PWM通过该LED显示
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  //复用开漏
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
 

}

//定时器5通道1输入捕获配置

void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM5时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO, ENABLE);  //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 |GPIO_Pin_1;  //PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 下拉
	
	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1 ; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2 ; 
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM5,TIM_IT_CC1 | TIM_IT_CC2 ,ENABLE);//允许CC1IE捕获中断	
	
   	TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
   
}

float ch1_duty_cycle =0.0 ,ch2_duty_cycle =0.0; 
u16 ch1_rising_cnt=0 , ch2_rising_cnt = 0; // 捕获到上升沿时的计数
u16 ch1_falling_cnt =0 ,ch2_falling_cnt = 0;// 捕获到下降沿时的计数
u8 ch1_capture_mode = 0 ,ch2_capture_mode=0;//上升沿捕获0  下降沿捕获1
u16 ch1_high_level_time = 0 ,ch2_high_level_time = 0;
u16 ch1_low_level_time = 0  ,ch2_low_level_time =0;

//定时器5中断服务程序	 
void TIM5_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//通道1捕获中断
	{
		if(ch1_capture_mode==0)//捕获到上升沿
		{
			ch1_rising_cnt = TIM5->CCR1;
			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);//切换为下降沿捕获
			ch1_capture_mode = 1;
			
			if(ch1_rising_cnt>ch1_falling_cnt)
				ch1_low_level_time = ch1_rising_cnt - ch1_falling_cnt;//上升沿时刻  - 下降沿时刻=低电平时间
			else
				ch1_low_level_time = 65535-ch1_falling_cnt+ch1_rising_cnt;
		}
		else //捕获到下降沿
		{
			ch1_falling_cnt = TIM5->CCR1;
			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);//切换为上升沿捕获	
			ch1_capture_mode = 0;
			
			if(ch1_rising_cnt < ch1_falling_cnt)
				ch1_high_level_time =  ch1_falling_cnt - ch1_rising_cnt;//下降沿时刻 - 上升沿时刻=高电平时间
			else
				ch1_high_level_time = 65535 + ch1_falling_cnt - ch1_rising_cnt;			
		}	
		if(ch1_capture_mode==1)//下一次捕获下降沿
		{
			ch1_duty_cycle = 1.0*ch1_low_level_time/(ch1_high_level_time + ch1_low_level_time)*100;
			//printf("duty_cycle = %f%%\r\n",duty_cycle*100);
			
		}
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1); //清除中断标志位
	}
	
	else if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)//通道2捕获中断
	{
		if(ch2_capture_mode==0)//捕获到上升沿
		{
			ch2_rising_cnt = TIM5->CCR2;
			TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);//切换为下降沿捕获
			ch2_capture_mode = 1;
			
			if(ch2_rising_cnt>ch2_falling_cnt)
				ch2_low_level_time = ch2_rising_cnt - ch2_falling_cnt;//上升沿时刻  - 下降沿时刻=低电平时间
			else
				ch2_low_level_time = 65535-ch2_falling_cnt+ch2_rising_cnt;
		}
		else //捕获到下降沿
		{
			ch2_falling_cnt = TIM5->CCR2;
			TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising);//切换为上升沿捕获	
			ch2_capture_mode = 0;
			
			if(ch2_rising_cnt < ch2_falling_cnt)
				ch2_high_level_time =  ch2_falling_cnt - ch2_rising_cnt;//下降沿时刻 - 上升沿时刻=高电平时间
			else
				ch2_high_level_time = 65535 + ch2_falling_cnt - ch2_rising_cnt;			
		}	
		if(ch2_capture_mode==1)//下一次捕获下降沿
		{
			ch2_duty_cycle = 1.0*ch2_low_level_time/(ch2_high_level_time + ch2_low_level_time)*100;
			//printf("duty_cycle = %f%%\r\n",duty_cycle*100);
			
		}
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC2); //清除中断标志位
	}

 
}
