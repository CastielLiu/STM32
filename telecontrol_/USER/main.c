#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24l01.h" 	 
#include "spi.h"
#include "fun.h"
#include "wdg.h"


u8 		TxData_Buf[32];
uint8_t is_bar_medium(uint16_t temp);
int main(void)
{	 
	u16 	ADC_data[8];
	u8 runMode =1 ;
	u8 readyToSetMode = 0;
	int16_t temp1=0 ,temp2=0,temp3=0,temp4=0;
	u8 status = 0;		
	uint16_t i=0 ,count =0;	    
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();
	KEY_Init();
	
	NRF24L01_Init();    		//��ʼ��NRF24L01 
	
	Initial_DMA_ADC1(&ADC_data[0]);
	ADC1_Config();  //initial    �ȳ�ʼ��DMA ������DMA���䣬�ٳ�ʼ��ADC�Ϳ������ת��
 
	delay_ms(100);
	
	while(NRF24L01_Check())
	{
			delay_ms(200);
			LED_OFF();
			IR_LED_OFF();

			delay_ms(200);
			LED_ON();
			IR_LED_ON();

	}
	IR_LED_OFF();
	delay_ms(10);
	NRF24L01_TX_Mode();
	
	//Bzz_ON();
	//delay_ms(100);
	//Bzz_OFF();
		
	IWDG_Init(72,50000);
	
	while (1)
	{
			delay_ms(10);
		//	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	 //2018.9.1 commit

			//ң������ص�ѹ����3.6V����
			if (BATTER_Volt < 2233)
			{
				;
//					Bzz_ON();
//					delay_ms(300);
//					Bzz_OFF();
//					delay_ms(200);
			}
			//������⣬mode�л�
			if(MODE_KEY ==0)
			{
				delay_ms(5);//����
				if(MODE_KEY ==0)
				{
					runMode = !runMode;
					
					//printf("runMode = %d\t",runMode);
				}
				while(MODE_KEY ==0){IWDG_Feed();	delay_ms(30);} ;//����⵽����ʱ����ѭ��ʱ��䳤��ι��ʱ�䱻�ͺ󣬽����¸�λ
																	//����ڰ����ɿ�֮ǰѭ��ι����
			}
			
			if(runMode ==1 )
				IR_LED_ON();
			else
				IR_LED_OFF();
				
			//0-1 �����٣�2-3 ������  4-5 ת�����֣�
			temp1 = leftHand_U_D + (rotarySwitch_1 -2048)/4;   //���ڷ�Χ-512 ~ 512
			if(temp1 > 4096) temp1 = 4096;
			else if(temp1 < 0) temp1 =0;
			TxData_Buf[0]=temp1 >> 8;
			TxData_Buf[1]=temp1 & 0xFF;
			
			temp2 = (4096-rightHand_U_D) + (rotarySwitch_3 -2048)/4;   //���ڷ�Χ-512 ~ 512
			if(temp2 > 4096) temp2 = 4096;
			else if(temp2 < 0) temp2 =0;
			TxData_Buf[2]=temp2 >> 8;
			TxData_Buf[3]=temp2 & 0xFF;
			
			temp3 = (4096 - rightHand_L_R) + (rotarySwitch_2 -2048)/4;   //���ڷ�Χ-512 ~ 512
			if(temp3 > 4096) temp3 = 4096;
			else if(temp3 < 0) temp3 =0;
			TxData_Buf[4]=temp3 >> 8;
			TxData_Buf[5]=temp3 & 0xFF;
			
			temp4 = (4096 - leftHand_L_R);
			if(temp4 > 4096) temp4 = 4096;
			else if(temp4 < 0) temp4 =0;
			TxData_Buf[6]=temp4 >> 8;
			TxData_Buf[7]=temp4 & 0xFF;
			
			if(temp1<1000&&temp2<1000&&temp3<1000&&temp4<1000)
				readyToSetMode =1;
			if(readyToSetMode && is_bar_medium(temp1) &&is_bar_medium(temp2) &&is_bar_medium(temp3) &&is_bar_medium(temp4))
			{
				runMode =1;
				readyToSetMode =0;
			}
			
			TxData_Buf[8] &= 0xfe; //ĩλ����
			TxData_Buf[8] |= (runMode&0x01);
			
			
			NRF24L01_TxPacket(TxData_Buf);  //send
			
			count ++;
			if(count%20 ==0 )
				LED1 = !LED1;
		
			IWDG_Feed();//ι��		

		}

	return 0;
		
}
 
uint8_t is_bar_medium(uint16_t temp)
{
	if(temp<2048+100 && temp>2048-100)
		return 1;
	return 0;
}


