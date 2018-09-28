#include "dac.h"
#include "math.h"
#include "speedControl.h"
#include "brakeControl.h"


//DAC通道2输出初始化
//PF1 制动or释放制动
void brakeControl_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF, ENABLE );	  //使能PORTA通道时钟PORTF时钟
	
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
	BRAKE_STATUS = BRAKE_DISABLE;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
    
	DAC_Init(DAC_Channel_2,&DAC_InitType);	 //初始化DAC通道2

	DAC_Cmd(DAC_Channel_2, ENABLE);  //使能DAC2
  
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
}

//制动控制，需要正反两个方向。
//暂无推杆属性，无法确定如何控制 2018.9.10
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
	
	DAC->DHR12R2 = dac_val;  //12位右对齐通道2寄存器
}

void emergencyBrake(void)
{
	speed_control(0.0);
	brake_control(-3.3);
}

