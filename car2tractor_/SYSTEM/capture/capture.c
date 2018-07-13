#include "capture.h"
#include "usart.h"
#include "string.h"




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

