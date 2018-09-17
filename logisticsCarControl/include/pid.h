#ifndef PID_H_
#define PID_H_

typedef struct 
{
	float setValue; 
	float actualValue;
	float controlValue;
	float lastLastError;      
	float lastError;        
	float currentError;  
	float SigmaError; 
	float Kp,Ki,Kd;    
} pid_t_;


void PID_init(pid_t_ *pid);
float PID2_realize(pid_t_ *pid,float setValue,float currentValue);
float PID1_realize(pid_t_ *pid,float setValue,float currentValue);



#endif
