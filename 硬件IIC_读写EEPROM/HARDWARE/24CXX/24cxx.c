#include "24cxx.h" 
#include "delay.h"

void AT24CXX_Init(void)
{
	IIC_Configuration();
}

//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	u8 temp1 = 0x55;
	IIC_Byte_Read(&temp,255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		IIC_Byte_Write(0x55,255);
	    IIC_Byte_Read(&temp,255);	  
		if(temp==0x55)return 0;
	}
	return 1;											  
}