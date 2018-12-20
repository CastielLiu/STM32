#include "main.h"

//KEY_UP  				PA0 ��¼Ŀ���
//KEY0  					PE4 
//KEY1  					PE3 ��ʼ��ȡ�����˼�ʻ


//��೵��ת�Ǵ�����    PA1   ADC1 CH1
//�Ҳ೵��ת�Ǵ�����    PA4   ADC1 CH4
//PWMת�����				PA7  	TIM3_CH2
//�������					PB2
//���ʹ��  				PF11

//TIM5 ���ڼ�ʱ  				TIM5

//spi      PB12 PB13 PB14 PB15
//24L01    PG6 PG7 PG8
//LCD     PB0 PD0 PD1 PD4 PD5 PD8 PD9 PD10 PD14 PD15 
//				PE7-15 PG0 PG12

//Ԥװ��ֵ�Ĵ���     TIMx->ARR  
//�ȽϼĴ���         TIMx->CCR2 
//CAN				StdId =0x12   ExtId=0x12;

//ת��������
//ǰ��ת����ת��뾶�Ĺ�ϵ
//radius = AXIS_DIS/sin(��) -> �� = asin(AXIS_DIS/radius)
#define WHEEL_DIS 0.5
#define AXIS_DIS  0.7
#define Dis_Threshold 3   //���뵱ǰĿ���С��Dis_Thresholdʱ���л�����һ��Ŀ���
#define DIS_STEP 0.2  //20cm per point
 




static gps_sphere_t  gps_sphere_start  , gps_sphere_target, gps_sphere_end;

static u8 g_ykSafty_num = 0;



extern u16 ADC_Value_Arr[ADC_AVERAGE_CNT][2];

