#include "stm32f10x.h"
#include "can.h"
#include "usart.h"
#include "delay.h"
 
 

int main(void)
{
	delay_init();
	uart_init(115200);
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//500kbps
	while(1)
	{
		delay_ms(5);
	}
	
}
