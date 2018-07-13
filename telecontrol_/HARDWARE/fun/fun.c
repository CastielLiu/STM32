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
	ADC_DeInit(ADC1);		//����ADC����	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	 //PA0/1/2/3/4/5/6/7 ��Ϊģ��ͨ����������                         
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4| GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                //ģ����������
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	//****ADC����
	ADC_InitStucture.ADC_Mode=ADC_Mode_Independent;	//ADC1,ADC2�����ڶ���ģʽ
	ADC_InitStucture.ADC_ScanConvMode=ENABLE;		//ʹ�ܶ�ͨ��ɨ��ģʽ
	ADC_InitStucture.ADC_ContinuousConvMode=ENABLE;	//ʹ������ת������ģʽ
	ADC_InitStucture.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;		//ѡ��ת����������Դ,�������������,�������ⲿ��������
	ADC_InitStucture.ADC_DataAlign=ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStucture.ADC_NbrOfChannel=8;				//�涨˳����й���ת����ADCͨ����Ŀ
	ADC_Init(ADC1,&ADC_InitStucture);
    
	//****ADC ����˳��		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_7Cycles5);			       //����˳��	
			
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_7Cycles5);			       //����˳��	
			
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_7Cycles5);		              //����˳��	
			
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_7Cycles5);		              //����˳��	
			
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,5,ADC_SampleTime_7Cycles5);		              //����˳��		
		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,6,ADC_SampleTime_7Cycles5);		//����˳��		
		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_6,7,ADC_SampleTime_7Cycles5);		//����˳��	
		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,8,ADC_SampleTime_7Cycles5);		//����˳��	
	//****ʹ���ڲ��¶ȴ������Ĳο���ѹ
	//ADC_TempSensorVrefintCmd(ENABLE);
	
	//****ADC1 DMA����
	ADC_DMACmd(ADC1,ENABLE);

	//****ADC1 ����
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);		//****����ADCУ׼�Ĵ���
	while (ADC_GetResetCalibrationStatus(ADC1));	
	
	ADC_StartCalibration(ADC1);		//****��ʼADCУ׼
	while (ADC_GetCalibrationStatus(ADC1));	//****�ȴ�ADCУ׼�Ĵ������

	//****�������ADCת��
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	

}

void Initial_DMA_ADC1(u16 *ADC_Buffer_ptr)
{
	DMA_InitTypeDef	DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);								//��ʼ��DMA1ͨ��1ΪĬ��ֵ
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);		//����DMAʱ��
	DMA_InitStructure.DMA_PeripheralBaseAddr=((u32)0x4001244c);	//DMA����Ĵ�������ʼ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)ADC_Buffer_ptr;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;			//DMA���䷽��,����Ϊ�Ĵ������䵽����
	DMA_InitStructure.DMA_BufferSize=8;			//DMA��������С
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;	//DMA����Ĵ�����ַ������
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;		//DMA�����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;	//��������Ĵ�������Ϊ
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;		//
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;				//DMA����Ϊѭ��ģʽ
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;				//�������ȼ�
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;				//����Ϊ�������
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);				
	DMA_Cmd(DMA1_Channel1, ENABLE);						//����DMA1ͨ��1
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);			//ʹ��DMA1����1��������ж�Դ

}
