#include "capture.h"
#include "usart.h"
#include "string.h"


void TIM5_Init(u16 arr,u16 psc)  
{	 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能TIM5时钟
	
	//初始化定时器5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//中断分组初始化
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIM5,TIM_IT_Update , ENABLE);//允许更新中断
	
  TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
   
}


extern speedMsg_t g_speedMsg;
extern u8 g_ykSafty_num ; //遥控信号异常时，急停
extern u8 start_driverless_flag;
void TIM5_IRQHandler(void)
{ 
		if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET) //更新中断
		{
			g_speedMsg.pulseNumPer50ms = TIM4->CNT - 10000;
		//	printf("pulseNumPer50ms=%d\r\n",g_speedMsg.pulseNumPer50ms );
			g_speedMsg.totalPulseNum +=g_speedMsg.pulseNumPer50ms;
			g_speedMsg.motorSpeed = g_speedMsg.pulseNumPer50ms *1.0/g_speedMsg.encoderLineNum /4./50.;
			TIM4->CNT = 10000;
			TIM4->CNT = 10000;
			if(start_driverless_flag ==0)//遥控模式下
			{
				g_ykSafty_num++;
				if(g_ykSafty_num >8)//50ms*8
					TIM3->CCR1 = 1499; //speed = 0;
			}
		}
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //清除中断标志位
}
// TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
// TIM_ICInitTypeDef TIM_ICInitStructure;

void TIM4_Capture_Init(uint16_t arr,uint16_t psc)
{  
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //貌似不可以在这定义这两个结构体，否则影响外部中断
	  TIM_ICInitTypeDef TIM_ICInitStructure;       
	  GPIO_InitTypeDef GPIO_InitStructure1;  
	  NVIC_InitTypeDef NVIC_InitStructure;
	
		memset(&TIM_TimeBaseStructure,sizeof(TIM_TimeBaseStructure),0);
		memset(&TIM_ICInitStructure,sizeof(TIM_ICInitStructure),0);
		memset(&GPIO_InitStructure1,sizeof(GPIO_InitStructure1),0);
		memset(&NVIC_InitStructure,sizeof(NVIC_InitStructure),0);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);   

		//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);  //部分重映射 CH1->PA15 CH2->PB3
																													// 
		//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //??JTAG??,?PB3,PB4????IO???
	
		GPIO_StructInit(&GPIO_InitStructure1); //clear to zero deflaut value

		GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  
		GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure1);  
	////////////////	
		TIM_DeInit(TIM4);    

		TIM_TimeBaseStructure.TIM_Prescaler = psc;  
		TIM_TimeBaseStructure.TIM_Period = arr;  
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  
		//使用编码器模式3  都计数
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//MODE 3
		// 定时器选择，模式选择（模式1，单相计数，模式2，两相计数，模式3，高精度两相计数），通道1捕捉极性，通道2捕捉极性。
		TIM_ICStructInit(&TIM_ICInitStructure);   //chnnel1  TIM_ICPolarity_Rising
		TIM_ICInitStructure.TIM_ICFilter =6;//滤波器 
		TIM_ICInit(TIM4, &TIM_ICInitStructure);  
		
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);  
///////

		 NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM2中断
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		 NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

			
		 TIM_Cmd(TIM4, ENABLE); 
		 TIM4->CNT = 10000;    //初始化计数值为10000  ，正转CNT++ 反转CNT--
}

