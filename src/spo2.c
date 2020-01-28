#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "ulp.h"
#include "spo2.h"
#include "spo2_driver.h"
#include "spo2_filter.h"


/**
 * spo2_runner() - spo2 task worker function
 * @pvParameters:	void* to pass function parameters.
 *
 * This worker function retrieves an input data sample from the ADCs. This
 * is the place where further processing of the data will happen.
 */
static void spo2_runner(void *pvParameters);


void
spo2_init
(_spo2_task *spo2_task, _spo2_queue *spo2_queue)
{
	spo2_init_peripherals();

	spo2_queue->handle = xQueueCreateStatic(
		spo2_queue->length,
		spo2_queue->item_size,
		spo2_queue->buffer,
		&spo2_queue->queue
	);

	spo2_task->handle = xTaskCreateStatic(
		spo2_runner,
		spo2_task->name,
		SPO2_TASK_STACK_SIZE,
		(void*)spo2_queue,
		spo2_task->priority,
		spo2_task->stack,
		&spo2_task->tcb
	);
}


static
void
spo2_runner
(void *pvParameters)
{
	static _spo2_input_sample sample = {0};
	static _spo2_adc_sample adc_sample;
	_spo2_queue *spo2_queue = (_spo2_queue*)pvParameters;

	for(;;)
	{
		xQueueReceive(&spo2_queue->queue, &adc_sample, portMAX_DELAY);
		spo2_filter(&sample, &adc_sample);

		ESP_LOGD(
			SPO2_TASK_NAME,
			"RED-DC=%-4d IRD-DC=%-4d RED-AC=%-4d IRD-AC=%-4d RED-ACDC=%-4d IRD-ACDC=%-4d",
			sample.red_dc,
			sample.ird_dc,
			sample.red_ac,
			sample.ird_ac,
			sample.red_acdc,
			sample.ird_acdc
		);
	}
}

