#include"fun.h"
#include"stdint.h"

uint8_t       Run_flag=0;

//uint32_t      Thr_Mid=0;
//uint32_t      Rool_Mid =0;
//uint32_t      Pitch_Mid =0;
//uint32_t      Yaw_Mid = 0;

//uint32_t      Rool_MAX =0;
//uint32_t      Pitch_MAX =0;
//uint32_t      Yaw_MAX =0;  //0-1000

void ADC1_Config(void)
{
	ADC_InitTypeDef	ADC_InitStucture;
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_DeInit(ADC1);		//重置ADC配置	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //使能ADC1通道时钟
 	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC最大时间不能超过14M
	
	 //PA0/1/2/3/4/5/6/7 作为模拟通道输入引脚                         
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4| GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                //模拟输入引脚
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	//****ADC配置
	ADC_InitStucture.ADC_Mode=ADC_Mode_Independent;	//ADC1,ADC2工作于独立模式
	ADC_InitStucture.ADC_ScanConvMode=ENABLE;		//使能多通道扫描模式
	ADC_InitStucture.ADC_ContinuousConvMode=ENABLE;	//使能连续转换工作模式
	ADC_InitStucture.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;		//选择转换工作触发源,定义由软件触发,不采用外部触发启动
	ADC_InitStucture.ADC_DataAlign=ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStucture.ADC_NbrOfChannel=8;				//规定顺序进行规则转换的ADC通道数目
	ADC_Init(ADC1,&ADC_InitStucture);
    
	//****ADC 采样顺序		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_7Cycles5);			       //采样顺序	
			
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_7Cycles5);			       //采样顺序	
			
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_7Cycles5);		              //采样顺序	
			
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_7Cycles5);		              //采样顺序	
			
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,5,ADC_SampleTime_7Cycles5);		              //采样顺序		
		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,6,ADC_SampleTime_7Cycles5);		//采样顺序		
		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,7,ADC_SampleTime_7Cycles5);		//采样顺序	
		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,8,ADC_SampleTime_7Cycles5);		//采样顺序	
	//****使能内部温度传感器的参考电压
	//ADC_TempSensorVrefintCmd(ENABLE);
	
	//****ADC1 DMA启动
	ADC_DMACmd(ADC1,ENABLE);

	//****ADC1 启动
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);		//****重置ADC校准寄存器
	while (ADC_GetResetCalibrationStatus(ADC1));	
	
	ADC_StartCalibration(ADC1);		//****开始ADC校准
	while (ADC_GetCalibrationStatus(ADC1));	//****等待ADC校准寄存器完毕

	//****软件启动ADC转换
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	

}

void Initial_DMA_ADC1(u16 *ADC_Buffer_ptr)
{
	DMA_InitTypeDef	DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);								//初始化DMA1通道1为默认值
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);		//启动DMA时钟
	DMA_InitStructure.DMA_PeripheralBaseAddr=((u32)0x4001244c);	//DMA外设寄存器地起始地址
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)ADC_Buffer_ptr;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;			//DMA传输方向,定议为寄存器传输到缓存
	DMA_InitStructure.DMA_BufferSize=8;			//DMA缓存区大小
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;	//DMA外设寄存器地址不递增
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;		//DMA缓存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;	//设置外设寄存器数据为
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;		//
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;				//DMA设置为循环模式
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;				//设置优先级
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;				//设置为单向访问
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);				
	DMA_Cmd(DMA1_Channel1, ENABLE);						//启动DMA1通道1
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);			//使能DMA1道道1传送完毕中断源

}
