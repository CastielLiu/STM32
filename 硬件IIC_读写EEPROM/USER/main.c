#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "24cxx.h"	 

/************************************************
 ALIENTEK精英STM32开发板实验22
 IIC 实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


 int main(void)
 {	 
	u16 i=0;
	u8 buf[] = "liushuaipeng"; 
	u8 buf1[15];
	u8 Addr = 0x00;
	IIC_Configuration();
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();		  		//初始化与LED连接的硬件接口
	KEY_Init();				//按键初始化		 	 	
	
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
