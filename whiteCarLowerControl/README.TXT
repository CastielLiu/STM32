 
//PWM速度控制        PA7  		TIM3_CH2
 
//GPS数据         USART3
 
//预装载值寄存器     TIMx->ARR  
//比较寄存器         TIMx->CCR2 
//CAN				StdId =0x12   ExtId=0x12;
//TIM5 用于计时
//TIM2 用于编码器计数

KEY1     进入或退出无人驾驶状态
 
正交编码器测速  motor_speed
 

遥控器信号丢失后将无法请求数据，因此可在遥控连接超时之后急停，而不用担心一直有伪数据。

USART2接收上位机数据

header  0x55 0xAA 
msgID	
msgLen
msg
checkNum

