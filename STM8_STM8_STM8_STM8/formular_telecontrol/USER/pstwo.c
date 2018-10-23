#include"pstwo.h"

u16 Handkey;
u8 Comd[2]={0x01,0x42};	
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 

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
    PSB_BLUE, //15
    PSB_PINK //16
	};	

//PB5数据线
//PB4时钟线
//PB3命令线
//PB2片选线

void PS2_Init(void)
{//0输入  1输出
   	PB_DDR &=0xdf; //PB5 输入
    PB_DDR |=0x10; //PB4 输出
    PB_DDR |=0x0c; //PB3  PB2  输出
  
//0浮空  1 上拉输入
//0开漏  1 推挽
    PD_CR1 &=0xdf; //PB5 浮空输入  接下拉电阻
    PD_CR1 |=0x10;//PB4 推挽输出
    PD_CR1 |=0x0c; //PB3  PB2 推挽
    
    PD_CR2 |=0x10;//PB4 10M
    PD_CR2 |=0x0c;//PB3  PB2 10M
    
    PD_CR2 &=0xdf; //PB5  禁用外部中断  
}


void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;       
		}
		else DO_L;

		CLK_H;           
		delay_us(50);
		CLK_L;
		delay_us(50);
		CLK_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
}
//判断红灯模式
//红灯 0
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);   
	PS2_Cmd(Comd[1]);  
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;
}
//读手柄
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;

	CS_L;

	PS2_Cmd(Comd[0]);   
	PS2_Cmd(Comd[1]);   

	for(byte=2;byte<9;byte++)         
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

u8 PS2_DataKey()
{
	u8 index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];  
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;         
}

u8 PS2_AnologData(u8 button)
{
	return Data[button];
}


void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}

void PS2_RequestData()
{
	PS2_ClearData();
	PS2_ReadData(); 
}

