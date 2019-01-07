#include <stdio.h> 
#include <stdint.h>

uint16_t CRC16Table[]={
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
	for(int i=0;i<size;i++)
		printf("%02x\t",dataPtr[i]);
	printf("  end\r\n");
}

uint16_t generateModBusCRC_byTable(const uint8_t *ptr,uint8_t size)
{
	uint16_t CRC16  = 0xffff;
    uint16_t tableNo = 0;
 
    for(int i = 0; i < size; i++)
    {
        tableNo = ((CRC16 & 0xff) ^ (ptr[i] & 0xff));
        CRC16  = ((CRC16 >> 8) & 0xff) ^ CRC16Table[tableNo];
    }
 
    return CRC16;  
} 

uint8_t g_setSpeed_8bytesCmd[8] = {0x01,0x06,0x00,0x6a,0x00,0x00}; //2 bytes speed 2bytes check_num
uint8_t g_setRotate_13bytesCmd[13] = {0x01,0x10,0x01,0x36,0x00,0x02,0x04};//4 byte pulseNum 2bytes check_num
										//0x0136 positive   0x0135 nagetive

uint8_t g_getErrorMsg_8bytesCmd[8] = {0x01,0x03,0x01,0x1F,0x00,0x01,0xB4,0x30}; //-> 01 03 02 {00 0E} 39 80  错误消息 
uint8_t g_getAdValue_8bytesCmd[8]  = {0x01,0x03,0x40,0x0D,0x00,0x01,0x00,0x09}; //-> 01 03 02 {0A 32} 3F 31  16位AD值 

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
{//-32768为顺时针转1圈，32768为逆时针1圈
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

void getAdcValue()
{
	sendControlCmd(g_getAdValue_8bytesCmd,8);
}

void steeringEnable()
{
	uint8_t enable_11bytesCmd[11]={0x01,0x10,0x00,0x33,0x00,0x01,0x02,0x00,0x01,0x62,0x53};//byte1 0x10 写多个寄存器  
																				//bytes 5 0x01 enable  0x00 disable
	sendControlCmd(enable_11bytesCmd,11);
}



int main()
{
	setSteeringSpeed(100);
	setSteeringRotate(1.0);
	void getAdcValue();
}













