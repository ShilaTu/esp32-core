#ifndef __SPO2__
#define __SPO2__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "spo2_driver.h"

#define SPO2_TASK_NAME "SpO2"
#define SPO2_TASK_STACK_SIZE 0x1000
#define SPO2_QUEUE_LENGTH 16
#define SPO2_QUEUE_ITEM_SIZE sizeof(_spo2_adc_sample)
#define SPO2_QUEUE_BUFFER_SIZE (SPO2_QUEUE_ITEM_SIZE * SPO2_QUEUE_LENGTH)


/**
 * struct _spo2_sample - life-sensor sample struct
 * @ird_dc:		infrared DC value
 * @red_dc:		red DC value
 * @ird_ac:		infrared AC value
 * @red_ac:		red AC value
 * @ird_acdc:	infrared error of DC and pre-processed DC
 * @red_acdc:	red error of DC and pre-processed DC
 *
 * This struct holds pre-processed sensor values
 */
typedef struct {
	int32_t ird_dc;
	int32_t red_dc;
	int32_t ird_ac;
	int32_t red_ac;
	int32_t ird_acdc;
	int32_t red_acdc;
} _spo2_input_sample;


/**
 * struct _spo2_task - life-sensor task struct
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
} _spo2_task;

/**
 * struct _spo2_queue - life-sensor queue struct
 * @length:	task related data
 * @item_size:	size of a single item
 * @buffer:	buffer to store the queues data
 * @queue:	pointer to the queues data structure
 * @handle:	a handle to reference the queue
 *
 * This struct serves as a shortcut for operating with FreeRTOS queues such
 * that it includes necessary fields for managing queues.
 */
typedef struct {
	UBaseType_t length;
	UBaseType_t item_size;
	uint8_t *buffer;
	StaticQueue_t queue;
	QueueHandle_t handle;
} _spo2_queue;


/**
 * spo2_init() - SpO2 (Pulsoxy) meassurement device interface initialization
 * @spo2_task:	task configuration
 * @spo2_queue:	queue configuration
 *
 * spo2_init() initializes peripherals (4xADC, 1xPWM, 2xGPIO) that is necessary
 * to operate together with the pulsoxy board which does preprocessing of input
 * data. After initialization a thread is started which periodically reads in
 * input data.
 */
void spo2_init(_spo2_task *spo2_task, _spo2_queue *spo2_queue);

#endif 
