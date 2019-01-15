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
#include "function.h"



void system_init()
{
 
	uint16_t _BKP_DRx;
	
	delay_init();	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	//CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,CAN_Mode_Normal); //250kbps
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal); //500kbps
	uart1_init(115200);	
	uart2_init(115200);	//�ӷ�����
	 
	//Adc_Init();
	
 	LED_Init();
	BEEP_Init();
	
	EXTIX_Init();
	TIM5_Init(10000,72-1);	// 10ms��� ���ڼ�ʱ 
	
	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)g_gps_data_buf,DMA_DATA_NUM);  //DMAͨ���� ��ֹ��ַ�Լ���������������

	MYDMA_Enable(DMA1_Channel3);//��ʼһ��DMA���� Ŀ���ǽ�����ɺ󴥷������ж�
								//Ȼ���ڿ����ж����ٴο�������
								//DMA������һֱ��������  һ���������̽���
	uart3_init(115200);
	
	//ADC_DMA_Config(); //ADC DMA��������
	
	LCD_Init();
	
	steeringEnable();
	
	/*ʹ�ú�����洢����  ����BKP->DR*  */
	RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ��
    RCC->APB1ENR|=1<<27;//ʹ�ܱ���ʱ��
	PWR->CR|=1<<8;    //ȡ��������д����
	
	
/*	
	for(_BKP_DRx = BKP_DR1; _BKP_DRx!=BKP_DR42; _BKP_DRx +=4 )
	{
		printf("%x\r\n",BKP_ReadBackupRegister(_BKP_DRx));
	}
*/


	while(BKP_ReadBackupRegister(BKP_DR2)!=0x00AA)
	{
		LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*5 ,32*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"please verify the angle sensor!");
		delay_ms(500);
	}
	LCD_Clear(WHITE);
	
	g_AngleSensorAdValueOffset = BKP_ReadBackupRegister(BKP_DR3);
	g_AngleSensorMaxAngle = BKP_ReadBackupRegister(BKP_DR5) & 0x7fff;//ȥ�����λ
	
	g_AngleSensorMaxAdValue =  BKP_ReadBackupRegister(BKP_DR4);
	if(BKP_ReadBackupRegister(BKP_DR5) >>15)
		g_roadWheelAngle_dir = 1;
	else
		g_roadWheelAngle_dir = -1;
	
	//printf("offset=%d\tmaxAngle=%d\tmaxAdValue=%d\tdir=%d\r\n", \
				g_AngleSensorAdValueOffset,g_AngleSensorMaxAngle,g_AngleSensorMaxAdValue,g_roadWheelAngle_dir);	

	//printf("��ʼ�����...\r\n");
}


#endif 
