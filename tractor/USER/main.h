#ifndef MAIN_H_
#define MAIN_H_

#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "wdg.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
#include "exti.h"
#include "direction.h"
#include "can.h"
#include "navigation2.h"
#include "beep.h"
#include "lcd.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "capture.h"
#include "dma.h"

#define YKMode 1

#if(YKMode==1)
	#include "24l01.h"
	#include "wireless_data.h"
#endif
u8 gps_data_buf[105];

gps_data_t *gpsPtr = (gps_data_t *)&(gps_data_buf[1]);

void system_init()
{
	delay_init();	    	   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,CAN_Mode_Normal); //250kbps
	
	uart1_init(115200);	
	//uart2_init(115200); 
	uart3_init(115200); 
	Adc_Init();
 	LED_Init();
	BEEP_Init();
	//IWDG_Init(0,700);   //60ms
 	TIM3_PWM_Init(4999, 72-1); //200HZ
	TIM3->CCR2 = 2499;
	car_control_init(); //��ʼ��ת����ת��ʹ������
	
	EXTIX_Init();
	TIM5_Init(10000,72-1);	// 10ms��� ���ڼ�ʱ 
	
	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)gps_data_buf,105);  //DMAͨ���� ��ֹ��ַ�Լ���������������
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE); //ʹ�ܴ���3��DMA����
	MYDMA_Enable(DMA1_Channel3);//��ʼһ��DMA���� Ŀ���ǽ�����ɺ󴥷������ж�
								//Ȼ���ٿ����ж����ٴο�������
	
	ADC_DMA_Config(); //ADC DMA��������
	
	LCD_Init();
	printf("��ʼ�����...\r\n");
}

#endif 