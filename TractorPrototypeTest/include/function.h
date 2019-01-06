#ifndef FUNCTION_H_
#define FUNCTION_H_
#include <stdint.h>
#include <sys.h>
#include <string.h>
#include "global_params.h"

void clearBKP(void);
void GetComma(const char *str,u8 *commaLocation_);
u8 gpsParse(const char *buf);

#endif