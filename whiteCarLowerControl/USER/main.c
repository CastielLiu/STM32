#include "main.h"

//KEY0  						PE4 
//KEY1  						PE3 开始、取消无人驾驶
//PWM速度控制				PA6   TIM3_CH1
//PWM转向控制				PA7  	TIM3_CH2
//正交编码器测速    PB6-7 TIM4_CH12
//TIM5 用于计时  					TIM5


//spi      PB12 PB13 PB14 PB15
//24L01    PG6 PG7 PG8
//LCD     PB0 PD0 PD1 PD4 PD5 PD8 PD9 PD10 PD14 PD15 
//				PE7-15 PG0 PG12


//预装载值寄存器     TIMx->ARR  
//比较寄存器         TIMx->CCR2 
//CAN				StdId =0x12   ExtId=0x12;


u8 g_driveMode = TELECONTROL_MODE; //UPPER_CONTROL_MODE
u8 g_ykSafty_num = 0;
char g_driveModeName[20];
carControlData_t	g_carControlMsg = {2048,2048,2048}; //初始化车速，转向角为0

int main(void)
{	
	u8 wirelessBuf[32];	 //遥控器数据接收缓存	

	const float MaxDriveSpeedRateLimit = 0.2;
	const float MaxReverseSpeedRateLimit = 0.6;
 
	LED0 = 0;
	system_init();
///////////////////////////  遥控器相关	

	NRF24L01_Init();    		//初始化NRF24L01 
	while(NRF24L01_Check())
	{
 		delay_ms(200);
		LED1 =!LED1;
		LED0=!LED0;
	}
	NRF24L01_RX_Mode();	

	
	POINT_COLOR=BLACK;//设置字体为黑色
	LCD_ShowString(30,20 ,48 ,16,16,"mode :");
	strcpy(g_driveModeName,"Telecontrol Mode");

  while(1)//25ms
	{
 
		if(g_driveMode == TELECONTROL_MODE)//非无人驾驶状态才能使用遥控器
		{
			if(NRF24L01_RxPacket(wirelessBuf)==0 && (wirelessBuf[31]&0xff) ==1)//收到消息  末位校验
			{
				g_ykSafty_num = 0;//每收到一次消息 计数值清零
				g_carControlMsg = dataConvert(wirelessBuf);

			}
			else
				continue;

		}
		else//无人驾驶模式
		{
			//串口接收上位机指令并进行解析得到控制消息！g_carControlMsg
		}
		
		//speed 0-4096
		if(g_carControlMsg.speed_l>2050)	
			TIM3->CCR1 = 1500+(g_carControlMsg.speed_l-2048)*500*MaxDriveSpeedRateLimit/4096;
		else if(g_carControlMsg.speed_l <2045)
			TIM3->CCR1 = 1500+(g_carControlMsg.speed_l-2048)*500*MaxReverseSpeedRateLimit/4096;
		TIM3->CCR2 = 1500+(g_carControlMsg.angle-2048)*500/4096;//angle
					
		POINT_COLOR=RED;//设置字体为红色 
		LCD_ShowString(78,20,96,16,16,g_driveModeName);	
		
		delay_ms(10);
	}	 
 }
