#ifndef  __DELAY_H_
#define  __DELAY_H_
#include "sys.h"

void delay_init(u8 clk); //延时函数初始化
void delay_us(u16 nus);  //us级延时函数,最大65536us.
void delay_ms(u32 nms);  //ms级延时函数



#endif
