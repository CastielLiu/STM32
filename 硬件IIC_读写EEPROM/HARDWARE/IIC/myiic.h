
#ifndef __IIC_H
#define __IIC_H
#include"stm32f10x.h"	

void IIC_Configuration(void);
void IIC_Byte_Write( u8 pBuffer, u8 Word_Address);
void IIC_Wait_EEprom(void);
void IIC_Byte_Read( u8* pRead, u8 Word_Address);
#endif	 













