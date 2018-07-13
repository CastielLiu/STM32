#include "direction.h"
#include "sys.h"
#include "math.h"


void car_control_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF, ENABLE);//使能 
//PB2 使能信号引脚
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_2);  //拉低  不使能
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //输出
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
}

void steer_control(float  angle_differ)
{
	 u16 period_ARR;
	 u16 PWM_rate;
	
	 if(angle_differ < -0.5)
	 {
			STEER_DIR = 1;
		 STEER_ENABLE =1;
	 }
	 else if(angle_differ > 0.5)
	 {
			STEER_DIR =0;
		 STEER_ENABLE =1;
	 }
	 else
		 STEER_ENABLE = 0;
	 
	PWM_rate = fabs(angle_differ) *400 + 0;
	 
	period_ARR = 72000000/PWM_rate/18 - 1;
	TIM3->ARR =period_ARR;
	TIM_SetCompare2(TIM3,period_ARR/2); 
}