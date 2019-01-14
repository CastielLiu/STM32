#ifndef BEIDOU_GPS_H_
#define BEIDOU_GPS_H_
#include <sys.h>
#include <string.h>
#include "global_params.h"

#define MaxCommaNumInGpsData  12

void GetComma(const u8 *str,u8 *commaLocation_);
u8 gpsParseBeiDou(const u8 *buf);















#endif