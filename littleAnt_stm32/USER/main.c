#include "main.h"
 

void write(USART_TypeDef* USARTx, uint8_t *Data,uint16_t len)
{
	int i=0;
	for(;i<len;i++)
	{
		USART_SendData(USARTx,*(Data+i));
		while(!USART_GetFlagStatus(USARTx,USART_FLAG_TXE));
	}
}

uint8_t generate_check_sum(uint8_t *buf,int len)
{
	uint8_t sum=0;
	int i=0;
	for(;i<len;i++)
	{
		sum += *(buf+i);
	}
	return sum;
}

u8 sendBuf[8]={0x66,0xCC,0x00,0x04,0x01,0x03,0x11};



int main(void)
{	
	uint8_t driverlessMode = 0;
	uint8_t emergencyBrake = 0;
	
	sendBuf[7] = generate_check_sum(sendBuf+2,5);
	
	system_init();

	
	while(1)//25ms
	{
		if(PFin(0) == 1)
		{
			delay_ms(3);
			if(PFin(0) == 1)
				driverlessMode = 1;
		}			
		else 
			driverlessMode = 0;
		if(PFin(1) == 1)
		{
			delay_ms(3);
			if(PFin(1) == 1)
			{
				emergencyBrake = 1;
				driverlessMode = 0;
			}
		}
		else
			emergencyBrake = 0;
		
		if(driverlessMode)
			sendBuf[5] |= 0x01;
		else
			sendBuf[5] &= 0xfe;
		
		if(emergencyBrake)
			sendBuf[5] |= 0x02;
		else
			sendBuf[5] &= 0xfd;
 		
		sendBuf[7] = generate_check_sum(sendBuf+2,5);
		
		write(USART1,sendBuf,8);
		delay_ms(20);
		
	}	 
	return 0;
}
