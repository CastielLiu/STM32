

#include "led.h"


void LED_conf(void)
{
  PD_DDR|=0x0c; //PD2  PD3  /* 设置数据方向寄存器 1为输出，0为输入--查看STM8寄存器.pdf P87 */
  PD_CR1|=0x0c;   /* 设置推挽输出--查看STM8寄存器.pdf P88 */
  PD_CR2|=0x0c;   /* 设置输出频率 1为10M，0为2M--查看STM8寄存器.pdf P89 */
  PD_ODR&=0xf3; //2,3引脚拉低
}

