#ifndef SPEED_CONTROL_H_
#define SPEED_CONTROL_H_

#define SPEED_DIRICTION PFout(0)
#define FRONT_DIR 0
#define BACK_DIR  1

void speedControl_Init(void);
void speed_control(float set_speed);

#endif