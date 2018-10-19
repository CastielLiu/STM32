
#include "clk_conf.h"
#include "uart.h"
#include "led.h"
#include "pstwo.h"
#include "delay_.h"


extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;

//PB5数据线
//PB4时钟线
//PB3命令线
//PB2片选线
//串口PD5 发送
//串口PD6 接收

int main(void)
{
  /* Infinite loop */
  u8 i=0;
  u8 sendBuf[6]={0x12,0x34,0x56};
  /*设置内部高速时钟16M为主时钟*/ 
  Clk_conf();
  uart_conf();
    /* LED初始化 */
  LED_conf();
  delay_init(8);
  PS2_Init();//遥控器
  EnableInterrupt;
  

  
  while(1)
  {
    if( !PS2_RedLight()) 
	{
        delay_ms(50);
        PS2_RequestData();
        sendBuf[3] = Data[8];
        sendBuf[4] = Data[5];
        UART2_SendString(sendBuf,5);
    }
    else
    {
      //printf("绿灯模式\r\n");
      delay_ms(50);
    }

  }
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 风驰iCreate嵌入式开发工作室 *****END OF FILE****/
