#ifndef DIRECTION_H_
#define DIRECTION_H_
#endif

#define STEER_DIR PBout(2)
#define STEER_ENABLE PFout(11)

void car_control_init(void);
void steer_control(float  angle_differ);
