#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "delay.h"

#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	

//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
#endif


