#include "dma.h"
#include "usart.h"
#include "global_params.h"
#include "string.h"



DMA_InitTypeDef DMA_InitStructure;

u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 	    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输
	
	
	
    DMA_DeInit(DMA_CHx);   //将DMA的通道1寄存器重设为缺省值

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMA通道的DMA缓存的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道USART1_Tx_DMA_Channel所标识的寄存器 
	
	 // NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);
	
} 

u16 ADC_temp_Buf[2];

void ADC_DMA_Config()  //DMA1_CH1 ADC1
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel1);   //将DMA的通道1寄存器重设为缺省值

	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;  //DMA外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_temp_Buf;  //DMA内存基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向
	DMA_InitStructure.DMA_BufferSize = ADC_CH_CNT ;  //DMA通道的DMA缓 存的大小  2个ADC通道，取10次的均值
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //数据宽度为8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式 //DMA_Mode_Circular
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

//开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //关闭USART1 TX DMA1 所指示的通道   
	 
 	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA_CHx, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
}	  

void ADC_DMA_Enable()
{ 
	DMA_Cmd(DMA1_Channel1, DISABLE );  //关闭USART1 TX DMA1 所指示的通道      
 	DMA_SetCurrDataCounter(DMA1_Channel1,ADC_CH_CNT );//DMA通道的DMA缓存的大小
 	DMA_Cmd(DMA1_Channel1, ENABLE);  //使能USART1 TX DMA1 所指示的通道 
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
	
	if(DMA_GetITStatus(DMA1_IT_TC3)!= RESET)//传输完成中断
	{ 
		DMA_ClearFlag(DMA1_FLAG_TC3);//清除dma传输完成中断标志
		
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
			
			send_gps_status = ((gpsPtr->a[2])<<4 )>>5 ; //a[2]的 4,5 6字节表示定位状态
			send_satellites = 10;  
			
			east_speed =  *(float *)gpsPtr->vel_e;
			north_speed = *(float *)gpsPtr->vel_n;
			down_speed = *(float *)gpsPtr->down_velocity;
			send_speed = sqrt(east_speed*east_speed+north_speed*north_speed+down_speed*down_speed)*3.6*100;//km/h  放大100倍
			printf("right\r\n");
		}
		else
		{
			USART_SendData(USART3,0xff);
		}
		
		
	}
}	




