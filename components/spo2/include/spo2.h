#ifndef __SPO2__
#define __SPO2__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "spo2_driver.h"
#include "macro/task.h"
#include "macro/queue.h"


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
} spo2_input_sample_t;


/**
 * spo2_t - type deffinition of the spo2 unit
 *
 * @adc_queue:	Receives ulp adc measurements.
 * @adc_task:	Processes adc input data from adc_queue.
 */
typedef struct {
	LIFESENSOR_QUEUE(spo2_adc_sample_t, 16) adc_queue;
	LIFESENSOR_TASK(4096) adc_task;
} spo2_t;


/**
 * spo2_init() - spo2 initialization
 * @spo2:	The only parameter necessary is the unit itself.
 *
 * The function initializes queues and tasks of the spo2 unit.
 */
void lifesensor_spo2_init(spo2_t *spo2, const char *name);

#endif 
