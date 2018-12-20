#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"
#include "global_params.h"
#include "string.h"
#include "lcd.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序
void EXTIX_Init(void)
{
 
   	EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟   重要

   //GPIOE.3	  中断线以及中断初始化配置 下降沿触发 //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

   //GPIOE.4	  中断线以及中断初始化配置  下降沿触发	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


   //GPIOA.0	  中断线以及中断初始化配置 上升沿触发 PA0  WK_UP
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键KEY1所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
 
}




static u8 point_seq_=0;

//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	char show_lon[12];
	char show_lat[12];
	float record_dis_threshold = 1.5;//m
	float	dis ;
	
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
	
	if(point_seq_ ==0 )
		dis= record_dis_threshold +1.5;//距离设置为大于record_dis_threshold的值
	else
		dis = point2point_dis(target_point[point_seq_-1],gps_sphere_now);

	//printf("\r\n%.8f\t%.8f\t\t%.8f\t%.8f\tpoint_seq_=%d\r\n",target_point[point_seq_].lat*180/pi,target_point[point_seq_].lon*180/pi,gps_sphere_now.lat*180/pi,gps_sphere_now.lon*180/pi,point_seq_);
	delay_ms(10);//消抖
	if(gps_sphere_now.lat==0.0 || gps_sphere_now.lon == 0.0)
	{
		LCD_ShowString(LCD_LU_X+25*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*1+(point_seq_)*LCD_FOND_SIZE*2,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"data error!");
		return;
	}
	if(WK_UP==1 && start_driverless_flag ==0)	 	 //WK_UP按键 且 未处于无人驾驶状态，防止无人驾驶时误触按键导致目标点重新记录
	{	
		//两次记录目标点小于record_dis_threshold  m时相当于上一个点重新记录 	
		if(dis > record_dis_threshold )//第一个目标点理应记录到0位置  
		{
			target_point[point_seq_] = gps_sphere_now;
			sprintf(show_lon,"%03.7f",target_point[point_seq_].lon*180/3.1415926);
			sprintf(show_lat,"%03.7f",target_point[point_seq_].lat*180/3.1415926);
		//clear
			LCD_ShowString(LCD_LU_X+25*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*1+(point_seq_)*LCD_FOND_SIZE*2,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"           ");

			LCD_ShowString(LCD_LU_X+6*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*1+(point_seq_)*LCD_FOND_SIZE*2,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_lon);
			LCD_ShowString(LCD_LU_X+6*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*2+(point_seq_)*LCD_FOND_SIZE*2,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_lat);
		
			point_seq_++;	
			printf("%f\t%f\tdis=%f\r\n",gps_sphere_now.lon*180/3.1415926,gps_sphere_now.lat*180/3.1415926,dis);
		}
	//	printf("当前经纬度：%.7f\t%.7f\r\n",(gps_msg->lat),(gps_msg->lon));
	//	printf("设置经纬度：%.7f\t%.7f\r\n",endlat[0],endlon[0]);
		BEEP = 1;
		delay_ms(300);
		BEEP = 0;
		delay_ms(300);
		BEEP = 1 ;
		delay_ms(300);
		BEEP = 0;
		
		while(WK_UP==1) ;
	}
			//printf("KEY_UP\r\n");
	 
}
 

//外部中断3服务程序

void EXTI3_IRQHandler(void)
{
	delay_ms(10);//消抖
	if(KEY1==0)	 //按键KEY1
	{				 
		start_driverless_flag =!start_driverless_flag ;
		if(start_driverless_flag==1)
		{
			actual_path_vertwx_num = point_seq_;  //right
			//point_seq_ =0; //seq 清零，下次记录从头开始
												//18.7.11 不要清零，下次记录复位单片机  
												//			目的是：取消无人驾驶之后  再次按下继续无人驾驶
			TIM3->CCR1 = 1531;  //设定固定速度
			//printf("start_driverless_flag = 1 \r\n");
		}
		else
		{
			TIM3->CCR1 = 1499; //无人驾驶取消，速度置零
		}
		while(KEY1==0) ;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}

//temp_
void EXTI4_IRQHandler(void)
{
//	static u16 val_ = 1499;
//	delay_ms(10);//消抖
//	if(KEY0 ==0 && TIM3->CCR2 <1998)
//	{
//		//TIM3->CCR1 = 1530;
//		val_+=50;
//		TIM3->CCR2 = val_;
//	}

//	while(KEY0==0) ;
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}
 
