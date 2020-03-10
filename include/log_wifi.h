#ifndef __LOG__
#define __LOG__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#define LOG_WIFI_TASK_NAME "Log_Wifi"
#define LOG_WIFI_QUEUE_SIZE 1024
#define LOG_WIFI_LINE_MAX_LENGTH 256
#define LOG_WIFI_TASK_STACK_SIZE 0x1000
#define LOG_WIFI_QUEUE_ITEM_SIZE (sizeof(_log_wifi_data))
#define LOG_WIFI_QUEUE_BUFFER_SIZE (LOG_WIFI_QUEUE_ITEM_SIZE * LOG_WIFI_QUEUE_SIZE)

/**
 * struct _log_wifi_data - single value to log
 * @key_name:       name of the field to log
 * @value:          value to log
 * @tags:           aditional tags to add to log
 * 
 * This struct holds a single line to log. 
 * Optional tags can be added.
 */
typedef struct {
    const char *format;
    const char *key_name;
    float value;
    const char *tags;
} _log_wifi_data;

/**
 * struct _log_wifi_queue - log queue struct
 * @length:         number of items teh queue can hold
 * @itemsize:       size of a single item in the queue
 * @buffer:         buffer to store queue data
 * @queue:          pointer to the queues data structure
 * @handle:         a handle to reference the queue
 * 
 * This struct serves as a shortcut for operating with FreeRTOS 
 * queues, such that it holds management data for the queue
 */
typedef struct {
    UBaseType_t length;
    UBaseType_t item_size;
    uint8_t *buffer;
    StaticQueue_t queue;
    QueueHandle_t handle;
} _log_wifi_queue;

/**
 * struct _log_wifi_task - log task struct
 * @tcb:	task related data
 * @handle:	a handle to reference the task
 * @stack:	the address of the tasks' stack
 *
 * This struct serves as a shortcut for operating with FreeRTOS tasks such
 * that it includes necessary fields for managing threads.
 */
typedef struct {
    const char *name;
    StaticTask_t tcb;
    TaskHandle_t handle;
    StackType_t *stack;
    UBaseType_t priority;
} _log_wifi_task;


/**
 * log_wifi_init() - initializes wifi logging task
 * @log_wifi_task:      task configuration struct
 * @log_wifi_queue:     queue configuration struct
 * 
 * log_wifi_init() initializes wifi periferals and a task that can
 * transmit log data items from it's input queue via wifi/udp. Log
 * data items contain a format string and a value and optional tasks.
 * The log_wifi_task formats data to be received by a time series 
 * database like influxdb.
 */
void log_wifi_init(
    _log_wifi_task *log_wifi_task, 
    _log_wifi_queue *log_wifi_queue
);

#endif