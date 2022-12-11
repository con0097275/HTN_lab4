#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "TaskFunction.h"
#include "main.h"
#include "request.h"
#include <inttypes.h>
#include <stdio.h>


static inline uint8_t allProcess_ready()
{
    for (uint32_t i = 0; i < TaskFunction_NUM; i++)
    {
        if (!ProcessReady[i])
            return 0;
    }

    return 1;
}

static inline void ClearAllReadyProcess()
{
    for (uint32_t i = 0; i < TaskFunction_NUM; i++)
    {
        ProcessReady[i] = 0;
    }
}

static inline void WakeUpAllFunctional()
{
    for (uint32_t i = 0; i < TaskFunction_NUM; i++)
    {
        if (eTaskGetState(FunctionalTaskHandle[i]) == eSuspended)
        {
            vTaskResume(FunctionalTaskHandle[i]);
        }
    }
}

// check debug
static inline void debug_print_ProcessReady()
{
    for (uint32_t i = 0; i < TaskFunction_NUM; i++)
    {
        printf("%" PRIu8 " ", ProcessReady[i]);
    }
    printf("\n");
}

void TaskFunction(void *param)
{
    size_t id = (size_t) param;

    while (1)
    {
        Request_t new_req;
        if (xQueuePeek(queue, &new_req, portMAX_DELAY) == pdPASS)
        {
            if (new_req.idTask == id)
            {
                // remove the request from queue
                xQueueReceive(queue, &new_req, portMAX_DELAY); 

                xSemaphoreTake(mt, portMAX_DELAY);
                ClearAllReadyProcess();
                xSemaphoreGive(mt);

                printf("Task with ID %zu checked the queue and run\n", id);
                // wake up all other suspended task
                WakeUpAllFunctional();
            }
            else
            {
                // not match, set ProcessReady to 1, then go into suspend to prevent
                // checking the request again
                xSemaphoreTake(mt, portMAX_DELAY);
                ProcessReady[id] = 1;
                xSemaphoreGive(mt);

                vTaskSuspend(NULL);
                continue;
            }
        }
    }

    vTaskDelete(NULL);
}


void TaskForCleanGarbageRequest(void *param)
{
    while (1)
    {
        // check if all task has process the request
        xSemaphoreTake(mt, portMAX_DELAY);
        uint8_t all_checked = allProcess_ready();
        xSemaphoreGive(mt);

        if (all_checked)
        {
            Request_t garbage_reg;

            // remove the request from queue
            xQueueReceive(queue, &garbage_reg, portMAX_DELAY); // should return success

            xSemaphoreTake(mt, portMAX_DELAY);
            ClearAllReadyProcess();
            xSemaphoreGive(mt);

            printf("Error: request not performed by any task\n");
            printf("Clean up garbage request %zu\n", garbage_reg.idTask);
            // then wake up all other suspended task
            WakeUpAllFunctional();
        }
    }

    vTaskDelete(NULL);
}
