#ifndef _TaskFunction_
#define _TaskFunction_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "main.h"
#include <stdint.h>

void TaskFunction(void *param);
void TaskForCleanGarbageRequest(void *param);

#endif
