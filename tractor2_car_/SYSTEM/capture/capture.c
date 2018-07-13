#include "capture.h"
#include "usart.h"
#include "string.h"


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


extern speedMsg_t g_speedMsg;
extern u8 g_ykSafty_num ; //ң���ź��쳣ʱ����ͣ
extern u8 start_driverless_flag;
void TIM5_IRQHandler(void)
{ 
		if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET) //�����ж�
		{
			g_speedMsg.pulseNumPer50ms = TIM4->CNT - 10000;
		//	printf("pulseNumPer50ms=%d\r\n",g_speedMsg.pulseNumPer50ms );
			g_speedMsg.totalPulseNum +=g_speedMsg.pulseNumPer50ms;
			g_speedMsg.motorSpeed = g_speedMsg.pulseNumPer50ms *1.0/g_speedMsg.encoderLineNum /4./50.;
			TIM4->CNT = 10000;
			TIM4->CNT = 10000;
			if(start_driverless_flag ==0)//ң��ģʽ��
			{
				g_ykSafty_num++;
				if(g_ykSafty_num >8)//50ms*8
					TIM3->CCR1 = 1499; //speed = 0;
			}
		}
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //����жϱ�־λ
}
// TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
// TIM_ICInitTypeDef TIM_ICInitStructure;

void TIM4_Capture_Init(uint16_t arr,uint16_t psc)
{  
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //ò�Ʋ��������ⶨ���������ṹ�壬����Ӱ���ⲿ�ж�
	  TIM_ICInitTypeDef TIM_ICInitStructure;       
	  GPIO_InitTypeDef GPIO_InitStructure1;  
	  NVIC_InitTypeDef NVIC_InitStructure;
	
		memset(&TIM_TimeBaseStructure,sizeof(TIM_TimeBaseStructure),0);
		memset(&TIM_ICInitStructure,sizeof(TIM_ICInitStructure),0);
		memset(&GPIO_InitStructure1,sizeof(GPIO_InitStructure1),0);
		memset(&NVIC_InitStructure,sizeof(NVIC_InitStructure),0);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);   

		//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);  //������ӳ�� CH1->PA15 CH2->PB3
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
		//ʹ�ñ�����ģʽ3  ������
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);//MODE 3
		// ��ʱ��ѡ��ģʽѡ��ģʽ1�����������ģʽ2�����������ģʽ3���߾��������������ͨ��1��׽���ԣ�ͨ��2��׽���ԡ�
		TIM_ICStructInit(&TIM_ICInitStructure);   //chnnel1  TIM_ICPolarity_Rising
		TIM_ICInitStructure.TIM_ICFilter =6;//�˲��� 
		TIM_ICInit(TIM4, &TIM_ICInitStructure);  
		
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);  
///////

		 NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM2�ж�
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
		 NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

			
		 TIM_Cmd(TIM4, ENABLE); 
		 TIM4->CNT = 10000;    //��ʼ������ֵΪ10000  ����תCNT++ ��תCNT--
}

