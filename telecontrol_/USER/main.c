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
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();
	KEY_Init();
	
	NRF24L01_Init();    		//初始化NRF24L01 
	
	Initial_DMA_ADC1(&ADC_data[0]);
	ADC1_Config();  //initial    先初始化DMA 并开启DMA传输，再初始化ADC和开启软件转换
 
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

			//遥控器电池电压过低3.6V报警
			if (BATTER_Volt < 2233)
			{
				;
//					Bzz_ON();
//					delay_ms(300);
//					Bzz_OFF();
//					delay_ms(200);
			}
			//按键检测，mode切换
			if(MODE_KEY ==0)
			{
				delay_ms(5);//消抖
				if(MODE_KEY ==0)
				{
					runMode = !runMode;
					
					//printf("runMode = %d\t",runMode);
				}
				while(MODE_KEY ==0){IWDG_Feed();	delay_ms(30);} ;//当检测到按键时，主循环时间变长，喂狗时间被滞后，将导致复位
																	//因此在按键松开之前循环喂狗。
			}
			
			if(runMode ==1 )
				IR_LED_ON();
			else
				IR_LED_OFF();
				
			//0-1 左轮速，2-3 右轮速  4-5 转向（右手）
			temp1 = leftHand_U_D + (rotarySwitch_1 -2048)/4;   //调节范围-512 ~ 512
			if(temp1 > 4096) temp1 = 4096;
			else if(temp1 < 0) temp1 =0;
			TxData_Buf[0]=temp1 >> 8;
			TxData_Buf[1]=temp1 & 0xFF;
			
			temp2 = (4096-rightHand_U_D) + (rotarySwitch_3 -2048)/4;   //调节范围-512 ~ 512
			if(temp2 > 4096) temp2 = 4096;
			else if(temp2 < 0) temp2 =0;
			TxData_Buf[2]=temp2 >> 8;
			TxData_Buf[3]=temp2 & 0xFF;
			
			temp3 = (4096 - rightHand_L_R) + (rotarySwitch_2 -2048)/4;   //调节范围-512 ~ 512
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
			
			TxData_Buf[8] &= 0xfe; //末位清零
			TxData_Buf[8] |= (runMode&0x01);
			
			
			NRF24L01_TxPacket(TxData_Buf);  //send
			
			count ++;
			if(count%20 ==0 )
				LED1 = !LED1;
		
			IWDG_Feed();//喂狗		

		}

	return 0;
		
}
 
uint8_t is_bar_medium(uint16_t temp)
{
	if(temp<2048+100 && temp>2048-100)
		return 1;
	return 0;
}


