#include "dac.h"
#include "speedControl.h"


//DAC通道1输出初始化
//PF0 速度方向引脚
void speedControl_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOF, ENABLE );	  //使能PORTA通道时钟
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOF, &GPIO_InitStructure);
	SPEED_DIRICTION = FRONT_DIR;	//初始化为前进挡
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
  
    DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值

}

//速度控制，模拟信号0-3.3v 
//stm32只能输出0-3.3v模拟信号。电机驱动器控制信号为0-5v
//如需更大速度，可使用电压放大器。
void speed_control(float set_speed)
{
	u16 dac_val;
	float voltage = set_speed/8;  //假定0-5V对应0-40km/h
	
	if (voltage>3.3) voltage = 3.3;
	
	dac_val = voltage/3.3 * 4096;
	
	DAC->DHR12R1 = dac_val;  //12位右对齐通道1寄存器
}



















































