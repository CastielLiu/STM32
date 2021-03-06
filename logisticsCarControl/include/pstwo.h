#ifndef __PSTWO_H
#define __PSTWO_H
#include "delay.h"
#include "sys.h"
/*********************************************************
Copyright (C), 2015-2025, YFRobot.
www.yfrobot.com
File：PS2驱动程序
Author：pinggai    Version:1.0     Data:2015/05/16
Description: PS2驱动程序
**********************************************************/	 
#define DI   PBin(12)           //PB12  输入

#define DO_H PBout(13)=1        //命令位高
#define DO_L PBout(13)=0        //命令位低

#define CS_H PBout(14)=1       //CS拉高
#define CS_L PBout(14)=0       //CS拉低

#define CLK_H PBout(15)=1      //时钟拉高
#define CLK_L PBout(15)=0      //时钟拉低


//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2         9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      26

//#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5                //右摇杆X轴数据
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8

#define PS2_L2	      ((Data[4])&0x01)
#define PS2_R2 		  ((Data[4]>>1)&0x01)
#define PS2_L1 		  ((Data[4]>>2)&0x01)
#define PS2_R1		  ((Data[4]>>3)&0x01)
#define PS2_TRIANGLE  ((Data[4]>>4)&0x01)
#define PS2_CIRCLE 	  ((Data[4]>>5)&0x01)
#define PS2_STOP 	  ((Data[4]>>6)&0x01)
#define PS2_RECTANGLE ((Data[4]>>7)&0x01)

/*
0	Idle  
1	ID	
2	0x5A	
3	Data1	：select、L3，R3，start，up，right，down，left
4	Data2	：L2，R2，L1,R1,△，O，×，□
5	Data3	：右手遥杆 左<->右 0x00<->0xff
6	Data4	：右手遥杆 上<->下 0x00<->0xff
7	Data5	：左手遥杆 左<->右 0x00<->0xff
8	Data6	：左手遥杆 上<->下 0x00<->0xff
*/

extern u8 Data[9];
extern u16 MASK[16];
extern u16 Handkey;

void PS2_Init(void);
u8 PS2_RedLight(void);//判断是否为红灯模式
void PS2_ReadData(void);
void PS2_Cmd(u8 CMD);		  //
u8 PS2_DataKey(void);		  //键值读取
u8 PS2_AnologData(u8 button); //得到一个摇杆的模拟量
void PS2_ClearData(void);	  //清除数据缓冲区
void PS2_RequestData(void);
void PS2_SpeedControl(u8 *data);

#endif





