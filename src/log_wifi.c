#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "wifi.h"
#include "transmission.h"

#include "log_wifi.h"

static void log_wifi_runner(void *pvParameters);

void
log_wifi_init
(_log_wifi_task *log_task, _log_wifi_queue *log_queue)
{
    wifi_init();
	wifi_wait_connected();
    transmission_init();

    log_queue->handle = xQueueCreateStatic(
        log_queue->length,
        log_queue->item_size,
        log_queue->buffer,
        &log_queue->queue
    );

    log_task->handle = xTaskCreateStatic(
        log_wifi_runner,
        log_task->name,
        LOG_WIFI_TASK_STACK_SIZE,
        (void*)log_queue,
        log_task->priority,
        log_task->stack,
        &log_task->tcb
    );
}

void
log_wifi_runner
(void *pvParameters)
{
    _log_wifi_queue *log_queue = (_log_wifi_queue *)pvParameters;
    static _log_wifi_data data = {0};
    static char line_buffer[LOG_WIFI_LINE_MAX_LENGTH];

    for(;;)
    {
        xQueueReceive(&log_queue->queue, &data, portMAX_DELAY);
        int len = snprintf(
            line_buffer, 
            LOG_WIFI_LINE_MAX_LENGTH, 
            data.format, 
            data.tags, 
            data.key_name, 
            data.value
        );
        if(len > 0) {
            transmission_send(line_buffer, len);
        }
    }
}