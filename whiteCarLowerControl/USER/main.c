#include "main.h"

//KEY0  						PE4 
//KEY1  						PE3 ��ʼ��ȡ�����˼�ʻ
//PWM�ٶȿ���				PA6   TIM3_CH1
//PWMת�����				PA7  	TIM3_CH2
//��������������    PB6-7 TIM4_CH12
//TIM5 ���ڼ�ʱ  					TIM5


//spi      PB12 PB13 PB14 PB15
//24L01    PG6 PG7 PG8
//LCD     PB0 PD0 PD1 PD4 PD5 PD8 PD9 PD10 PD14 PD15 
//				PE7-15 PG0 PG12


//Ԥװ��ֵ�Ĵ���     TIMx->ARR  
//�ȽϼĴ���         TIMx->CCR2 
//CAN				StdId =0x12   ExtId=0x12;


u8 g_driveMode = TELECONTROL_MODE; //UPPER_CONTROL_MODE
u8 g_ykSafty_num = 0;
char g_driveModeName[20];
carControlData_t	g_carControlMsg = {2048,2048,2048}; //��ʼ�����٣�ת���Ϊ0

int main(void)
{	
	u8 wirelessBuf[32];	 //ң�������ݽ��ջ���	

	const float MaxDriveSpeedRateLimit = 0.2;
	const float MaxReverseSpeedRateLimit = 0.6;
 
	LED0 = 0;
	system_init();
///////////////////////////  ң�������	

	NRF24L01_Init();    		//��ʼ��NRF24L01 
	while(NRF24L01_Check())
	{
 		delay_ms(200);
		LED1 =!LED1;
		LED0=!LED0;
	}
	NRF24L01_RX_Mode();	

	
	POINT_COLOR=BLACK;//��������Ϊ��ɫ
	LCD_ShowString(30,20 ,48 ,16,16,"mode :");
	strcpy(g_driveModeName,"Telecontrol Mode");

  while(1)//25ms
	{
 
		if(g_driveMode == TELECONTROL_MODE)//�����˼�ʻ״̬����ʹ��ң����
		{
			if(NRF24L01_RxPacket(wirelessBuf)==0 && (wirelessBuf[31]&0xff) ==1)//�յ���Ϣ  ĩλУ��
			{
				g_ykSafty_num = 0;//ÿ�յ�һ����Ϣ ����ֵ����
				g_carControlMsg = dataConvert(wirelessBuf);

			}
			else
				continue;

		}
		else//���˼�ʻģʽ
		{
			//���ڽ�����λ��ָ����н����õ�������Ϣ��g_carControlMsg
		}
		
		//speed 0-4096
		if(g_carControlMsg.speed_l>2050)	
			TIM3->CCR1 = 1500+(g_carControlMsg.speed_l-2048)*500*MaxDriveSpeedRateLimit/4096;
		else if(g_carControlMsg.speed_l <2045)
			TIM3->CCR1 = 1500+(g_carControlMsg.speed_l-2048)*500*MaxReverseSpeedRateLimit/4096;
		TIM3->CCR2 = 1500+(g_carControlMsg.angle-2048)*500/4096;//angle
					
		POINT_COLOR=RED;//��������Ϊ��ɫ 
		LCD_ShowString(78,20,96,16,16,g_driveModeName);	
		
		delay_ms(10);
	}	 
 }
