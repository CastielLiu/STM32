#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ��ӢSTM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{

	  GPIO_InitTypeDef GPIO_InitStructure;
	  //ʹ��LED�õ�ʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //������B��ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; 
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	  GPIO_Init(GPIOB, &GPIO_InitStructure);

    Bzz_OFF();
}
 
