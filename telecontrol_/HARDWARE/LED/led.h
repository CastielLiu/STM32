#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define  LED_ON()           GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define  LED_OFF()          GPIO_SetBits  (GPIOB, GPIO_Pin_11)
#define  Bzz_ON()           GPIO_SetBits  (GPIOB, GPIO_Pin_10)
#define  Bzz_OFF()          GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define  IR_LED_ON()        GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define  IR_LED_OFF()       GPIO_SetBits  (GPIOB, GPIO_Pin_12)


void LED_Init(void);//��ʼ��

		 				    
#endif
