#ifndef STEERING_MOTOR_H
#define STEERING_MOTOR_H
#include <stdint.h>
#include <stdio.h>
#include "usart.h"

void steeringEnable(void);
uint16_t getAdcValue(void);
float getCurrentRoadWheelAngle(void);
void setSteeringRotate(float cycleNum);
void setSteeringSpeed(uint8_t speed);
uint16_t generateModBusCRC_byTable(const uint8_t *ptr,uint8_t size);
void sendControlCmd(const uint8_t * dataPtr,int size);
uint16_t getErrorMsg(void);
void steer_control(float angle_diff);


#endif

