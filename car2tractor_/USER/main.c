#include "main.h"

//KEY_UP  					   PA0 ��¼Ŀ���
//KEY0  						PE4 
//KEY1  						PE3 ��ʼ��ȡ�����˼�ʻ


//����ת�Ǵ�����    PA1   ADC1 CH1
//PWMת�����				PA7  	TIM3_CH2
//�������					PB2
//���ʹ��  				PF11


//TIM5 ���ڼ�ʱ  					TIM5

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

u8 timer_full=0;

extern u8 flag;
extern gps_data_t *gps ;
extern u8 actual_path_vertwx_num;

gps_sphere_t target_point[MAX_PATH_VERTEX_NUM]; 
gps_sphere_t  gps_sphere_start  ,gps_sphere_now, gps_sphere_target, gps_sphere_end;
u8 start_driverless_flag=0;

u8 g_ykSafty_num = 0;

char mode_name[20];

int main(void)
{	
	u8 wirelessBuf[32];	 //ң�������ݽ��ջ���	
	int num=0 ;
	int print_count =0;
	
 	u16 Adc_value = 0 ;
	float current_angle = 0;
	float  angle_sensor_voltage;
	
	float  mid_voltage= 1.6; //ת���е��ѹֵ �궨
	float  angle_increment = 360.0/3.3; //������ÿ�仯 1V �Ƕ�����
	
	extern u16  send_steer_angle;   //ԭʼ�Ƕ�����90��  15λΪ����0��1��
	
	float expect_angle =0.0 , angle_differ = 0.0;
	u8 num_use_to_for_cycle =0;
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
	}
	NRF24L01_RX_Mode();	
#endif
	
	POINT_COLOR=BLACK;//��������Ϊ��ɫ
	LCD_ShowString(30,20 ,48 ,16,16,"mode :");
	LCD_ShowString(30,46 ,176,16,16,"A    :             Lon");
	LCD_ShowString(30,62 ,176,16,16,"                   Lat");
	LCD_ShowString(30,78 ,176,16,16,"B    :             Lon");
	LCD_ShowString(30,94 ,176,16,16,"                   Lat");
	LCD_ShowString(30,110,176,16,16,"C    :             Lon");
	LCD_ShowString(30,126,176,16,16,"                   Lat");
	LCD_ShowString(30,142,176,16,16,"D    :             Lon");
	LCD_ShowString(30,158,176,16,16,"                   Lat");
	LCD_ShowString(30,190,48 ,16,16,"angle:");
	LCD_ShowString(30,206,48 ,16,16,"speed:");
	LCD_ShowString(30,222,15*8,16,16,"target status :");
	LCD_ShowString(30,238,15*8,16,16,"segment status:");
	LCD_ShowString(30,254,11*8,16,16,"curret yaw:");
	LCD_ShowString(30,254+16,11*8,16,16,"expect yaw:");
#ifdef DEBUG
	strcpy(mode_name,"debug");
#endif

	
  while(1)//25ms
	{
	//	TIM2->CNT = 0; //���ڼ�������ʱ
//	float  mid_voltage= 1.5; //ת���е��ѹֵ �궨
//	float  angle_increment = 360.0/5.0; //������ÿ�仯 1V �Ƕ�����		
		
		Adc_value = Get_Adc_Average(ADC_Channel_1,5);
		angle_sensor_voltage = Adc_value/4095 *3.3;
		current_angle = (angle_sensor_voltage - mid_voltage) * angle_increment;
		send_steer_angle = abs(current_angle) * 90; //����90��
		//printf("angle_sensor_voltage=%f\tcurrent_angle=%f\tsend_steer_angle=%d\r\n",angle_sensor_voltage,current_angle,send_steer_angle);
		if(current_angle >0)
				send_steer_angle |= 0x8000; //��λ��1
		else
				send_steer_angle &= 0x7fff; //��λ��0
		
		
		if(start_driverless_flag ==0)//�����˼�ʻ״̬����ʹ��ң����
		{
#if(YKMode==1)
			strcpy(mode_name,"Record Point");
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
#ifndef DEBUG
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
			LCD_ShowString(30+11*8,254+16,5*8,16,16,show_expect_yaw);
			
			if(yaw_err >0.01 || yaw_err <-0.01)//��ֹ��������Լ��������
			{
				turning_radius = -0.5 *rectangular.distance /sin(yaw_err); //��������ģ�ͼ���ת��뾶��ǰ��ת�ǵĹ�ϵ
				//turning_radius *=100;//to cm 
			

				
//��ͬ����ģ���޸����´���				
				expect_angle = asin(AXIS_DIS/turning_radius) *180/pi;
				angle_differ = expect_angle - current_angle;
				steer_control(angle_differ);
				
//	��ͬ����ģ���޸����ϴ���
			}
			
			
#else
				request_angle = 20.;
#endif
			
			print_count ++;
		}
		
//��ʾ				
		POINT_COLOR=RED;//��������Ϊ��ɫ 
		sprintf(show_angle,"%d",TIM3->CCR2);
		LCD_ShowString(78,20,96,16,16,mode_name);	
		//printf("%s\r\n",mode_name);
		LCD_ShowString(78,190,50,16,16,show_angle);
		sprintf(show_speed,"%.2f",car_speed);
		LCD_ShowString(78,206,50,16,16,show_speed);
		sprintf(show_target_status,"%03d/%03d",target_point_seq+1,actual_path_vertwx_num);
		LCD_ShowString(30+15*8,222,50,16,16,show_target_status);
		sprintf(show_segment_status,"%3d/%3d",segment_seq,segment_num);
		LCD_ShowString(30+15*8,238,56,16,16,show_segment_status);
		sprintf(show_current_yaw,"%3.1f",gps_sphere_now.yaw*180/3.1415926);
		LCD_ShowString(30+11*8,254,5*8,16,16,show_current_yaw);
		
		delay_ms(10);
	//	num = TIM2 ->CNT;
		//printf("%d\t%d\r\n",timer_full ,num);
		//timer_full =0;
		//IWDG_ReloadCounter();
	}	 
 }
