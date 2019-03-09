#define MaxBufLen  256  // ���δ洢��������� ���ֽڣ� 

uint8_t usartBuf[MaxBufLen]; 

int dataSize=0; // �������ݴ洢���е��ֽڸ��� 

uint8_t *headPtr = usartBuf;
uint8_t *rearPtr = usartBuf;
uint8_t *usartBufEndPtr = usartBuf+MaxBufLen;

#include "string.h"

void USART1_IRQHandler(void) 
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
/*ͷָ����βָ����ͬһ��λ�ù����������
1.��һ�δ洢���ݣ���ʱdataSizeΪ0 ����ʱ�Ӵ��ڻ���������������֮��ŵ�βָ��λ�ü���
2.���δ洢����������ʱ��Ҫ��ȡһ���ֽڡ�����һ���ֽڣ����������ֽ�Ӧ����ɵ��ֽڣ�Ҳ���ǵ�ǰhead��ָ���λ��
	��������°����ֽ�д��rearָ��λ�ñ㸲���˾��ֽڣ�ͬʱӦ��headָ���ƶ�����һ���ֽ�λ�ã��� 
*/
		if(rearPtr-headPtr == 0 && dataSize!=0)
		{
			headPtr ++;
			//������ͷָ��headָ���������β��Ӧ�������û����鿪ʼ��ʵ�ֻ��ζ�ȡ�����򽫴���ķ�������������ڴ������´��� 
			if(headPtr == usartBufEndPtr) headPtr = usartBuf;
		}
		*rearPtr = USART1->DR;
		rearPtr++;
		//������βָ��ָ���������βӦ��������Ϊ���鿪ʼ��ʵ�ֻ��δ洢�����򽫴����д����������ڴ�������������� 
		if(rearPtr == usartBufEndPtr) rearPtr = usartBuf;
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}		
} 

uint8_t readDataFromBuf(uint8_t *dst,int count)
{
	//ÿ�ζ�ȡ����Ӧ���жϻ����������ݳ��� 
	if(rearPtr -headPtr >=0) 
		dataSize = rearPtr - headPtr;
	else
		dataSize = MaxBufLen + rearPtr -headPtr;
	
	if(count > dataSize) return 0; //�����ȡ�ĳ��ȳ����������е����ݳ����򲻶�ȡ 
	
	if(count <= usartBufEndPtr - headPtr) //��������ȡ���ֽھ���ͷָ��ĺ��棬ֱ�Ӷ�ȡ���� 
	{									//����Ӧ�Ȱ�ͷָ���������ݶ������ٶ�ȡλ��ͷָ��֮ǰ������ 
		memcpy(dst,headPtr,count);//��ʱ������������ж�  ������һ�ֽڶ�ʧ 
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

