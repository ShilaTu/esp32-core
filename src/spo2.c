#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "spo2.h"
#include "spo2_driver.h"


static const char* TAG = "SpO2";


/**
 * spo2_runner() - spo2 task worker function
 * @pvParameters:	void* to pass function parameters.
 *
 * This worker function retrieves an input data sample from the ADCs. This
 * is the place where further processing of the data will happen.
 */
static void spo2_runner(void *pvParameters);

/**
 * spo2_read() - read a single input data sample
 * @sample:	space for sample readings
 *
 * Read out a single data sample using the different peripherals needed.
 */
static void spo2_read(spo2_adc_sample *sample);


void
spo2_init
(const char* name, _spo2_task *spo2_task)
{
	spo2_init_peripherals();

	spo2_task->handle = xTaskCreateStatic(
		spo2_runner,
		name,
		SPO2_STACK_SIZE,
		NULL,
		tskIDLE_PRIORITY,
		spo2_task->stack,
		&spo2_task->tcb
	);
}


static
void
spo2_runner
(void *pvParameters)
{
	spo2_adc_sample sample;

	for(;;)
	{
		spo2_read(&sample);
		ESP_LOGD(
			TAG,
			"RED-DC=%-4d IRD-DC=%-4d RED-AC=%-4d IRD-AC=%-4d",
			sample.red_dc,
			sample.red_ac,
			sample.ird_dc,
			sample.ird_ac
		);
		vTaskDelay(SPO2_READ_PERIOD);
	}
}

static
void
spo2_read(spo2_adc_sample *sample)
{
	spo2_read_adc(sample);
}
