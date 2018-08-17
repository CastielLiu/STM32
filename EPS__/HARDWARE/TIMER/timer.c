#include "timer.h"
#include "usart.h"
#include "can.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��Ӣ STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

u8 engine_status[]={0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00};
u8 engine_speed[]= {0x00,0x03,0xE8,0x00,0x00,0x00,0x00,0x00};
u8 car_speed[]=    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//TIM2 ���ڼ�ʱ
void TIM2_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update ,   //TIM �����ж�Դ
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����					 
}

void TIM2_IRQHandler(void)   //TIM2�ж�
{
	static u16 cnt = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
	{
		Can_Send_Msg(0x280,engine_status,8);//������״̬ID 10ms
		
		Can_Send_Msg(0x1A0,engine_speed,8);//������ת��ID  10ms
		if(cnt%2==0)
			Can_Send_Msg(0x318,car_speed,8);//����ID 20ms
		cnt++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
	}
}




//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
TIM_OCInitTypeDef  TIM_OCInitStructure;
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA	| RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5                                                                       	 //����TIM3��CH2�����PWMͨ����LED��ʾ
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  //���ÿ�©
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
 

}

//��ʱ��5ͨ��1���벶������

void TIM5_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 |GPIO_Pin_1;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_ResetBits(GPIOA,GPIO_Pin_0);						 //PA0 ����
	
	//��ʼ����ʱ��5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1 ; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2 ; 
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM5,TIM_IT_CC1 | TIM_IT_CC2 ,ENABLE);//����CC1IE�����ж�	
	
   	TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
   
}

float ch1_duty_cycle =0.0 ,ch2_duty_cycle =0.0; 
u16 ch1_rising_cnt=0 , ch2_rising_cnt = 0; // ����������ʱ�ļ���
u16 ch1_falling_cnt =0 ,ch2_falling_cnt = 0;// �����½���ʱ�ļ���
u8 ch1_capture_mode = 0 ,ch2_capture_mode=0;//�����ز���0  �½��ز���1
u16 ch1_high_level_time = 0 ,ch2_high_level_time = 0;
u16 ch1_low_level_time = 0  ,ch2_low_level_time =0;

//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//ͨ��1�����ж�
	{
		if(ch1_capture_mode==0)//����������
		{
			ch1_rising_cnt = TIM5->CCR1;
			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);//�л�Ϊ�½��ز���
			ch1_capture_mode = 1;
			
			if(ch1_rising_cnt>ch1_falling_cnt)
				ch1_low_level_time = ch1_rising_cnt - ch1_falling_cnt;//������ʱ��  - �½���ʱ��=�͵�ƽʱ��
			else
				ch1_low_level_time = 65535-ch1_falling_cnt+ch1_rising_cnt;
		}
		else //�����½���
		{
			ch1_falling_cnt = TIM5->CCR1;
			TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);//�л�Ϊ�����ز���	
			ch1_capture_mode = 0;
			
			if(ch1_rising_cnt < ch1_falling_cnt)
				ch1_high_level_time =  ch1_falling_cnt - ch1_rising_cnt;//�½���ʱ�� - ������ʱ��=�ߵ�ƽʱ��
			else
				ch1_high_level_time = 65535 + ch1_falling_cnt - ch1_rising_cnt;			
		}	
		if(ch1_capture_mode==1)//��һ�β����½���
		{
			ch1_duty_cycle = 1.0*ch1_low_level_time/(ch1_high_level_time + ch1_low_level_time)*100;
			//printf("duty_cycle = %f%%\r\n",duty_cycle*100);
			
		}
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1); //����жϱ�־λ
	}
	
	else if (TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)//ͨ��2�����ж�
	{
		if(ch2_capture_mode==0)//����������
		{
			ch2_rising_cnt = TIM5->CCR2;
			TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);//�л�Ϊ�½��ز���
			ch2_capture_mode = 1;
			
			if(ch2_rising_cnt>ch2_falling_cnt)
				ch2_low_level_time = ch2_rising_cnt - ch2_falling_cnt;//������ʱ��  - �½���ʱ��=�͵�ƽʱ��
			else
				ch2_low_level_time = 65535-ch2_falling_cnt+ch2_rising_cnt;
		}
		else //�����½���
		{
			ch2_falling_cnt = TIM5->CCR2;
			TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising);//�л�Ϊ�����ز���	
			ch2_capture_mode = 0;
			
			if(ch2_rising_cnt < ch2_falling_cnt)
				ch2_high_level_time =  ch2_falling_cnt - ch2_rising_cnt;//�½���ʱ�� - ������ʱ��=�ߵ�ƽʱ��
			else
				ch2_high_level_time = 65535 + ch2_falling_cnt - ch2_rising_cnt;			
		}	
		if(ch2_capture_mode==1)//��һ�β����½���
		{
			ch2_duty_cycle = 1.0*ch2_low_level_time/(ch2_high_level_time + ch2_low_level_time)*100;
			//printf("duty_cycle = %f%%\r\n",duty_cycle*100);
			
		}
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC2); //����жϱ�־λ
	}

 
}
