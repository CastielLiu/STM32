#include "main.h"

//KEY_UP  				PA0 记录目标点
//KEY0  					PE4 
//KEY1  					PE3 开始、取消无人驾驶


//TIM5 用于计时  				TIM5

//LCD     PB0 PD0 PD1 PD4 PD5 PD8 PD9 PD10 PD14 PD15 
//				PE7-15 PG0 PG12

//预装载值寄存器     TIMx->ARR  
//比较寄存器         TIMx->CCR2 
//CAN				StdId =0x12   ExtId=0x12;

//转向，左负右正
//前轮转角与转弯半径的关系
//radius = AXIS_DIS/sin(θ) -> θ = asin(AXIS_DIS/radius)
#define WHEEL_DIS 0.5
#define AXIS_DIS  0.7
#define Dis_Threshold 3   //距离当前目标点小于Dis_Threshold时，切换到下一个目标点
#define DIS_STEP 0.2  //20cm per point
 

static gps_sphere_t  gps_sphere_start  , gps_sphere_target, gps_sphere_end;

int main(void)
{	
	
 	u16 Adc_value = 0 ;
	float road_wheel_angle=0.0;
	float expect_angle =0.0 , angle_differ = 0.0;
	
	float t_yaw_start =0.0,t_yaw_now=0.0,lateral_err=0.0,yaw_err=0.0;
	gps_rect_t  rectangular;
	float  turning_radius ;
	u8 target_point_seq = 0; //序号
	
	char show_angle[6];
	char show_exAngle[6];
	char show_speed[6]="0.00";
	char show_target_status[7]; //1/4
	char show_segment_status[7];// 1/50
	char show_current_yaw[5];
	char show_expect_yaw[5];
	
	float total_dis =0.0;  //起点到末点的距离  用于片段划分
	int segment_num=0 , segment_seq=0; //起点到末点的分段数，当前所处段数
	float lat_step,lon_step; //经纬度的步长
	u8 switch_lastpoint_flag=1;
	
	float tempFloat=0.0;
	

	LED0 = 0;
	system_init();
	
	POINT_COLOR=BLACK;//设置字体为黑色
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*0 ,6*LCD_FOND_SIZE/2 ,LCD_FOND_SIZE,LCD_FOND_SIZE,"mode :");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*1 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"A    :             Lon");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*2 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"                   Lat");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*3 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"B    :             Lon");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*4 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"                   Lat");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*5 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"C    :             Lon");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*6 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"                   Lat");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*7 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"D    :             Lon");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*8 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"                   Lat");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*9 ,35*LCD_FOND_SIZE/2 ,LCD_FOND_SIZE,LCD_FOND_SIZE,"angle:     deg  exAngle:     deg");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*10,16*LCD_FOND_SIZE/2 ,LCD_FOND_SIZE,LCD_FOND_SIZE,"speed:     km/h");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*11,15*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"target  status:");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*12,15*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"segment status:");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*13,12*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"current yaw:");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*14,12*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"expect  yaw:");
	
  while(1)//25ms
	{
		if(g_start_driverless_flag ==0) 
		{
			strcpy(g_mode_name,"Record Point");
			//debug...
			if((tempFloat =getCurrentRoadWheelAngle()) <50.0)//角度获取失败时值为180.0 
				road_wheel_angle = tempFloat;
			steer_control(g_debugRoadWheelAngle - road_wheel_angle);
			expect_angle = g_debugRoadWheelAngle;
		}
		else//无人驾驶模式
		{
			strcpy(g_mode_name,"Driverless  ");
			//以下内容在一个步长内只需要计算一次
			if(switch_lastpoint_flag == 1)  //第一次进入程序时flag=1；
			{
				switch_lastpoint_flag =0; //reset
				
				if(target_point_seq == 0)
					gps_sphere_start =  target_point[g_actual_path_vertwx_num-1];
				else
					gps_sphere_start = target_point[target_point_seq-1];
				gps_sphere_end = target_point[target_point_seq];
				total_dis = point2point_dis(gps_sphere_end,gps_sphere_start);
				segment_num = total_dis/DIS_STEP;
				lat_step = (gps_sphere_end.lat - gps_sphere_start.lat)/segment_num ;
				lon_step = (gps_sphere_end.lon - gps_sphere_start.lon)/segment_num ;
			}
			//以上内容在一个步长内只需要计算一次			
			if(segment_seq>segment_num)
			{
				switch_lastpoint_flag = 1;
				segment_seq = 0;
				target_point_seq ++;
				if(target_point_seq >g_actual_path_vertwx_num-1)
					 target_point_seq =0;
				continue ;  //务必continue  因为需要重新计算以下代码所需参数
			}
			
			
			gps_sphere_target.lon = gps_sphere_start.lon + lon_step*segment_seq;  //计算当前目标点经纬度
			gps_sphere_target.lat = gps_sphere_start.lat + lat_step*segment_seq;
			
			if(point2point_dis(g_gps_sphere_now,gps_sphere_target)<Dis_Threshold )
			{
				segment_seq++;//切换目标点
				continue;
			}
			
			
			relative_XY_dis_yaw(g_gps_sphere_now,gps_sphere_target,&rectangular);//计算当前期望航向和离目标点的距离

			//lateral_err = LateralError(t_yaw_start,rectangular.yaw,rectangular.distance);
			
			yaw_err = rectangular.yaw - g_gps_sphere_now.yaw; //当前航向和期望航向的偏差
			
			sprintf(show_expect_yaw,"%3.1f",rectangular.yaw*180/3.1415926);
			LCD_ShowString(LCD_LU_X+12*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*14,5*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_expect_yaw);
			
			if(yaw_err ==0.0)
				continue;
		
			turning_radius = -0.5 *rectangular.distance /sin(yaw_err); //根据汽车模型计算转弯半径与前轮转角的关系
			//turning_radius *=100;//to cm 
			
			if((tempFloat =getCurrentRoadWheelAngle()) <50.0)
				road_wheel_angle = tempFloat;

			g_s16_steer_angle = road_wheel_angle*100; 
			
		//不同汽车模型修改以下代码				
			expect_angle = asin(AXIS_DIS/turning_radius) *180/pi;
			angle_differ = expect_angle - road_wheel_angle;
			steer_control(angle_differ);
			
		//	不同汽车模型修改以上代码
		}
		
//显示				
		POINT_COLOR=RED;//设置字体为红色 
		sprintf(show_angle,"%2.1f",road_wheel_angle);
		LCD_ShowString(LCD_LU_X+6*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*0,12*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,g_mode_name);	
		LCD_ShowString(LCD_LU_X+6*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*9,5*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_angle);
		
		sprintf(show_exAngle,"%2.1f",expect_angle);
		LCD_ShowString(LCD_LU_X+24*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*9,5*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_exAngle);
		
		sprintf(show_speed,"%2.2f",g_vehicleSpeed);
		LCD_ShowString(LCD_LU_X+6*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*10,5*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_speed);
		sprintf(show_target_status,"%d/%d",target_point_seq+1,g_actual_path_vertwx_num);
		LCD_ShowString(LCD_LU_X+15*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*11,8*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_target_status);
		sprintf(show_segment_status,"%d/%d",segment_seq,segment_num);
		LCD_ShowString(LCD_LU_X+15*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*12,8*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_segment_status);
		sprintf(show_current_yaw,"%3.1f",g_gps_sphere_now.yaw*180/3.1415926);
		LCD_ShowString(LCD_LU_X+12*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*13,5*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_current_yaw);
	
		delay_ms(30);
		//printf("lon:%3.7f\tlat:%3.7f\r\n",g_gps_sphere_now.lon*180.0/pi,g_gps_sphere_now.lat*180.0/pi);
		LCD_ShowxNum(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*15,(u32)(rectangular.distance),3,LCD_FOND_SIZE,0);
	}	 
	
 }
