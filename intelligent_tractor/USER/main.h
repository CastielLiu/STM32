#ifndef MAIN_H_
#define MAIN_H_

#include "global_params.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "wdg.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
#include "exti.h"
#include "can.h"
#include "beep.h"
#include "lcd.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"
#include "capture.h"
#include "dma.h"
#include "steering_motor.h"

void system_init()
{
	delay_init();	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,CAN_Mode_Normal); //250kbps
	
	uart1_init(115200);	
	uart2_init(115200);	//�ӷ�����
	 
	//Adc_Init();
	
 	LED_Init();
	BEEP_Init();
	
	EXTIX_Init();
	TIM5_Init(10000,72-1);	// 10ms��� ���ڼ�ʱ 
	
	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)gps_data_buf,105);  //DMAͨ���� ��ֹ��ַ�Լ���������������

	MYDMA_Enable(DMA1_Channel3);//��ʼһ��DMA���� Ŀ���ǽ�����ɺ󴥷������ж�
								//Ȼ���ڿ����ж����ٴο�������
								//DMA������һֱ��������  һ���������̽���
	uart3_init(115200);
	
	//ADC_DMA_Config(); //ADC DMA��������
	
	LCD_Init();
	
	steeringEnable();
	
	printf("��ʼ�����...\r\n");
}


#endif 
