机械楼102拖拉机底层控制程序



// 车轮转角传感器    PA1
//PWM速度控制        PA7  		TIM3_CH2
// 电机方向          PB2
//GPS数据         USART3
//电机使能           PF11
//预装载值寄存器     TIMx->ARR  
//比较寄存器         TIMx->CCR2 
//CAN				StdId =0x12   ExtId=0x12;
//TIM5 用于计时
//TIM2 用于编码器计数

KEY1     进入或退出无人驾驶状态
KEY_UP 记录目标点。
正交编码器测速  motor_speed
非无人驾驶模式下，即记录模式下可遥控控制。
