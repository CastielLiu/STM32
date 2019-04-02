#include "main.h"
 
u8 g_ykSafty_num = 0;

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

#define BUF_LEN 12

int main(void)
{	
	u8 wirelessBuf[32];	 //遥控器数据接收缓存	
	u8 sendBuf[BUF_LEN]={0x66,0xcc};
	int i=0;
	int count=0;
	
	system_init();
	NRF24L01_Init();    		//初始化NRF24L01 
	while(NRF24L01_Check())
	{
 		delay_ms(200);
		LED1 =!LED1;
		LED0=!LED0;
	}
	NRF24L01_RX_Mode();	
	
	while(1)//25ms
	{
		if(NRF24L01_RxPacket(wirelessBuf)==0 )//收到消息
		{
			for(i=0;i<BUF_LEN-3;i++)
				sendBuf[2+i] = wirelessBuf[i];
			
			sendBuf[BUF_LEN-1] = generate_check_sum(sendBuf,BUF_LEN-1);
			write(USART1,sendBuf,BUF_LEN);
		}
		delay_ms(10);
		count++;
		if(count%30==0)
			LED1 = !LED1;
		
	}	 
 }
