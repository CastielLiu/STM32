
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

//PB5������
//PB4ʱ����
//PB3������
//PB2Ƭѡ��
//����PD5 ����
//����PD6 ����

int main(void)
{
  /* Infinite loop */
  u16 Handkey;
  u8 sendBuf[12] = {0x66,0xCC};
  /*�����ڲ�����ʱ��16MΪ��ʱ��*/ 
  Clk_conf();
  uart_conf();
    /* LED��ʼ�� */
  LED_conf();
  delay_init(8);
  PS2_Init();//ң����
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
