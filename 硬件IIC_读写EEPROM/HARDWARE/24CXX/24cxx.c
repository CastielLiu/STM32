#include "24cxx.h" 
#include "delay.h"

void AT24CXX_Init(void)
{
	IIC_Configuration();
}

//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	u8 temp1 = 0x55;
	IIC_Byte_Read(&temp,255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		IIC_Byte_Write(0x55,255);
	    IIC_Byte_Read(&temp,255);	  
		if(temp==0x55)return 0;
	}
	return 1;											  
}