#ifndef __SPO2_FILTER__
#define __SPO2_FILTER__

#include "spo2.h"
#include "spo2_driver.h"

#define ADC_OFFSET_HALF 2048


/**
 * spo2_filter() - Applies digital filters to adc sample
 * @sample:     source and(!) target sample that contains (previous) filtered
 *              values
 * @adc_sample: source sample that cntains raw adc readings
 *
 * This function applies digital filters to the values from `adc_sample` while
 * additionally using the old values from the previous filtered `sample`.
 */
void spo2_filter(spo2_input_sample_t *sample, spo2_adc_sample_t *adc_sample);


#endif

