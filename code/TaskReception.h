#ifndef _TaskReception_
#define _TaskReception_


#include "main.h"

/*
 * Will use the hardware RNG to generate random number to use as task ID
 * The output number range is 0 -> TaskFunction_NUM
 * Input: param is a pointer to a QueueHandle_t, the queue to add request to
 */
void TaskReception(void *param);

#endif
