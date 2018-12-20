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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
   	EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��   ��Ҫ

   //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOE.4	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


   //GPIOA.0	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
}




static u8 point_seq_=0;

//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)
{
	char show_lon[12];
	char show_lat[12];
	float record_dis_threshold = 1.5;//m
	float	dis ;
	
	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
	
	if(point_seq_ ==0 )
		dis= record_dis_threshold +1.5;//��������Ϊ����record_dis_threshold��ֵ
	else
		dis = point2point_dis(target_point[point_seq_-1],gps_sphere_now);

	//printf("\r\n%.8f\t%.8f\t\t%.8f\t%.8f\tpoint_seq_=%d\r\n",target_point[point_seq_].lat*180/pi,target_point[point_seq_].lon*180/pi,gps_sphere_now.lat*180/pi,gps_sphere_now.lon*180/pi,point_seq_);
	delay_ms(10);//����
	if(gps_sphere_now.lat==0.0 || gps_sphere_now.lon == 0.0)
	{
		LCD_ShowString(LCD_LU_X+25*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*1+(point_seq_)*LCD_FOND_SIZE*2,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"data error!");
		return;
	}
	if(WK_UP==1 && start_driverless_flag ==0)	 	 //WK_UP���� �� δ�������˼�ʻ״̬����ֹ���˼�ʻʱ�󴥰�������Ŀ������¼�¼
	{	
		//���μ�¼Ŀ���С��record_dis_threshold  mʱ�൱����һ�������¼�¼ 	
		if(dis > record_dis_threshold )//��һ��Ŀ�����Ӧ��¼��0λ��  
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
	//	printf("��ǰ��γ�ȣ�%.7f\t%.7f\r\n",(gps_msg->lat),(gps_msg->lon));
	//	printf("���þ�γ�ȣ�%.7f\t%.7f\r\n",endlat[0],endlon[0]);
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
 

//�ⲿ�ж�3�������

void EXTI3_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY1==0)	 //����KEY1
	{				 
		start_driverless_flag =!start_driverless_flag ;
		if(start_driverless_flag==1)
		{
			actual_path_vertwx_num = point_seq_;  //right
			//point_seq_ =0; //seq ���㣬�´μ�¼��ͷ��ʼ
												//18.7.11 ��Ҫ���㣬�´μ�¼��λ��Ƭ��  
												//			Ŀ���ǣ�ȡ�����˼�ʻ֮��  �ٴΰ��¼������˼�ʻ
			TIM3->CCR1 = 1531;  //�趨�̶��ٶ�
			//printf("start_driverless_flag = 1 \r\n");
		}
		else
		{
			TIM3->CCR1 = 1499; //���˼�ʻȡ�����ٶ�����
		}
		while(KEY1==0) ;
	}		 
	EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
}

//temp_
void EXTI4_IRQHandler(void)
{
//	static u16 val_ = 1499;
//	delay_ms(10);//����
//	if(KEY0 ==0 && TIM3->CCR2 <1998)
//	{
//		//TIM3->CCR1 = 1530;
//		val_+=50;
//		TIM3->CCR2 = val_;
//	}

//	while(KEY0==0) ;
	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE4�ϵ��жϱ�־λ  
}
 
