#include "function.h"
#include "usart.h"
#include "lcd.h"
#include "beep.h"



void clearBKP(void)
{
	uint16_t _BKP_DRx;
	
	for(_BKP_DRx = BKP_DR2; _BKP_DRx!=BKP_DR42; _BKP_DRx +=4 )
		BKP_WriteBackupRegister(_BKP_DRx, 0x0000);
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
	else g_start_driverless_flag =0;
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
		
			//BEEP = 1; delay_ms(300); BEEP = 0; delay_ms(300); BEEP = 1 ; delay_ms(300); BEEP = 0; 
			Can_Send_Msg(0x3C0,&can3C0Buf,1);
		}
		
	}
}
