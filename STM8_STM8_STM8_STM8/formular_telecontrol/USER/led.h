
#ifndef __LED_CONF_H
#define __LED_CONF_H
#include "type_def.h"

/* ����λ���� ǰ������������Ϊ������� */
#define PD0_out  PD_ODR_ODR0  
#define PD1_out  PD_ODR_ODR1  
#define PD2_out  PD_ODR_ODR2  
#define PD3_out  PD_ODR_ODR3  

#define LED0  PD2_out
#define LED1  PD3_out

/* LED�������� */
void LED_conf(void);


#endif




