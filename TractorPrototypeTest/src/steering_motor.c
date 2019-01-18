#include "steering_motor.h"
#include "global_params.h"


static uint8_t g_setSpeed_8bytesCmd[8] = {0x01,0x06,0x00,0x6a,0x00,0x00}; //2 bytes speed 2bytes check_num
static uint8_t g_setRotate_13bytesCmd[13] = {0x01,0x10,0x01,0x36,0x00,0x02,0x04};//4 byte pulseNum 2bytes check_num
										//0x0136 positive   0x0135 nagetive

static uint8_t g_getAdValue_8bytesCmd[8]  = {0x01,0x03,0x40,0x0D,0x00,0x01,0x00,0x09}; //-> 01 03 02 {0A 32} 3F 31  16bits AD值 


const uint16_t CRC16Table[]={
0x0,    0xc0c1, 0xc181, 0x140,  0xc301, 0x3c0,  0x280,  0xc241, 0xc601, 0x6c0,  0x780,  0xc741, 0x500,  0xc5c1, 0xc481,
0x440,  0xcc01, 0xcc0,  0xd80,  0xcd41, 0xf00,  0xcfc1, 0xce81, 0xe40,  0xa00,  0xcac1, 0xcb81, 0xb40,  0xc901, 0x9c0,
0x880,  0xc841, 0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40, 0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01,
0x1dc0, 0x1c80, 0xdc41, 0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641, 0xd201, 0x12c0, 0x1380, 0xd341,
0x1100, 0xd1c1, 0xd081, 0x1040, 0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240, 0x3600, 0xf6c1, 0xf781,
0x3740, 0xf501, 0x35c0, 0x3480, 0xf441, 0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41, 0xfa01, 0x3ac0,
0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840, 0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41, 0xee01,
0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40, 0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041, 0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281,
0x6240, 0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441, 0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0,
0x6e80, 0xae41, 0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840, 0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01,
0x7bc0, 0x7a80, 0xba41, 0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40, 0xb401, 0x74c0, 0x7580, 0xb541,
0x7700, 0xb7c1, 0xb681, 0x7640, 0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041, 0x5000, 0x90c1, 0x9181,
0x5140, 0x9301, 0x53c0, 0x5280, 0x9241, 0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440, 0x9c01, 0x5cc0,
0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40, 0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841, 0x8801,
0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40, 0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641, 0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081,
0x4040};

void sendControlCmd(const uint8_t * dataPtr,int size)
{
	int i = 0;
	for(;i<size;i++)
	{
		while((USART2->SR&0X40)==0); //发送完成标志位
		USART_SendData(USART2,dataPtr[i]);
		//USART1->DR = dataPtr[i];
		//printf("%x\t",dataPtr[i]);
	}
	//printf("\r\n");
	//printf("sendCmd\r\n");
}

uint16_t generateModBusCRC_byTable(const uint8_t *ptr,uint8_t size)
{
	uint16_t CRC16  = 0xffff;
    uint16_t tableNo = 0;
	int i = 0;
 
    for( ; i < size; i++)
    {
        tableNo = ((CRC16 & 0xff) ^ (ptr[i] & 0xff));
        CRC16  = ((CRC16 >> 8) & 0xff) ^ CRC16Table[tableNo];
    }
 
    return CRC16;  
} 
void setSteeringSpeed(uint8_t speed)
{
	uint16_t CRC_checkNum ;
	if(speed >100) 
		speed = 100;
	g_setSpeed_8bytesCmd[5] = speed & 0xff;
	CRC_checkNum = generateModBusCRC_byTable(g_setSpeed_8bytesCmd,6);
	g_setSpeed_8bytesCmd[6] = CRC_checkNum &0xff;
	g_setSpeed_8bytesCmd[7] = CRC_checkNum >>8;
	sendControlCmd(g_setSpeed_8bytesCmd,8); 
}

void setSteeringRotate(float cycleNum)
{ 
	uint16_t CRC_checkNum ;
	
	int pulseNum = -cycleNum*32768;
	if(pulseNum < 0)
		g_setRotate_13bytesCmd[3] = 0x36; //clock wise ID
	else
		g_setRotate_13bytesCmd[3] = 0x35; //
	g_setRotate_13bytesCmd[7] = (pulseNum>>8)&0xFF;
	g_setRotate_13bytesCmd[8] = (pulseNum>>0)&0xFF;
	g_setRotate_13bytesCmd[9] = (pulseNum>>24)&0xFF;
	g_setRotate_13bytesCmd[10] = (pulseNum>>16)&0xFF;
	
	CRC_checkNum = generateModBusCRC_byTable(g_setRotate_13bytesCmd,11);
	g_setRotate_13bytesCmd[11] = CRC_checkNum &0xff;
	g_setRotate_13bytesCmd[12] = CRC_checkNum >>8;
	sendControlCmd(g_setRotate_13bytesCmd,13); 
}

