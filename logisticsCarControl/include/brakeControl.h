#ifndef BRAKE_CONTROL_H_
#define BRAKE_CONTROL_H_

#define BRAKE_STATUS PFout(1)
#define BRAKE_ENABLE 1
#define BRAKE_DISABLE 0

void brakeControl_Init(void);
void brake_control(float set_brake_voltage);
void emergencyBrake(void);


#endif


