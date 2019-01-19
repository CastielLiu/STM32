#ifndef STEERING_MOTOR_H
#define STEERING_MOTOR_H
#include <stdint.h>
#include <stdio.h>
#include "usart.h"
#include "can.h"

#define GET_AD_MODE0 0  //1.19测试使能电机是否收到应答。。。
#define GET_AD_MODE1 1  //错误率高 ，1.19测试为何出错，错误时数据情况
#define GET_AD_MODE1_OutTimeVesion 11  //添加超时  需调试超时时间


#define GET_AD_MODE3 3   //1.18new  利用空闲检测数据接收完成，可吸收非AD字段

#define GET_AD_MODE_DEBUG 5 //输出定值  

#define GET_AD_MODE  GET_AD_MODE3


//2019.1.19 9:13  GET_AD_MODE3与GET_AD_MODE1_OutTimeVesion实现功能

void steeringEnable(void);
void steeringDisable();
uint16_t getAdcValue(void);
float getCurrentRoadWheelAngle(void);
void setSteeringRotate(float cycleNum);
void setSteeringSpeed(uint8_t speed);
uint16_t generateModBusCRC_byTable(const uint8_t *ptr,uint8_t size);
void sendControlCmd(const uint8_t * dataPtr,int size);
uint16_t getErrorMsg(void);
void steer_control(float angle_diff);


#endif

