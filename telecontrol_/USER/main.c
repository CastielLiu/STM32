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

#define YK_MODE  1

u8 		TxData_Buf[32];

int main(void)
{	 
	u16 	ADC_data[8];
	u8 runMode =1 ;//Ĭ��ң��ģʽ
	int16_t temp=0;
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
	delay_ms(10);
	NRF24L01_TX_Mode();
	
	Bzz_ON();
	delay_ms(100);
	Bzz_OFF();
	
//	printf("��ʼ����ɡ�����\r\n");
	if(runMode)
		TxData_Buf[31] |=1;
		
	IWDG_Init(72,50000);
	
	while (1)
	{
			delay_ms(30);
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
				delay_ms(10);//����
				if(MODE_KEY ==0)
				{
					runMode = !runMode;
					TxData_Buf[31] &= 0xfe; //ĩλ����
					TxData_Buf[31] |= runMode;
					//printf("runMode = %d\t",runMode);
				}
				while(MODE_KEY ==0){IWDG_Feed();	delay_ms(30);} ;//����⵽����ʱ����ѭ��ʱ��䳤��ι��ʱ�䱻�ͺ󣬽����¸�λ
																	//����ڰ����ɿ�֮ǰѭ��ι����
			}
			
			if(runMode ==1 )
			{
				//0-1 �����٣�2-3 ������  4-5 ת�����֣�
				temp = leftHand_U_D + (rotarySwitch_1 -2048)/4;   //���ڷ�Χ-512 ~ 512
				if(temp > 4096) temp = 4096;
				else if(temp < 0) temp =0;
				TxData_Buf[0]=temp >> 8;
				TxData_Buf[1]=temp & 0xFF;
				
				temp = (4096-rightHand_U_D) + (rotarySwitch_3 -2048)/4;   //���ڷ�Χ-512 ~ 512
				if(temp > 4096) temp = 4096;
				else if(temp < 0) temp =0;
				TxData_Buf[2]=temp >> 8;
				TxData_Buf[3]=temp & 0xFF;
				
				temp = (4096 - rightHand_L_R) + (rotarySwitch_2 -2048)/4;   //���ڷ�Χ-512 ~ 512
				if(temp > 4096) temp = 4096;
				else if(temp < 0) temp =0;
				TxData_Buf[4]=temp >> 8;
				TxData_Buf[5]=temp & 0xFF;
				
				NRF24L01_TxPacket(TxData_Buf);  //send
				
				count ++;
				if(count%10 ==0 )
					LED1 = !LED1;
			}
			IWDG_Feed();//ι��		

//			for(i=0;i<8;i++)
//				printf("%d  ",ADC_data[i]);
//			printf("count=%d\r\n",count);
			
		
			//printf("%x\r\n",TxData_Buf[31]);
		}

	return 0;
		
}
 


