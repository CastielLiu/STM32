#include "dac.h"
#include "speedControl.h"


//DACͨ��1�����ʼ��
//PF0 �ٶȷ�������
void speedControl_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF, ENABLE );	  //ʹ��PORTAͨ��ʱ��
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
	SPEED_DIRICTION = FRONT_DIR;	//��ʼ��Ϊǰ����
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DAC1
  
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ

}

//�ٶȿ��ƣ�ģ���ź�0-3.3v 
//stm32ֻ�����0-3.3vģ���źš���������������ź�Ϊ0-5v
//��������ٶȣ���ʹ�õ�ѹ�Ŵ�����
void speed_control(float set_speed)
{
	u16 dac_val;
	float voltage = set_speed/8;  //�ٶ�0-5V��Ӧ0-40km/h
	
	if (voltage>3.3) voltage = 3.3;
	
	dac_val = voltage/3.3 * 4096;
	
	DAC->DHR12R1 = dac_val;  //12λ�Ҷ���ͨ��1�Ĵ���
}



















































