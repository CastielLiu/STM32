#include "pstwo.h"
#include "usart.h"
#include "param.h"
#include "speedControl.h"
#include "brakeControl.h"
/*********************************************************
Copyright (C), 2015-2025, ZYRobot.
��������ƽ��豸���޹�˾
www.hnzhiyu.cn
File��PS2��������
Author��pinggai    Version:1.0     Data:2015/05/16
Description: PS2��������
**********************************************************/	 
u16 Handkey;
u8 Comd[2]={0x01,0x42};	//��ʼ�����������
u8 Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
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
	};	//����ֵ�밴����

//�ֱ��ӿڳ�ʼ��    ����  DI->PB12 
//                  ���  DO->PB13    CS->PB14  CLK->PB15
void PS2_Init(void)
{
    //����  DI->PB12
	RCC->APB2ENR|=1<<3;     //ʹ��PORTBʱ��
	GPIOB->CRH&=0XFFF0FFFF;//PB12���ó�����	Ĭ������  
	GPIOB->CRH|=0X00080000;   

    //  DO->PB13    CS->PB14  CLK->PB15
	RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��  	   	  	 
	GPIOB->CRH&=0X000FFFFF; 
	GPIOB->CRH|=0X33300000;//PB13��PB14��PB15 �������   	 											  
}

//���ֱ���������
void PS2_Cmd(u8 CMD)
{
	volatile u16 ref=0x01;
	Data[1] = 0;
	for(ref=0x01;ref<0x0100;ref<<=1)
	{
		if(ref&CMD)
		{
			DO_H;                   //�����Ϊ����λ PBout(13)
		}
		else DO_L;

		CLK_H;                        //ʱ������
		delay_us(50);
		CLK_L;
		delay_us(50);
		CLK_H;
		if(DI)
			Data[1] = ref|Data[1];
	}
}
//�ж��Ƿ�Ϊ���ģʽ
//����ֵ��0�����ģʽ
//		  ����������ģʽ
u8 PS2_RedLight(void)
{
	CS_L;
	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������
	CS_H;
	if( Data[1] == 0X73)   return 0 ;
	else return 1;

}
//��ȡ�ֱ�����
void PS2_ReadData(void)
{
	volatile u8 byte=0;
	volatile u16 ref=0x01;

	CS_L;

	PS2_Cmd(Comd[0]);  //��ʼ����
	PS2_Cmd(Comd[1]);  //��������

	for(byte=2;byte<9;byte++)          //��ʼ��������
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

//�Զ�������PS2�����ݽ��д���      ֻ�����˰�������         Ĭ�������Ǻ��ģʽ  ֻ��һ����������ʱ
//����Ϊ0�� δ����Ϊ1
u8 PS2_DataKey()
{
	u8 index;

	PS2_ClearData();
	PS2_ReadData();

	Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0�� δ����Ϊ1
	for(index=0;index<16;index++)
	{	    
		if((Handkey&(1<<(MASK[index]-1)))==0)
		return index+1;
	}
	return 0;          //û���κΰ�������
}

//�õ�һ��ҡ�˵�ģ����	 ��Χ0~256
u8 PS2_AnologData(u8 button)
{
	return Data[button];
}

//������ݻ�����
void PS2_ClearData()
{
	u8 a;
	for(a=0;a<9;a++)
		Data[a]=0x00;
}

void PS2_RequestData()
{
	PS2_ClearData();//������飬��ֹ��ң���������ͨ���쳣ʱ�����ź���Ȼ����
	PS2_ReadData(); //��ȡ9���ֽڵ����ݡ�
/*
bit0 ---------------bit7
0	Idle  
1	ID	
2	0x5A	
3	Data1	��select��L3��R3��start��up��right��down��left
4	Data2	��L2��R2��L1,R1,����O��������
5	Data3	������ң�� ��<->�� 0x00<->0xff
6	Data4	������ң�� ��<->�� 0x00<->0xff
7	Data5	������ң�� ��<->�� 0x00<->0xff
8	Data6	������ң�� ��<->�� 0x00<->0xff
*/
}

//����ҡ�˷��򲦶�ʵ��ɲ��
//����ɲ��ʹ������ֹ��ҡ����Ȼ���ڲ���״̬������ʹ������������ʻ
//�ͷ�ҡ�˺󣬲ſ��ͷ�ɲ������
void PS2_SpeedControl(u8 *data)
{
	static u8 PS2_brake_flag =0;
	u8 speedBuf = data[8];
	printf("%d\r\n",speedBuf);
	if(g_vehicleSpeed_LF !=0 || g_vehicleSpeed_RF !=0 )//��ǰ�ٶȲ�Ϊ0
	{	
		if(SPEED_DIRICTION == FRONT_DIR &&  speedBuf>130 )//����ǰ�����ֱ��󲦣�����ϣ���ƶ�
		{
			speed_control(0); 
			brake_control(3.3*(speedBuf-127)/127);
			PS2_brake_flag = 1;//�ƶ���־λ��1
		}
		else if(SPEED_DIRICTION == BACK_DIR && speedBuf <125)//���ں��˵��ֱ�ǰ��������ϣ���ƶ�
		{
			speed_control(0); 
			brake_control(3.3*(speedBuf-127)/127);
			PS2_brake_flag = 1;//�ƶ���־λ��1
		}
	}

	if(speedBuf<129 && speedBuf >126)//�ٶ�ҡ����λ
	{
		PS2_brake_flag = 0;  
	}
	
	if(PS2_brake_flag ==0)
	{
		brake_control(-3.3);//�ͷ�ɲ��
		speed_control(g_teleControlMaxSpeed *(127 - speedBuf)/127);
	}
}