int main(void)
{	
	u16 num_use_to_for_cycle =0;
	u8 wirelessBuf[32];	 //ң�������ݽ��ջ���	
	int print_count =0;
	
 	u16 Adc_value = 0 ;
	u32 Left_front_wheel_Adc_sum=0 , Right_front_wheel_Adc_sum =0;
	float Left_front_wheel_angle = 0.0 , Right_front_wheel_angle=0.0;
	
	float  Left_sensor_voltage =0.0 ,Right_sensor_voltage =0.0;
	
	float  Left_sensor_mid_voltage= 1.6; //ת���е��ѹֵ �궨 
	
	float  Right_sensor_mid_voltage= 1.6; 
	
	float  angle_increment = 360.0/3.3; //������ÿ�仯 1V �Ƕ�����
	
	float Left_wheel_angle =0.0 , Right_wheel_angle = 0.0;

	
	float expect_angle =0.0 , angle_differ = 0.0;
	
#if(YKMode==1)
	carControlData_t	carControlMsg = {2048,2048,2048}; //��ʼ�����٣�ת���Ϊ0
#endif
	float t_yaw_start =0.0,t_yaw_now=0.0,lateral_err=0.0,yaw_err=0.0;
	gps_rect_t  rectangular;
	float  turning_radius ;
	u8 target_point_seq = 0; //���
	float car_speed =0.0;
	
	char show_angle[6];
	char show_speed[6]="0.00";
	char show_target_status[7]; //1/4
	char show_segment_status[7];// 1/50
	char show_current_yaw[5];
	char show_expect_yaw[5];
	
	float total_dis =0.0;  //��㵽ĩ��ľ���  ����Ƭ�λ���
	int segment_num=0 , segment_seq=1; //��㵽ĩ��ķֶ�������ǰ��������
	float lat_step,lon_step; //��γ�ȵĲ���
	u8 switch_lastpoint_flag=1;
	

	LED0 = 0;
	system_init();
///////////////////////////  ң�������	
#if(YKMode==1)
	NRF24L01_Init();    		//��ʼ��NRF24L01 
	while(NRF24L01_Check())
	{
 		delay_ms(200);
		LED1 =!LED1;
		LED0=!LED0;
		LCD_ShowString(20,20,26*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"please check the WIRELESS!")
	}
	NRF24L01_RX_Mode();	
#endif
	
	POINT_COLOR=BLACK;//��������Ϊ��ɫ
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*0 ,6*LCD_FOND_SIZE/2 ,LCD_FOND_SIZE,LCD_FOND_SIZE,"mode :");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*1 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"A    :             Lon");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*2 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"                   Lat");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*3 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"B    :             Lon");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*4 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"                   Lat");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*5 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"C    :             Lon");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*6 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"                   Lat");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*7 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"D    :             Lon");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*8 ,22*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"                   Lat");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*9 ,6*LCD_FOND_SIZE/2 ,LCD_FOND_SIZE,LCD_FOND_SIZE,"angle:");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*10,6*LCD_FOND_SIZE/2 ,LCD_FOND_SIZE,LCD_FOND_SIZE,"speed:");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*11,15*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"target status :");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*12,15*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"segment status:");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*13,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"curret yaw:");
	LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*14,11*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,"expect yaw:");
	
  while(1)//25ms
	{
		for(num_use_to_for_cycle=0,Left_front_wheel_Adc_sum=0,Right_front_wheel_Adc_sum =0;
			num_use_to_for_cycle<ADC_AVERAGE_CNT ; num_use_to_for_cycle++)
		{
			Left_front_wheel_Adc_sum +=  ADC_Value_Arr[num_use_to_for_cycle][0];
			Right_front_wheel_Adc_sum += ADC_Value_Arr[num_use_to_for_cycle][1];
			//printf("%d\t%d\r\n" ,ADC_Value_Arr[num_use_to_for_cycle][0], ADC_Value_Arr[num_use_to_for_cycle][1]);
		}
		
		Adc_value = Left_front_wheel_Adc_sum/ADC_AVERAGE_CNT;
		Left_sensor_voltage = Adc_value*1.0/4095 *3.3;
		Left_wheel_angle = (Left_sensor_voltage - Left_sensor_mid_voltage) * angle_increment;
		 
		//printf("%d\t",Adc_value);
		Adc_value = Right_front_wheel_Adc_sum/ADC_AVERAGE_CNT;
		Right_sensor_voltage = Adc_value*1.0/4095 *3.3;
		Right_wheel_angle = (Right_sensor_voltage - Right_sensor_mid_voltage) * angle_increment;
		
		//printf("%d\r\n\r\n",Adc_value);
		//printf("L=%f\tR=%f\r\n",Left_wheel_angle,Right_wheel_angle);
		
		//printf("L_angle_sensor_voltage=%f\tR_angle_sensor_voltage=%f\r\n",Left_sensor_voltage,Right_sensor_voltage);
		
		send_steer_angle = abs(Left_wheel_angle) * 90; //����90��
		//printf("angle_sensor_voltage=%f\tcurrent_angle=%f\tsend_steer_angle=%d\r\n",angle_sensor_voltage,current_angle,send_steer_angle);
		if(Left_wheel_angle >0)
				send_steer_angle |= 0x8000; //��λ��1
		else
				send_steer_angle &= 0x7fff; //��λ��0
		
		
		if(start_driverless_flag ==0)//�����˼�ʻ״̬����ʹ��ң����
		{
			strcpy(mode_name,"Record Point");
#if(YKMode==1)
			if(NRF24L01_RxPacket(wirelessBuf)==0 && (wirelessBuf[31]&0xff) ==1)//�յ���Ϣ  ĩλУ��
			{
				g_ykSafty_num = 0;//ÿ�յ�һ����Ϣ ����ֵ����
				carControlMsg = dataConvert(wirelessBuf);
				//printf("%d\t%d\t%d\t\n",carControlMsg.speed_l,carControlMsg.speed_r,carControlMsg.angle);
			
				TIM3->CCR1 = 1460+carControlMsg.speed_l*(1540-1460)/4096;//speed
				TIM3->CCR2 = 1000+carControlMsg.angle*(2000-1000)/4096;//angle
				
				
				//printf("speed_pwm = %d\r\n",TIM3->CCR1);
			}
#else
			;
#endif
		}
		else//���˼�ʻģʽ
		{

			strcpy(mode_name,"Driverless  ");
//����������һ��������ֻ��Ҫ����һ��
			if(switch_lastpoint_flag == 1)  //��һ�ν������ʱflag=1��
			{
				switch_lastpoint_flag =0; //reset
				
				if(target_point_seq == 0)
					gps_sphere_start =  target_point[actual_path_vertwx_num-1];
				else
					gps_sphere_start = target_point[target_point_seq-1];
				gps_sphere_end = target_point[target_point_seq];
				total_dis = point2point_dis(gps_sphere_end,gps_sphere_start);
				segment_num = total_dis/DIS_STEP;
				lat_step = (gps_sphere_end.lat - gps_sphere_start.lat)/segment_num ;
				lon_step = (gps_sphere_end.lon - gps_sphere_start.lon)/segment_num ;
			}
//����������һ��������ֻ��Ҫ����һ��			
			if(segment_seq>segment_num)
			{
				switch_lastpoint_flag = 1;
				segment_seq = 0;
				target_point_seq ++;
				if(target_point_seq >actual_path_vertwx_num-1)
					 target_point_seq =0;
				continue ;  //���continue  ��Ϊ��Ҫ���¼������´����������
			}
			
			
			gps_sphere_target.lon = gps_sphere_start.lon + lon_step*segment_seq;  //���㵱ǰĿ��㾭γ��
			gps_sphere_target.lat = gps_sphere_start.lat + lat_step*segment_seq;
			
			if(point2point_dis(gps_sphere_now,gps_sphere_target)<Dis_Threshold )
			{
				segment_seq++;//�л�Ŀ���
				continue;
			}
			
			
			relative_XY_dis_yaw(gps_sphere_now,gps_sphere_target,&rectangular);//���㵱ǰ�����������Ŀ���ľ���

			//lateral_err = LateralError(t_yaw_start,rectangular.yaw,rectangular.distance);
			
			yaw_err = rectangular.yaw - gps_sphere_now.yaw; //��ǰ��������������ƫ��
			
			sprintf(show_expect_yaw,"%3.1f",rectangular.yaw*180/3.1415926);
			LCD_ShowString(LCD_LU_X+11*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*14,5*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_expect_yaw);
			
			if(yaw_err >0.01 || yaw_err <-0.01)//��ֹ��������Լ��������
			{
				turning_radius = -0.5 *rectangular.distance /sin(yaw_err); //��������ģ�ͼ���ת��뾶��ǰ��ת�ǵĹ�ϵ
				//turning_radius *=100;//to cm 
				
//��ͬ����ģ���޸����´���				
				expect_angle = asin(AXIS_DIS/turning_radius) *180/pi;
				angle_differ = expect_angle - Left_wheel_angle;
				steer_control(angle_differ);
				
//	��ͬ����ģ���޸����ϴ���
			}
			
		}
		
//��ʾ				
		POINT_COLOR=RED;//��������Ϊ��ɫ 
		sprintf(show_angle,"%.1f",Left_wheel_angle);
		LCD_ShowString(LCD_LU_X+6*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*0,12*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,mode_name);	
		//printf("%s\r\n",mode_name);
		LCD_ShowString(LCD_LU_X+6*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*9,6*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_angle);
		sprintf(show_speed,"%.2f",car_speed);
		LCD_ShowString(LCD_LU_X+6*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*10,6*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_speed);
		sprintf(show_target_status,"%03d/%03d",target_point_seq+1,actual_path_vertwx_num);
		LCD_ShowString(LCD_LU_X+15*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*11,6*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_target_status);
		sprintf(show_segment_status,"%3d/%3d",segment_seq,segment_num);
		LCD_ShowString(LCD_LU_X+11*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*12,7*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_segment_status);
		sprintf(show_current_yaw,"%3.1f",gps_sphere_now.yaw*180/3.1415926);
		LCD_ShowString(LCD_LU_X+11*LCD_FOND_SIZE/2,LCD_LU_Y + LCD_FOND_SIZE*13,5*LCD_FOND_SIZE/2,LCD_FOND_SIZE,LCD_FOND_SIZE,show_current_yaw);
		
		//LCD_ShowString(LCD_LU_X,LCD_LU_Y + LCD_FOND_SIZE*0 ,6*LCD_FOND_SIZE/2 ,LCD_FOND_SIZE,LCD_FOND_SIZE,"mode :");
	
		delay_ms(50);
//	//	num = TIM2 ->CNT;
//		//printf("%d\t%d\r\n",timer_full ,num);
//		//timer_full =0;
//		//IWDG_ReloadCounter();
	}	 
 }
