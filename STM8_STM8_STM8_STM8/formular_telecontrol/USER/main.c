
#include "clk_conf.h"
#include "uart.h"
#include "led.h"
#include "pstwo.h"
#include "delay_.h"


extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;

u8 generate_check_sum(u8 *buf,int len)
{
  u8 sum=0;
  int i=0;
  for(;i<len;i++)
  {
    sum += *(buf+i);
  }
  return sum;
}

//PB5数据线
//PB4时钟线
//PB3命令线
//PB2片选线
//串口PD5 发送
//串口PD6 接收

int main(void)
{
  /* Infinite loop */
  u16 Handkey;
  u8 sendBuf[12] = {0x66,0xCC};
  /*设置内部高速时钟16M为主时钟*/ 
  Clk_conf();
  uart_conf();
    /* LED初始化 */
  LED_conf();
  delay_init(8);
  PS2_Init();//遥控器
  EnableInterrupt;
  
  int i=0;
  
  while(1)
  {
    if( !PS2_RedLight()) 
	{
        delay_ms(50);
        PS2_RequestData();
        for(i=0;i<9;i++)
          sendBuf[2+i] = Data[i];
        sendBuf[11] = generate_check_sum(sendBuf,11);
        
        UART2_SendString(sendBuf,12);
      /*  
        Handkey=(Data[4]<<8)|Data[3];
        if((Handkey&(1<<(MASK[PSB_SQUARE-1]-1)))==0)
        { 
          LED0 = 1;
          LED1 = 0;
        }
        else if((Handkey&(1<<(MASK[PSB_CIRCLE-1]-1)))==0)
        {
          LED1 = 1;
          LED0 = 0;
        }
        else if((Handkey&(1<<(MASK[PSB_CROSS-1]-1)))==0)
        {
          LED1 = 0;
          LED0 = 0;
        }
        printf("111\r\n");*/
    }
    else
    {
      //printf("绿灯模式\r\n");
      delay_ms(30);
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
