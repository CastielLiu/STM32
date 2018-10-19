#ifndef __UART_H
#define __UART_H



#include "type_def.h"
#include <stdio.h>
#include <iostm8s105k4.h>
#define HSIClockFreq 16000000
//#define HSIClockFreq 12000000
#define BaudRate  9600
#define UART2_FLAG_TXE  (uint8_t)0x80  /*!< Transmit Data Register Empty flag */
#define UART2_FLAG_RXNE (uint8_t)0x20 /*!< Read Data Register Not Empty flag */
#define RxBufferSize 64
void uart_conf(void);
void UART2_SendByte(u8 data);
void UART2_SendString(u8* Data,u16 len);
u8 UART2_ReceiveByte(void);
int fputc(int ch, FILE *f);
#endif