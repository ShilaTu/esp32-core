#include "ulp.h"
#include "spo2.h"
#include "wifi.h"
#include "transmission.h"
#include "log_wifi.h"


static uint8_t spo2_queue_buffer[SPO2_QUEUE_BUFFER_SIZE];
static StackType_t spo2_task_stack[SPO2_TASK_STACK_SIZE];

_spo2_queue spo2_queue = {
	.length = SPO2_QUEUE_LENGTH,
	.item_size = SPO2_QUEUE_ITEM_SIZE,
	.buffer = spo2_queue_buffer
};

_spo2_task spo2_task = {
	.name = SPO2_TASK_NAME,
	.priority = tskIDLE_PRIORITY,
	.stack = spo2_task_stack
};

static uint8_t log_wifi_queue_buffer[LOG_WIFI_QUEUE_BUFFER_SIZE];
static StackType_t log_wifi_task_stack[LOG_WIFI_TASK_STACK_SIZE];

_log_wifi_queue log_wifi_queu = {
	.length = LOG_WIFI_QUEUE_SIZE,
	.item_size = LOG_WIFI_QUEUE_ITEM_SIZE,
	.buffer = log_wifi_queue_buffer
};

_log_wifi_task log_wifi_task = {
	.name = LOG_WIFI_TASK_NAME,
	.priority = tskIDLE_PRIORITY,
	.stack = log_wifi_task_stack
};


/**
 * app_main() - main thread after boot process
 *
 * The prototype should be part of the esp32 idf framework. This is the entry
 * point of the life-sensor application which is used to setup necessary memory
 * objects and start necessary threads.
 */
void app_main()
{
	spo2_init(&spo2_task, &spo2_queue);
	ulp_init(&spo2_queue);
	log_wifi_init(&log_wifi_task, &log_wifi_queu);
}

