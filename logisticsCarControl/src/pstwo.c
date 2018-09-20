#include "pstwo.h"
#include "usart.h"
#include "param.h"
#include "speedControl.h"
#include "brakeControl.h"
/*********************************************************
Copyright (C), 2015-2025, ZYRobot.
湖南智宇科教设备有限公司
www.hnzhiyu.cn
File：PS2驱动程序
Author：pinggai    Version:1.0     Data:2015/05/16
Description: PS2驱动程序
**********************************************************/	 
u16 Handkey;
u8 Comd[2]={0x01,0x42};	//开始命令。请求数据
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //数据存储数组
u16 MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//按键值与按键明

//手柄接口初始化    输入  DI->PB12 
//                  输出  DO->PB13    CS->PB14  CLK->PB15
void PS2_Init(void)
{
    //输入  DI->PB12
	RCC->APB2ENR|=1<<3;     //使能PORTB时钟
	GPIOB->CRH&=0XFFF0FFFF;//PB12设置成输入	默认下拉  
	GPIOB->CRH|=0X00080000;   

    //  DO->PB13    CS->PB14  CLK->PB15
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟  	   	  	 
	GPIOB->CRH&=0X000FFFFF; 
	GPIOB->CRH|=0X33300000;//PB13、PB14、PB15 推挽输出   	 											  
}

//向手柄发送命令
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //输出以为控制位 PBout(13)
		}
		else DO_L;

		CLK_H;                        //时钟拉高
		delay_us(50);
		CLK_L;
		delay_us(50);
		CLK_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
}
//判断是否为红灯模式
//返回值；0，红灯模式
//		  其他，其他模式
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//读取手柄数据
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;

	CS_L;

	PS2_Cmd(Comd[0]);  //开始命令
	PS2_Cmd(Comd[1]);  //请求数据

	for(byte=2;byte<9;byte++)          //开始接受数据
	{
		for(ref=0x01;ref<0x100;ref<<=1)
		{
			CLK_H;
			CLK_L;
			delay_us(50);
			CLK_H;
		      if(DI)
		      Data[byte] = ref|Data[byte];
		}
        delay_us(50);
	}
	CS_H;	
}

//对读出来的PS2的数据进行处理      只处理了按键部分         默认数据是红灯模式  只有一个按键按下时
//按下为0， 未按下为1
u8 PS2_DataKey()
{
	u8 index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //这是16个按键  按下为0， 未按下为1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //没有任何按键按下
}

//得到一个摇杆的模拟量	 范围0~256
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}

//清除数据缓冲区
void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}

void PS2_RequestData()
{
	PS2_ClearData();//清空数组，防止因遥控器掉电或通信异常时控制信号依然保持
	PS2_ReadData(); //读取9个字节的数据。
/*
bit0 ---------------bit7
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
}

//利用摇杆反向拨动实现刹车
//考虑刹车使车辆静止后摇杆依然处于拨动状态，不能使车辆反方向行驶
//释放摇杆后，才可释放刹车！！
void PS2_SpeedControl(u8 *data)
{
	static u8 PS2_brake_flag =0;
	u8 speedBuf = data[8];
	printf("%d\r\n",speedBuf);
	if(g_vehicleSpeed_LF !=0 || g_vehicleSpeed_RF !=0 )//当前速度不为0
	{	
		if(SPEED_DIRICTION == FRONT_DIR &&  speedBuf>130 )//正在前进但手柄后拨，表明希望制动
		{
			speed_control(0); 
			brake_control(3.3*(speedBuf-127)/127);
			PS2_brake_flag = 1;//制动标志位置1
		}
		else if(SPEED_DIRICTION == BACK_DIR && speedBuf <125)//正在后退但手柄前拨，表明希望制动
		{
			speed_control(0); 
			brake_control(3.3*(speedBuf-127)/127);
			PS2_brake_flag = 1;//制动标志位置1
		}
	}

	if(speedBuf<129 && speedBuf >126)//速度摇杆中位
	{
		PS2_brake_flag = 0;  
	}
	
	if(PS2_brake_flag ==0)
	{
		brake_control(-3.3);//释放刹车
		speed_control(g_teleControlMaxSpeed *(127 - speedBuf)/127);
	}
}


