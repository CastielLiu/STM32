#define MaxBufLen  256  // 环形存储区最大容量 （字节） 

uint8_t usartBuf[MaxBufLen]; 

int dataSize=0; // 环形数据存储区中的字节个数 

uint8_t *headPtr = usartBuf;
uint8_t *rearPtr = usartBuf;
uint8_t *usartBufEndPtr = usartBuf+MaxBufLen;

#include "string.h"

void USART1_IRQHandler(void) 
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
/*头指针与尾指针在同一个位置共有两种情况
1.第一次存储数据：此时dataSize为0 ，此时从串口缓存区读到的数据之间放到尾指针位置即可
2.环形存储区已满，此时需要读取一个字节、丢弃一个字节，而丢弃的字节应是最旧的字节，也就是当前head所指向的位置
	这种情况下把新字节写入rear指针位置便覆盖了旧字节，同时应将head指针移动到下一个字节位置！！ 
*/
		if(rearPtr-headPtr == 0 && dataSize!=0)
		{
			headPtr ++;
			//若数据头指针head指向了数组结尾，应将其重置回数组开始，实现环形读取。否则将错误的访问数组以外的内存区域导致错误 
			if(headPtr == usartBufEndPtr) headPtr = usartBuf;
		}
		*rearPtr = USART1->DR;
		rearPtr++;
		//若数据尾指针指向了数组结尾应将其重置为数组开始，实现环形存储。否则将错误的写数组以外的内存区域而引发错误 
		if(rearPtr == usartBufEndPtr) rearPtr = usartBuf;
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}		
} 

uint8_t readDataFromBuf(uint8_t *dst,int count)
{
	//每次读取数据应先判断缓存区的数据长度 
	if(rearPtr -headPtr >=0) 
		dataSize = rearPtr - headPtr;
	else
		dataSize = MaxBufLen + rearPtr -headPtr;
	
	if(count > dataSize) return 0; //如果读取的长度超过缓存区中的数据长度则不读取 
	
	if(count <= usartBufEndPtr - headPtr) //如果所需读取的字节均在头指针的后面，直接读取即可 
	{									//否则应先把头指针后面的数据读出，再读取位于头指针之前的数据 
		memcpy(dst,headPtr,count);//此时如果发生串口中断  将导致一字节丢失 
		headPtr += count;
	}
	else
	{
		memcpy(dst,headPtr,usartBufEndPtr - headPtr);
		memcpy(dst+(usartBufEndPtr-headPtr),usartBuf,count-(usartBufEndPtr-headPtr));
		headPtr = usartBuf + count-(usartBufEndPtr-headPtr);
	}
	return 1;
}

