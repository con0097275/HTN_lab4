#ifndef _MAIN_H_
#define _MAIN_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#define TaskFunction_NUM (5)
#define ADDED_GARBAGE_REQUEST (5)
#define QUEUE_SIZE (TaskFunction_NUM)

// queue
extern QueueHandle_t queue;
// mutex to avoid race condition when accessing queue and ProcessReady
// the esp32 have two core after all
extern SemaphoreHandle_t mt;
// global uint8_t array that serve as bool, each task will set the corresponding element to signify
// that the signal has been read by the task and determined not to process it
extern uint8_t ProcessReady[TaskFunction_NUM];
// task handle to selectively resume functional task
extern TaskHandle_t FunctionalTaskHandle[TaskFunction_NUM];

#endif

