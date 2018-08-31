#include "timer.h"
#include "led.h"
#include "usart.h"
#include "string.h"
#include "can.h"
#include "delay.h"
#include "dma.h"



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


u16  send_steer_angle;   //原始角度扩大90倍  15字节为方向，0左，1右
extern int send_lon,send_lat,send_height;
extern u16 send_speed,send_yaw;
extern u8 send_gps_status,send_satellites;   //use to can send msg 



u8 send_buf_0x4C0[8];
u8 send_buf_0x4C1[8];
u8 send_buf_0x4C2[8];
u8 send_buf_0x4C3[8];

extern u16 ADC_temp_Buf[2];
u16 ADC_Value_Arr[ADC_AVERAGE_CNT][2];

void TIM5_IRQHandler(void)
{ 
	static uint32_t count=0;
	
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET) //更新中断
	{
		if(count%5==0) //50ms
		{
			memcpy(send_buf_0x4C0,&send_lat,4);
			memcpy(&send_buf_0x4C0[4],&send_lon,4);
			Can_Send_Msg(0x4C0,send_buf_0x4C0,8);
			
			memcpy(send_buf_0x4C1,&send_height,4);
			Can_Send_Msg(0x4C1,send_buf_0x4C1,4);
			
			memcpy(send_buf_0x4C2,&send_speed,2);
			memcpy(&send_buf_0x4C2[2],&send_yaw,2);
			memcpy(&send_buf_0x4C2[4],&send_gps_status,1);
			memcpy(&send_buf_0x4C2[5],&send_satellites,1);
			Can_Send_Msg(0x4C2,send_buf_0x4C2,6);
			
			if((send_steer_angle & 0x8000)==0)
				send_buf_0x4C3[0] = 2;//左
			else
				send_buf_0x4C3[0] = 1;//右偏
			send_steer_angle = (send_steer_angle & 0x7fff)*10/9; //前一时刻被放大了90倍  现在调整为100倍
			memcpy(&send_buf_0x4C3[1],&send_steer_angle,2);
			delay_ms(1);
			Can_Send_Msg(0x4C3,send_buf_0x4C3,3);
		}
		
//			printf("send_steer_angle=%d\r\n",send_steer_angle);
//			printf("send_lon=%d\r\n",send_lon);
//			printf("send_lat=%d\r\n",send_lat);
//			printf("send_height=%d\r\n",send_height);
//			printf("send_speed=%d\r\n",send_speed);
//			printf("send_yaw=%d\r\n",send_yaw);
//			printf("send_gps_status=%d\r\n",send_gps_status);
//			printf("send_satellites=%d\r\n\r\n",send_satellites);
	}
	ADC_DMA_Enable();//开启一次ADC转换和DMA传输
	//printf("%d\t%d\r\n",ADC_temp_Buf[0],ADC_temp_Buf[1]);
	//*(u32 *)ADC_Value_Arr[count%ADC_AVERAGE_CNT]= *(u32 *)ADC_temp_Buf; //将传输完成的数据存进新的数组，以便于求平均值
		//count%ADC_AVERAGE_CNT 计数器对总个数取余，目的是将数据循环存进数组
	ADC_Value_Arr[count%ADC_AVERAGE_CNT][0] = ADC_temp_Buf[0];
	ADC_Value_Arr[count%ADC_AVERAGE_CNT][1] = ADC_temp_Buf[1];
	count ++;
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //清除中断标志位
}


//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

TIM_OCInitTypeDef  TIM_OCInitStructure;

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;  //这块地址好像不能读写  否则会影响外部中断！
	
	memset(&GPIO_InitStructure,sizeof(GPIO_InitStructure),0);
	memset(&TIM_TimeBaseStructure,sizeof(TIM_TimeBaseStructure),0);
	memset(&TIM_OCInitStructure,sizeof(TIM_OCInitStructure),0);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟  //RCC_APB2Periph_AFIO
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5 部分重映射：通道1和2，全部映射，全部通道   
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	PA7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6; //TIM3_CH2 TIM3_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); 
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3

}
