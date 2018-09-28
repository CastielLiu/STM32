#include "dac.h"
#include "math.h"
#include "speedControl.h"
#include "brakeControl.h"


//DACͨ��2�����ʼ��
//PF1 �ƶ�or�ͷ��ƶ�
void brakeControl_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF, ENABLE );	  //ʹ��PORTAͨ��ʱ��PORTFʱ��
	
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
	BRAKE_STATUS = BRAKE_DISABLE;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
    
	DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��2

	DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��DAC2
  
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}

//�ƶ����ƣ���Ҫ������������
//�����Ƹ����ԣ��޷�ȷ����ο��� 2018.9.10
void brake_control(float set_brake_voltage)
{
	u16 dac_val;
	
	if(set_brake_voltage<0)
	{
		BRAKE_STATUS = BRAKE_DISABLE;
		set_brake_voltage = -set_brake_voltage;
	}
	else
		BRAKE_STATUS = BRAKE_ENABLE;
	
	if (set_brake_voltage>3.3) set_brake_voltage = 3.3;
	
	dac_val = set_brake_voltage/3.3 * 4095;
	
	DAC->DHR12R2 = dac_val;  //12λ�Ҷ���ͨ��2�Ĵ���
}

void emergencyBrake(void)
{
	speed_control(0.0);
	brake_control(-3.3);
}

