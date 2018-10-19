/******************** (C) COPYRIGHT  风驰iCreate嵌入式开发工作室 ***************************
 * 文件名  ：
 * 描述    ：中断服务子函数库   
 * 实验平台：iCreate STM8开发板
 * 寄存器版本  ：V1.0.0
 * 作者    ：ling_guansheng  QQ：779814207
 * 博客    ：
 *修改时间 ：2011-12-20



****************************************************************************************/
#include "uart.h"
u8 RxBuffer[RxBufferSize];
u8 UART_RX_NUM=0;
#pragma vector=1
__interrupt void TRAP_IRQHandler(void)
{
  
}
#pragma vector=2
__interrupt void TLI_IRQHandler(void)
{
  
}
#pragma vector=3
__interrupt void AWU_IRQHandler(void)
{
  
}
#pragma vector=4
__interrupt void CLK_IRQHandler(void)
{
  
  
}
#pragma vector=5
__interrupt void EXTI_PORTA_IRQHandler(void)
{
  
}
#pragma vector=6
__interrupt void EXTI_PORTB_IRQHandler(void)
{
  
}
#pragma vector=7
__interrupt void EXTI_PORTC_IRQHandler(void)
{
  
}
#pragma vector=8
__interrupt void EXTI_PORTD_IRQHandler(void)
{
 
  
}
#pragma vector=9
__interrupt void EXTI_PORTE_IRQHandler(void)
{
   
}


#pragma vector=0xC
__interrupt void SPI_IRQHandler(void)
{
  
}
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)
{
  

}
#pragma vector=0xE
__interrupt void TIM1_CAP_COM_IRQHandler(void)
{
  
}

#pragma vector=0xF
__interrupt void TIM2_UPD_OVF_BRK_IRQHandler(void)
{
     
}
#pragma vector=0x10
__interrupt void TIM2_CAP_COM_IRQHandler(void)
{
  
}


#pragma vector=0x11
__interrupt void TIM3_UPD_OVF_BRK_IRQHandler(void)
{
  
}
#pragma vector=0x12
__interrupt void TIM3_CAP_COM_IRQHandler(void)
{
  
}


#pragma vector=0x13
__interrupt void UART2_TX_IRQHandler(void)
{
  
}

#pragma vector=UART2_R_RXNE_vector
__interrupt void UART2_RX_IRQHandler(void)
{ 
 // UART2_SendByte(0xff);
  u8 res;
    if(UART2_SR_RXNE ==1 )
    {
      res = (u8)UART2_DR;
      UART2_SendByte(res);
      UART2_SR_RXNE =0;
    }
    
   
}



/******************* (C) COPYRIGHT 风驰iCreate嵌入式开发工作室 *****END OF FILE****/