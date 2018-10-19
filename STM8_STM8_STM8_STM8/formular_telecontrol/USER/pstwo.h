#ifndef PSTWO_H_
#define PSTWO_H_
#include "sys.h"
#include "iostm8s105k4.h"
#include "delay_.h"



#define DI   PB_IDR&0x20  //PB5 in

#define DO_H PB_ODR |=0x08  //PB3 =1
#define DO_L PB_ODR &=0xf7  //PB3 =0

#define CS_H PB_ODR |=0x04  //PB2=1
#define CS_L PB_ODR &=0xfb  //PB2=0

#define CLK_H PB_ODR |=0x10  //PB4=1
#define CLK_L PB_ODR &=0xef  //PB4=0

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

extern u8 Data[9];
extern u16 MASK[16];
extern u16 Handkey;

void PS2_Init(void);
u8 PS2_RedLight(void);//?D??¨º?¡¤??ao¨¬¦Ì??¡ê¨º?
void PS2_ReadData(void);
void PS2_Cmd(u8 CMD);		  //
u8 PS2_DataKey(void);		  //?¨¹?¦Ì?¨¢¨¨?
u8 PS2_AnologData(u8 button); //¦Ì?¦Ì?¨°???¨°???¦Ì??¡ê?a¨¢?
void PS2_ClearData(void);	  //??3y¨ºy?Y?o3???
void PS2_RequestData(void);
void PS2_SpeedControl(u8 *data);

#endif