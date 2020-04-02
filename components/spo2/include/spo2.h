#ifndef __SPO2__
#define __SPO2__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "spo2_driver.h"


/**
 * spo2_runner() - spo2 task worker function
 * @pvParameters:	void* to pass function parameters.
 *
 * This worker function retrieves an input data sample from the ADCs. This
 * is the place where further processing of the data will happen.
 */
void spo2_runner(void *pvParameters);


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


#endif 
