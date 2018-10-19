#include "delay_.h"

volatile u8 fac_us=0; //us��ʱ������ 

void delay_init(u8 clk)
{
 if(clk>16)fac_us=(16-4)/4;//24Mhzʱ,stm8���19������Ϊ1us
 else if(clk>4)fac_us=(clk-4)/4; 
 else fac_us=1;
}
//��ʱnus
//��ʱʱ��=(fac_us*4+4)*nus*(T)
//����,TΪCPU����Ƶ��(Mhz)�ĵ���,��λΪus.
//׼ȷ��:
//92%  @24Mhz
//98%  @16Mhz
//98%  @12Mhz
//86%  @8Mhz
void delay_us(u16 nus)
{  
__asm(
"PUSH A          \n"  //1T,ѹջ
"DELAY_XUS:      \n"   
"LD A,fac_us     \n"   //1T,fac_us���ص��ۼ���A
"DELAY_US_1:     \n"  
"NOP             \n"  //1T,nop��ʱ
"DEC A           \n"  //1T,A--
"JRNE DELAY_US_1 \n"   //������0,����ת(2T)��DELAY_US_1����ִ��,������0,����ת(1T).
"NOP             \n"  //1T,nop��ʱ
"DECW X          \n"  //1T,x--
"JRNE DELAY_XUS  \n"    //������0,����ת(2T)��DELAY_XUS����ִ��,������0,����ת(1T).
"POP A           \n"  //1T,��ջ
); 
} 
//��ʱnms  
//Ϊ��֤׼ȷ��,nms��Ҫ����16640.
void delay_ms(u32 nms)
{
 u8 t;
 if(nms>65)
 {
  t=nms/65;
  while(t--)delay_us(65000);
  //nms=nmse;
 }
 delay_us(nms*1000);
}