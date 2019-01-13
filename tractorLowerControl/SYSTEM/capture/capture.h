#ifndef CAPTURE_H_
#define CAPTURE_H_

#include "sys.h"

#define QuadratureEncoder 1

typedef struct
{
	s16 pulseNumPer50ms;
	int totalPulseNum;
	float carSpeed;
	float odometer;
	float motorSpeed;
	u16 encoderLineNum;
	
	
}speedMsg_t;


void TIM4_Capture_Init(uint16_t arr,uint16_t psc);



#endif 
