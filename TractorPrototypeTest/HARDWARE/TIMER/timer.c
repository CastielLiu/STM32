#include "timer.h"
#include "led.h"
#include "usart.h"
#include "string.h"
#include "can.h"
#include "delay.h"
#include "dma.h"
#include "global_params.h"

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

 
void TIM5_IRQHandler(void)
{ 
	u8 send_buf_0x4C0[8] = {0};
	u8 send_buf_0x4C1[8] = {0};
	u8 send_buf_0x4C2[8] = {0};
	u8 send_buf_0x4C3[8] = {0};
	static uint32_t count=0;
	
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET) //更新中断
	{
		if(count%5==0) //50ms
		{
//			g_u32_lat = 311234567;
//			g_u32_lon = 1181234567;
			send_buf_0x4C0[0] = (g_u32_lat>>24)&0xff;
			send_buf_0x4C0[1] = (g_u32_lat>>16)&0xff;
			send_buf_0x4C0[2] = (g_u32_lat>>8)&0xff;
			send_buf_0x4C0[3] = (g_u32_lat)&0xff;
			
			send_buf_0x4C0[4] = (g_u32_lon>>24)&0xff;
			send_buf_0x4C0[5] = (g_u32_lon>>16)&0xff;
			send_buf_0x4C0[6] = (g_u32_lon>>8)&0xff;
			send_buf_0x4C0[7] = (g_u32_lon)&0xff;
			
			Can_Send_Msg(0x4C0,send_buf_0x4C0,8); 
			//printf("lonLat\r\n");
		}
		else if(count%5==1)
		{
			memcpy(send_buf_0x4C1,&g_int_height,4);
			Can_Send_Msg(0x4C1,send_buf_0x4C1,8);
		}
		else if(count%5==2)
		{		
			g_u16_yaw = 1234;
			
			send_buf_0x4C2[0] = (g_s16_speed>>8)&0xff;
			send_buf_0x4C2[1] = g_s16_speed &0xff;
			
			send_buf_0x4C2[2] = (g_u16_yaw>>8)&0xff;
			send_buf_0x4C2[3] = g_u16_yaw &0xff;
			
			send_buf_0x4C2[4] = g_u8_gps_status ;
			send_buf_0x4C2[5] = g_u8_gps_satellites ;
			Can_Send_Msg(0x4C2,send_buf_0x4C2,8);
		}
		else if(count%5==3)
		{
			if(g_s16_steer_angle > 0)
			{
				send_buf_0x4C3[0] = 2;//左
				send_buf_0x4C3[1] = (g_s16_steer_angle>>8)&0xff;
				send_buf_0x4C3[2] = g_s16_steer_angle & 0xff;
			}
			else
			{
				send_buf_0x4C3[0] = 1;//右偏
				send_buf_0x4C3[1] = (-g_s16_steer_angle>>8)&0xff;
				send_buf_0x4C3[2] = -g_s16_steer_angle & 0xff;
			}
			
			
			Can_Send_Msg(0x4C3,send_buf_0x4C3,8);
		}
		
//			printf("g_s16_steer_angle=%d\r\n",g_s16_steer_angle);
//			printf("g_u32_lon=%d\r\n",g_u32_lon);
//			printf("g_u32_lat=%d\r\n",g_u32_lat);
//			printf("g_int_height=%d\r\n",g_int_height);
//			printf("g_s16_speed=%d\r\n",g_s16_speed);
//			printf("g_u16_yaw=%d\r\n",g_u16_yaw);
//			printf("g_u8_gps_status=%d\r\n",g_u8_gps_status);
//			printf("g_u8_gps_satellites=%d\r\n\r\n",g_u8_gps_satellites);
	}
	count ++;
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //清除中断标志位
}

 
