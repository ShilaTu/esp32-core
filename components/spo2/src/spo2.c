#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "ulp.h"
#include "spo2.h"
#include "spo2_driver.h"
#include "spo2_filter.h"
#include "macro/queue.h"


/**
 * spo2_runner() - spo2 task worker function
 * @pvParameters:	void* to pass function parameters.
 *
 * This worker function retrieves an input data sample from the ADCs. This
 * is the place where further processing of the data will happen.
 */
static void spo2_runner(void *pvParameters);


void
lifesensor_spo2_init
(spo2_t *spo2, const char *name)
{
	lifesensor_queue_init(&spo2->adc_queue);
	lifesensor_task_init(
		&spo2->adc_task,
		name,
		spo2_runner,
		&spo2->adc_queue.queue,
		tskIDLE_PRIORITY
	);
}


static
void
spo2_runner
(void *args)
{
	static spo2_input_sample_t sample = {0};
	static spo2_adc_sample_t adc_sample;
	lifesensor_queue_t *spo2_queue = (lifesensor_queue_t*)args;

	for(;;)
	{
		xQueueReceive(&spo2_queue->queue, &adc_sample, portMAX_DELAY);
		spo2_filter(&sample, &adc_sample);

		ESP_LOGD(
			"SpO2",
			"RED-DC=%-4d IRD-DC=%-4d RED-AC=%-4d IRD-AC=%-4d"
			" RED-ACDC=%-4d IRD-ACDC=%-4d",
			sample.red_dc,
			sample.ird_dc,
			sample.red_ac,
			sample.ird_ac,
			sample.red_acdc,
			sample.ird_acdc
		);
	}
}
