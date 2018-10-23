
#include "clk_conf.h"
#include "uart.h"
#include "led.h"
#include "pstwo.h"
#include "delay_.h"


extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;

//PB5������
//PB4ʱ����
//PB3������
//PB2Ƭѡ��
//����PD5 ����
//����PD6 ����

int main(void)
{
  /* Infinite loop */
  u8 i=0;
  u16 Handkey;
  u8 sendBuf[6]={0x12,0x34,0x56};
  /*�����ڲ�����ʱ��16MΪ��ʱ��*/ 
  Clk_conf();
  uart_conf();
    /* LED��ʼ�� */
  LED_conf();
  delay_init(8);
  PS2_Init();//ң����
  EnableInterrupt;
  

  
  while(1)
  {
    if( !PS2_RedLight()) 
	{
        delay_ms(75);
        PS2_RequestData();
        sendBuf[3] = Data[8];
        sendBuf[4] = Data[5];
        //UART2_SendString(sendBuf,5);
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
        //printf("\r\n");
    }
    else
    {
      //printf("�̵�ģʽ\r\n");
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

/******************* (C) COPYRIGHT ���iCreateǶ��ʽ���������� *****END OF FILE****/
