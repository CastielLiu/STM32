#ifndef FUNCTION_H_
#define FUNCTION_H_
#include <stdint.h>
#include <sys.h>
#include <string.h>
#include "global_params.h"
#include "can.h"
#include "steering_motor.h"



//上位机请求命令ID
#define REQ_CLEAR_CONFIG_DATA 0
#define REQ_STEER_VERIFY 1
#define REQ_ANGLE_SENSOR_VERIFY 2
#define REQ_RECORD_TARGET_POINT 3
#define REQ_START_DRIVERLESS 4
#define REQ_PAUSE_DRIVERLESS 5
#define REQ_STOP_DRIVERLESS 6
#define REQ_STEERING_DEBUG 7
#define REQ_STEERING_ENABLE 8
#define REQ_STEERING_DISABLE 9


//应答上位机指令ID
#define ANS_STEER_VERIFY 1
#define ANS_CLEAR_CONFIG_DATA 0
#define ANS_ANGLE_SENSOR_VERIFY 2
#define ANS_RECORD_TARGET_POINT 3
#define ANS_DRIVE_STATUS 4

#define ANS_STEER_DEBUG 7 


//应答上位机指令参数
#define DRIVERLESS_STATE 1
#define MANUAL_DRIVE_STATE 0
#define PAUSE_DRIVERLESS_STATE 2

#define STEER_DEBUG_YES 1
#define STEER_DEBUG_NO 0



void clearBKP(void);


void recordTargetPoint(void);
void pauseDriverless(void);
void startDriverless(void);
void steeringVerify(void);
void angleSensorVerify( const CanRxMsg *  RxMessage);
void steeringDebug(const CanRxMsg * RxMessage);

#endif