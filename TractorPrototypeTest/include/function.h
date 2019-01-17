#ifndef FUNCTION_H_
#define FUNCTION_H_
#include <stdint.h>
#include <sys.h>
#include <string.h>
#include "global_params.h"
#include "can.h"
#include "steering_motor.h"

#define CLEAR_CONFIG_DATA 0
#define STEER_VERIFY 1
#define ANGLE_SENSOR_VERIFY 2
#define RECORD_TARGET_POINT 3
#define START_DRIVERLESS 4
#define PAUSE_DRIVERLESS 5
#define STOP_DRIVERLESS 6
#define STEERING_DEBUG 7

#define DRIVERLESS_STATE 1
#define MANUAL_DRIVE_STATE 0
#define PAUSE_DRIVERLESS_STATE 2

void clearBKP(void);


void recordTargetPoint();
void pauseDriverless();
void startDriverless();
void steeringVerify();
void angleSensorVerify( const CanRxMsg *  RxMessage);
void steeringDebug(const CanRxMsg * RxMessage);

#endif