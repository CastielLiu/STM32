
#ifndef __LED_CONF_H
#define __LED_CONF_H
#include "type_def.h"

/* 定义位操作 前提是引脚设置为输出功能 */
#define PD0_out  PD_ODR_ODR0  
#define PD1_out  PD_ODR_ODR1  
#define PD2_out  PD_ODR_ODR2  
#define PD3_out  PD_ODR_ODR3  

#define LED0  PD2_out
#define LED1  PD3_out

/* LED函数定义 */
void LED_conf(void);


#endif