#if GET_AD_MODE==GET_AD_MODE0
uint16_t getAdcValue(void)  
{
	uint8_t i=0;
	uint8_t buf[7];
	uint8_t temp;
	uint16_t CRC_checkNum;
	sendControlCmd(g_getAdValue_8bytesCmd,8);
	while(((USART2->SR>>4)&0x01)!=1)
		;//printf("not ideal\r\n"); //空闲标志未置位
	temp = USART2->SR;
	temp = USART2->DR;//清除空闲标志
	while(1)
	{
		if((USART2->SR >>5)&0x01) //读数据寄存器非空
		{
			buf[i] = (USART2->DR) &0xff;
			//printf("%x\t",buf[i]);
			i++;
			if(i == 7) //读取完毕
				break;
		}		
	}
	CRC_checkNum = generateModBusCRC_byTable(buf,5);
	//printf("i = %d\t%x\t%x\t%x\t%x\t%x\t%x\t%x\r\n",i,buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6]);
	if((CRC_checkNum&0xff) == buf[5] && (CRC_checkNum>>8) == buf[6])
		return buf[3]*256 + buf[4];
	else
	{
		printf("读取AD值失败\r\n");
		return 0; //error
	}
}

#elif GET_AD_MODE==GET_AD_MODE1
uint16_t getAdcValue(void)  
{
	uint8_t i=0;
	const int bufLen = 14;//两倍包长
	uint8_t buf[bufLen] ;
	uint8_t *headPtr;
	uint16_t CRC_checkNum;
	sendControlCmd(g_getAdValue_8bytesCmd,8);
	while(1)
	{
		if((USART2->SR >>5)&0x01) //读数据寄存器非空
		{
			buf[i] = (USART2->DR) &0xff;
			
			if(i >= 6 && buf[i-6]==0x01 &&buf[i-5]==0x03 && buf[i-4]==0x02) 
			{
				headPtr = &buf[i-6];
				break;
			}
			i++;
			if(i==bufLen)
			{
				printf("i=%d\t读取AD值数组溢出失败\r\n",i);
				return 0; //error
			}
		}		
	}
	CRC_checkNum = generateModBusCRC_byTable(headPtr,5);
	if((CRC_checkNum&0xff) == headPtr[5] && (CRC_checkNum>>8) == headPtr[6])
		return headPtr[3]*256 + headPtr[4];
	else
	{
		printf("读取AD值校验失败\r\n");
		printf("i = %d\t%x\t%x\t%x\t%x\t%x\t%x\t%x\r\n",i,headPtr[0],headPtr[1],headPtr[2],headPtr[3],headPtr[4],headPtr[5],headPtr[6]);
		return 0; //error
	}
}

#elif GET_AD_MODE==GET_AD_MODE1_OutTimeVesion
uint16_t getAdcValue(void)  
{
	uint8_t i=0;
	int timeOut = 10000;
	const int bufLen = 14;//两倍包长
	uint8_t buf[bufLen] ;
	uint8_t *headPtr;
	uint16_t CRC_checkNum;
	sendControlCmd(g_getAdValue_8bytesCmd,8);
	while(--timeOut)
	{
		if((USART2->SR >>5)&0x01) //读数据寄存器非空
		{
			buf[i] = (USART2->DR) &0xff;
			
			if(i >= 6 && buf[i-6]==0x01 &&buf[i-5]==0x03 && buf[i-4]==0x02) 
			{
				headPtr = &buf[i-6];
				break;
			}
			i++;
			if(i==bufLen)
			{
				printf("i=%d\t读取AD值数组溢出失败\r\n",i);
				return 0; //error
			}
		}
		delay_us(50);
	}
	if(timeOut == 0) 
	{
		printf("AD get timeout! i = %d\r\n",i);
		return 0;
	}
	CRC_checkNum = generateModBusCRC_byTable(headPtr,5);
	if((CRC_checkNum&0xff) == headPtr[5] && (CRC_checkNum>>8) == headPtr[6])
		return headPtr[3]*256 + headPtr[4];
	else
	{
		printf("读取AD值校验失败\r\n");
		printf("i = %d\t%x\t%x\t%x\t%x\t%x\t%x\t%x\r\n",i,headPtr[0],headPtr[1],headPtr[2],headPtr[3],headPtr[4],headPtr[5],headPtr[6]);
		return 0; //error
	}
}

