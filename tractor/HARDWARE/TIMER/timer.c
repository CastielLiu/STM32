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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM5ʱ��
	
	//��ʼ����ʱ��5 TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM5,TIM_IT_Update , ENABLE);//��������ж�
	
  TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
}


u16  send_steer_angle;   //ԭʼ�Ƕ�����90��  15�ֽ�Ϊ����0��1��
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
	
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET) //�����ж�
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
				send_buf_0x4C3[0] = 2;//��
			else
				send_buf_0x4C3[0] = 1;//��ƫ
			send_steer_angle = (send_steer_angle & 0x7fff)*10/9; //ǰһʱ�̱��Ŵ���90��  ���ڵ���Ϊ100��
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
	ADC_DMA_Enable();//����һ��ADCת����DMA����
	//printf("%d\t%d\r\n",ADC_temp_Buf[0],ADC_temp_Buf[1]);
	//*(u32 *)ADC_Value_Arr[count%ADC_AVERAGE_CNT]= *(u32 *)ADC_temp_Buf; //��������ɵ����ݴ���µ����飬�Ա�����ƽ��ֵ
		//count%ADC_AVERAGE_CNT ���������ܸ���ȡ�࣬Ŀ���ǽ�����ѭ���������
	ADC_Value_Arr[count%ADC_AVERAGE_CNT][0] = ADC_temp_Buf[0];
	ADC_Value_Arr[count%ADC_AVERAGE_CNT][1] = ADC_temp_Buf[1];
	count ++;
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //����жϱ�־λ
}


//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��

TIM_OCInitTypeDef  TIM_OCInitStructure;

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;  //����ַ�����ܶ�д  �����Ӱ���ⲿ�жϣ�
	
	memset(&GPIO_InitStructure,sizeof(GPIO_InitStructure),0);
	memset(&TIM_TimeBaseStructure,sizeof(TIM_TimeBaseStructure),0);
	memset(&TIM_OCInitStructure,sizeof(TIM_OCInitStructure),0);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO , ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��  //RCC_APB2Periph_AFIO
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5 ������ӳ�䣺ͨ��1��2��ȫ��ӳ�䣬ȫ��ͨ��   
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	PA7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6; //TIM3_CH2 TIM3_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); 
	
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3

}
