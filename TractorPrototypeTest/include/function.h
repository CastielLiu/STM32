#ifndef FUNCTION_H_
#define FUNCTION_H_
#include <stdint.h>
#include <sys.h>
#include <string.h>
#include "global_params.h"
#include "can.h"

#define CLEAR_CONFIG_DATA 0
#define STEER_VERIFY 1
#define ANGLE_SENSOR_VERIFY 2
#define RECORD_TARGET_POINT 3
#define START_DRIVERLESS 4
#define PAUSE_DRIVERLESS 5
#define STOP_DRIVERLESS 6

void clearBKP(void);
void GetComma(const char *str,u8 *commaLocation_);
u8 gpsParse(const char *buf);
void recordTargetPoint();
void pauseDriverless();
void startDriverless();

#endif