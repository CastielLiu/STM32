#ifndef STEERING_MOTOR_H
#define STEERING_MOTOR_H
#include <stdint.h>
#include <stdio.h>
#include "usart.h"
#include "can.h"

#define GET_AD_MODE0 0  //1.19����ʹ�ܵ���Ƿ��յ�Ӧ�𡣡���
#define GET_AD_MODE1 1  //�����ʸ� ��1.19����Ϊ�γ�������ʱ�������
#define GET_AD_MODE1_OutTimeVesion 11  //��ӳ�ʱ  ����Գ�ʱʱ��


#define GET_AD_MODE3 3   //1.18new  ���ÿ��м�����ݽ�����ɣ������շ�AD�ֶ�

#define GET_AD_MODE_DEBUG 5 //�����ֵ  

#define GET_AD_MODE  GET_AD_MODE3


//2019.1.19 9:13  GET_AD_MODE3��GET_AD_MODE1_OutTimeVesionʵ�ֹ���

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

