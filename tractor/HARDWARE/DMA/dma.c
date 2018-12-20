#include "dma.h"
#include "usart.h"
#include "global_params.h"
#include "string.h"



DMA_InitTypeDef DMA_InitStructure;

u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 	    
//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	
	
    DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽��
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ��� 
	
	 // NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);
	
} 

u16 ADC_temp_Buf[2];

void ADC_DMA_Config()  //DMA1_CH1 ADC1
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_temp_Buf;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽��
	DMA_InitStructure.DMA_BufferSize = ADC_CH_CNT ;  //DMAͨ����DMA�� ��Ĵ�С  2��ADCͨ����ȡ10�εľ�ֵ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ //DMA_Mode_Circular
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ���
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��   
	 
 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}	  

void ADC_DMA_Enable()
{ 
	DMA_Cmd(DMA1_Channel1, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA1_Channel1,ADC_CH_CNT );//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA1_Channel1, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}	 







union CON
{
	u8 in[8];
	double out;
}convert;

void DMA1_Channel3_IRQHandler(void)
{
	float east_speed, north_speed, down_speed;
	
	if(DMA_GetITStatus(DMA1_IT_TC3)!= RESET)//��������ж�
	{ 
		DMA_ClearFlag(DMA1_FLAG_TC3);//���dma��������жϱ�־
		
		if(gps_data_buf[1]==0x14 && gps_data_buf[2] ==0x64)
		{
			gps_data_buf[1] = 0x00;
			gps_data_buf[2] = 0x00;
			
			gps_sphere_now.yaw = *(float  *)gpsPtr->yaw;
			memcpy(convert.in,gpsPtr->lon,8);
			gps_sphere_now.lon = convert.out;
			memcpy(convert.in,gpsPtr->lat,8);
			gps_sphere_now.lat = convert.out;
			
						
			//generate send msg
			send_lon = gps_sphere_now.lon *180/pi *10000000;
			send_lat = gps_sphere_now.lat *180/pi *10000000;
			send_yaw = gps_sphere_now.yaw *180/pi *100;
			
			send_gps_status = ((gpsPtr->a[2])<<4 )>>5 ; //a[2]�� 4,5 6�ֽڱ�ʾ��λ״̬
			send_satellites = 10;  
			
			east_speed =  *(float *)gpsPtr->vel_e;
			north_speed = *(float *)gpsPtr->vel_n;
			down_speed = *(float *)gpsPtr->down_velocity;
			send_speed = sqrt(east_speed*east_speed+north_speed*north_speed+down_speed*down_speed)*3.6*100;//km/h  �Ŵ�100��
			printf("right\r\n");
		}
		else
		{
			USART_SendData(USART3,0xff);
		}
		
		
	}
}	




