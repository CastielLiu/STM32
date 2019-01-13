#include "function.h"
#include "usart.h"
#include "lcd.h"
#include "beep.h"

#define MaxCommaNumInGpsData  12

void clearBKP(void)
{
	uint16_t _BKP_DRx;
	
	for(_BKP_DRx = BKP_DR2; _BKP_DRx!=BKP_DR42; _BKP_DRx +=4 )
		BKP_WriteBackupRegister(_BKP_DRx, 0x0000);
}

void GetComma(const char *str,u8 *commaLocation_)
{
	u8 i = 0,j=1;
	for(;i<strlen(str);i++)
	{
		if(*(str+i)==',')
		{
			commaLocation_[j] = i;
			j++;
			if(j>MaxCommaNumInGpsData-1) break;
			
		}
	}
	
}

u8 gpsParse(const char *buf)
{
	u8 commaLocation[MaxCommaNumInGpsData];
	char tempBuf[20];
	double tempLat,tempLon,tempYaw;
	if(NULL == strstr(buf,"$GPRMC"))
		return 2;
	
	GetComma(buf,commaLocation);
	strncpy(tempBuf,&buf[commaLocation[2]+1],commaLocation[3]-commaLocation[2]-1);
	g_gps_status = tempBuf[0];
	//printf("%s\r\n",buf);
	if('A' == g_gps_status)//数据有效
	{
		strncpy(tempBuf,&buf[commaLocation[3]+1],commaLocation[4]-commaLocation[3]-1);
		tempLat = atof(tempBuf);
		strncpy(tempBuf,&buf[commaLocation[5]+1],commaLocation[6]-commaLocation[5]-1);
		tempLon = atof(tempBuf);
		
		tempLat = (tempLat - (int)(tempLat /100 )*100 )/60 + (int)(tempLat /100 ) ;
		tempLon = (tempLon - (int)(tempLon /100 )*100 )/60 + (int)(tempLon /100 ) ;
		
		if(tempLat >= 90 || tempLat<= 0 || tempLon>=180 || tempLon<=0 ) return 3;
		
		strncpy(tempBuf,&buf[commaLocation[8]+1],commaLocation[9]-commaLocation[8]-1);
		tempYaw = atof(tempBuf);
		
		//printf("yaw = %f\r\n",tempYaw);
		if(tempYaw >360.0 || tempYaw<0.0) return 3;
		
		strncpy(tempBuf,&buf[commaLocation[7]+1],commaLocation[8]-commaLocation[7]-1);
		
		g_vehicleSpeed = atof(tempBuf)* 1.85  ; //1海里=1.85公里;
		
		g_int_lon = tempLon *10000000;
		g_int_lat = tempLat *10000000;
		g_u16_yaw = tempYaw *100;
		//g_s16_speed = 10;//km/h  放大100倍
		//g_int_height = 9.0 *1000 ;	
		
		
		g_gps_sphere_now.lat = tempLat*pi/180.0;
		g_gps_sphere_now.lon = tempLon*pi/180.0;
		g_gps_sphere_now.yaw = tempYaw*pi/180.0;
 
		
	//	printf("lat=%f\tlon=%fyaw=%f\r\n",g_gps_sphere_now.lat,g_gps_sphere_now.lon,g_gps_sphere_now.yaw);
		return 0;
	}
	else
		return 1;
}


void startDriverless()
{
	u8 can3C0Buf = START_DRIVERLESS;
	if(g_start_driverless_flag==1) return;
	
	g_actual_path_vertwx_num = g_recordTargetSeq;
	if(g_actual_path_vertwx_num >2)
	{
		g_start_driverless_flag =1;
		Can_Send_Msg(0x3C0,&can3C0Buf,1);
	}
}
void pauseDriverless()
{
	u8 can3C0Buf = PAUSE_DRIVERLESS;
	if(g_start_driverless_flag==0) return;
	Can_Send_Msg(0x3C0,&can3C0Buf,1);
}

void recordTargetPoint()
{
	char show_lon[12];
	char show_lat[12];
	float	dis ;
	u8 can3C0Buf = RECORD_TARGET_POINT;
	
	if(g_recordTargetSeq ==0 )
		dis= g_MinDisBetweenTwoTarget  +1.5;//距离设置为大于record_dis_threshold的值
	else
		dis = point2point_dis(target_point[g_recordTargetSeq-1],g_gps_sphere_now);

	if(g_gps_sphere_now.lat==0.0 || g_gps_sphere_now.lon == 0.0)
	{
		LCD_ShowString(LCD_LU_X+25*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*1+(g_recordTargetSeq)*LCD_FOND_SIZE*2,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"data error!");
		return;
	}
	if(g_start_driverless_flag ==0)	 	 //未处于无人驾驶状态，防止无人驾驶时误触按键导致目标点重新记录
	{	
		//两次记录目标点小于record_dis_threshold  m时相当于上一个点重新记录 	
		if(dis > g_MinDisBetweenTwoTarget )//第一个目标点理应记录到0位置  
		{
			target_point[g_recordTargetSeq] = g_gps_sphere_now;
			sprintf(show_lon,"%03.7f",target_point[g_recordTargetSeq].lon*180/3.1415926);
			sprintf(show_lat,"%03.7f",target_point[g_recordTargetSeq].lat*180/3.1415926);
		//clear
			LCD_ShowString(LCD_LU_X+25*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*1+(g_recordTargetSeq)*LCD_FOND_SIZE*2,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"           ");

			LCD_ShowString(LCD_LU_X+6*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*1+(g_recordTargetSeq)*LCD_FOND_SIZE*2,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_lon);
			LCD_ShowString(LCD_LU_X+6*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*2+(g_recordTargetSeq)*LCD_FOND_SIZE*2,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_lat);
		
			g_recordTargetSeq++;	
			//printf("%f\t%f\tdis=%f\r\n",g_gps_sphere_now.lon*180/3.1415926,g_gps_sphere_now.lat*180/3.1415926,dis);
		
			BEEP = 1; delay_ms(300); BEEP = 0; delay_ms(300); BEEP = 1 ; delay_ms(300); BEEP = 0; 
			Can_Send_Msg(0x3C0,&can3C0Buf,1);
		}
		
	}
}
