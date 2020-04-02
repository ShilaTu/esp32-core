#include "ulp.h"
#include "spo2.h"
#include "spo2_driver.h"
#include "macro_task.h"
#include "macro_queue.h"

/**
 * spo2_t - type deffinition of the spo2 unit
 *
 * @adc_queue:	Receives ulp adc measurements.
 * @adc_tasks:	Processes adc input data from adc_queue.
 */
typedef struct {
	LIFESENSOR_QUEUE(spo2_adc_sample_t, 16) adc_queue;
	LIFESENSOR_TASK(4096) adc_task;
} spo2_t;


/*
 * Unit definitions
 */
spo2_t spo2;


/**
 * app_main() - main thread after boot process
 *
 * The prototype should be part of the esp32 idf framework. This is the entry
 * point of the life-sensor application which is used to setup necessary memory
 * objects and start necessary threads.
 */
void app_main()
{
	lifesensor_queue_init(&spo2.adc_queue);
	lifesensor_task_init(
		&spo2.adc_task,
		"spo2_adc",
		spo2_runner,
		&spo2.adc_queue.queue,
		tskIDLE_PRIORITY
	);

	ulp_init(&spo2.adc_queue.queue);
}
