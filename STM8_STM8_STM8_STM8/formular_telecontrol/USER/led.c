

#include "led.h"


void LED_conf(void)
{
  PD_DDR|=0x0c; //PD2  PD3  /* �������ݷ���Ĵ��� 1Ϊ�����0Ϊ����--�鿴STM8�Ĵ���.pdf P87 */
  PD_CR1|=0x0c;   /* �����������--�鿴STM8�Ĵ���.pdf P88 */
  PD_CR2|=0x0c;   /* �������Ƶ�� 1Ϊ10M��0Ϊ2M--�鿴STM8�Ĵ���.pdf P89 */
  PD_ODR&=0xf3; //2,3��������
}

