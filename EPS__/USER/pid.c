#include<stdio.h>
#include "pid.h"
#include<math.h>
#include<stdlib.h>

void PID_init(pid_t_ *pid)
{
	pid->setValue =0.0;
	pid->actualValue =0.0;
	pid->controlValue =0.0;
	pid->lastLastError =0.0;
	pid->lastError =0.0;
	pid->currentError =0.0;
	pid->SigmaError =0.0;
	pid->Kp = 0.03;
	pid->Ki = 0.00;
	pid->Kd = 0.0;
}

float PID1_realize(pid_t_ *pid,float setValue,float currentValue) //Î»ÖÃPID
{
	pid->actualValue = currentValue;
	pid->setValue = setValue;
	pid->currentError = pid->setValue - pid->actualValue;
	pid->SigmaError += pid->currentError;
	if(fabs(pid->currentError)<0.10 )
		pid->SigmaError =0.0;
	pid->controlValue =   pid->Kp*pid->currentError
						+ pid->Ki*pid->SigmaError
						+ pid->Kd*(pid->currentError - pid->lastError);
											
//	printf("\ncurrentError=%f  lastError=%f  control_value=%f p=%.3f  i=%.3f  d=%.3f\n",
//						pid->currentError,pid->lastError,pid->controlValue,pid->Kp,pid->Ki,pid->Kd);
						
	pid->lastError = pid->currentError;
	pid->lastLastError = pid->lastError;
	
	return pid->controlValue;
}

float PID2_realize(pid_t_ *pid,float setValue,float currentValue)  //ÔöÁ¿PID
{
	pid->actualValue = currentValue;
	pid->setValue = setValue;
	pid->currentError = pid->setValue - pid->actualValue;
	pid->controlValue +=  pid->Kp *(pid->currentError -pid->lastError)
											+ pid->Ki * pid->currentError
											+ pid->Kd * (pid->currentError - 2*pid->lastError +pid->lastLastError);
	pid->lastError = pid->currentError;
	pid->lastLastError = pid->lastError;
	
	return pid->controlValue;
}
