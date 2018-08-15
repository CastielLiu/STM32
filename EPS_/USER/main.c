#include "delay.h"
#include "can.h"
#include "timer.h"
#include "usart.h"

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	delay_init();
	TIM2_Init(9999,72-1);//10ms溢出
	TIM5_Cap_Init(65535,72-1);//每计数一次1us
	uart_init(115200);
	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);
	while(1)
	{
		delay_ms(10);
		
	}
}


 