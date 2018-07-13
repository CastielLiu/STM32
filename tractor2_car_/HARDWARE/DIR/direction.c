#include "direction.h"
#include "sys.h"
#include "math.h"


void car_control_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF, ENABLE);//使能 

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //输出
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

 void steer_control(float  request_angle)
{
	u16 ccr2 =0;
	ccr2 = 1500 + request_angle * 10;//系数待定
	TIM3->CCR2 = ccr2;
}