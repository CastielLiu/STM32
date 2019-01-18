#ifndef STEERING_MOTOR_H
#define STEERING_MOTOR_H
#include <stdint.h>
#include <stdio.h>
#include "usart.h"

#define GET_AD_MODE0 0
#define GET_AD_MODE1 1
#define GET_AD_MODE_DEBUG 2
#define GET_AD_MODE  GET_AD_MODE0


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

