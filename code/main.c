#include "TaskFunction.h"
#include "main.h"
#include "TaskReception.h"
#include "request.h"
#include <stdio.h>

QueueHandle_t queue;
SemaphoreHandle_t mt;
uint8_t ProcessReady[TaskFunction_NUM] = { 0 };
TaskHandle_t FunctionalTaskHandle[TaskFunction_NUM];

void app_main(void)
{
    queue = xQueueCreate(sizeof(Request_t), QUEUE_SIZE);
    mt = xSemaphoreCreateMutex();

    xTaskCreate(TaskReception, "Reception task", 2048, NULL,
            1, NULL);

    for (size_t i = 0; i < TaskFunction_NUM; i++)
    {
        xTaskCreate(TaskFunction, "Functional task", 2048, (void*) i,
                1, &FunctionalTaskHandle[i]);
    }

    xTaskCreate(TaskForCleanGarbageRequest, "Cleanup task", 2048, NULL,
            1, NULL);

    vTaskDelete(NULL);
}