#elif GET_AD_MODE==GET_AD_MODE3
uint16_t getAdcValue(void)
{
	uint8_t i=0;
	const int bufLen = 14;//两倍包长
	uint8_t buf[bufLen] ;
	uint8_t *headPtr;
	
	uint8_t temp;
	uint16_t CRC_checkNum;
	int timeOut = 10000;  //600ms 超时  太长！
	sendControlCmd(g_getAdValue_8bytesCmd,8);
	
	while(--timeOut)
	{
		if((USART2->SR >>5)&0x01) //读数据寄存器非空
		{
			buf[i] = (USART2->DR) &0xff;
			i++;
		}
		else if(((USART2->SR>>4)&0x01)==1)//总线空闲
		{
			temp = USART2->SR;  temp = USART2->DR;
			if(i>=7)
			{
				headPtr = &buf[i-7];
				break;
			}
		}
		delay_us(50); //115200,69us/byte,延时时间应小于69us 防止漏掉字节！
	}
	if(timeOut==0)
	{
		u8 canSendTimeOut = 0xff;
		Can_Send_Msg(0x3C0,&canSendTimeOut,1);
		printf("AD get timeout! i = %d\r\n",i);
		return 0;
	}
	CRC_checkNum = generateModBusCRC_byTable(headPtr,5);
	if((CRC_checkNum&0xff) == headPtr[5] && (CRC_checkNum>>8) == headPtr[6])
		return headPtr[3]*256 + headPtr[4];
	else
	{
		printf("AD校验失败\r\n");
		printf("i = %d\t%x\t%x\t%x\t%x\t%x\t%x\t%x\r\n",i,buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6]);
		return 0; //error
	}
}

#elif GET_AD_MODE==GET_AD_MODE_DEBUG
uint16_t getAdcValue(void) 
{
	return 2048;
}

#endif

float getCurrentRoadWheelAngle(void)
{
	uint16_t adValue = getAdcValue();
	if(adValue==0) return 180.0; //error
	else
		return 1.0*g_roadWheelAngle_dir * (adValue - g_AngleSensorAdValueOffset)*g_AngleSensorMaxAngle/g_AngleSensorMaxAdValue ;
}


uint16_t getErrorMsg()
{
	uint8_t i=0;
	uint8_t buf[7];
	uint16_t CRC_checkNum;
	uint8_t getErrorMsg_8bytesCmd[8] = {0x01,0x03,0x01,0x1F,0x00,0x01,0xB4,0x30}; //-> 01 03 02 {00 0E} 39 80  errorMsg
	sendControlCmd(getErrorMsg_8bytesCmd,8);
	while(1)
	{
		if((USART2->SR >>5)&0x01) //读数据寄存器非空
		{
			buf[i] = (USART2->DR) &0xff;
			i++;
			if(i == 7)
				break;
		}		
	}
	CRC_checkNum = generateModBusCRC_byTable(buf,5);
	if((CRC_checkNum&0xff) == buf[5] && (CRC_checkNum>>8) == buf[6])
		return (buf[3]*256 + buf[4]);
	else
		return 0xffff;
	
}

void steeringEnable()
{
	uint8_t enable_11bytesCmd[11]={0x01,0x10,0x00,0x33,0x00,0x01,0x02,0x00,0x01,0x62,0x53};//byte1 0x10 ??????  
																				//bytes 5 0x01 enable  0x00 disable
	sendControlCmd(enable_11bytesCmd,11);
}

void steeringDisable()
{
	uint8_t disable_11bytesCmd[11]={0x01,0x10,0x00,0x33,0x00,0x01,0x02,0x00,0x00,0xA3,0x93};
	sendControlCmd(disable_11bytesCmd,11);
}


//方向盘转一圈前轮改变 角度值
static const float degreePerCycle = 20.0;
void steer_control(float angle_diff)
{
	float cycleNum = - angle_diff / degreePerCycle;
	setSteeringSpeed(20); //10rpm
	delay_ms(1);
	setSteeringRotate(cycleNum);
}






