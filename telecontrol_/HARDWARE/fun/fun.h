#ifndef _fun_H_
#define _fun_H_
#include"stdint.h"
#include "stm32f10x.h"

#endif

#define  leftHand_L_R         ADC_data[5]  //左、右           //左手左右 0-4096      u16
#define  rightHand_U_D    ADC_data[7]  //前进、后退   		//右手上下  0-4096
#define  rightHand_L_R            ADC_data[6] //左右侧飞   				//右手左右  4096-0
#define  leftHand_U_D              ADC_data[4]  //油门							//左手上下  4096-0

#define  rotarySwitch_2     ADC_data[0]	 //左、右补偿				//
#define  rotarySwitch_1   	ADC_data[2]  //前进、后退补偿
#define  rotarySwitch_3     ADC_data[1]  //左右侧飞补偿
#define  BATTER_Volt        ADC_data[3]  //电池电压

#define  LED_ON()           GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define  LED_OFF()          GPIO_SetBits  (GPIOB, GPIO_Pin_11)
#define  LED1								PBout(11)
#define  Bzz_ON()           GPIO_SetBits  (GPIOB, GPIO_Pin_10)
#define  Bzz_OFF()          GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define  Beep 							PBout(10)
#define  IR_LED_ON()        GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define  IR_LED_OFF()       GPIO_SetBits  (GPIOB, GPIO_Pin_12)
#define  LED2 							PBout(12)

#define  SW_Key             GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define  State_Key			    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

#define TRUE  1
#define FALSE 0

void ADC1_Config(void);
void Initial_DMA_ADC1(u16 *ADC_Buffer_ptr);
