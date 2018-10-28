#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "24cxx.h"	 

/************************************************
 ALIENTEK��ӢSTM32������ʵ��22
 IIC ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


 int main(void)
 {	 
	u16 i=0;
	u8 buf[] = "liushuaipeng"; 
	u8 buf1[15];
	u8 Addr = 0x00;
	IIC_Configuration();
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//������ʼ��		 	 	
	
	while(AT24CXX_Check())
	{
		LED0 = !LED0;
		delay_ms(300);
	}
	for(i=0;i<sizeof(buf);i++,Addr++)
	{
		IIC_Byte_Write( buf[i], Addr);
		delay_ms(2);
	}
	Addr = 0;
	
	delay_ms(20);
	for(i=0;i<10;i++,Addr++)
	{
		IIC_Byte_Read(&buf1[i], Addr);
		delay_ms(2);
	}
	while(1)
	{
		printf("%s\r\n",buf1);
		delay_ms(500);
	}

	return 0 ;
}
