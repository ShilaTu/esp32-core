#ifndef __SPO2_FILTER__
#define __SPO2_FILTER__

#include "spo2.h"
#include "spo2_driver.h"

#define ADC_OFFSET_HALF 2048

#define DC_LOWPASS(fb,adc) ((((126*fb)>>7)+((2*(adc))>>7)))
#define AC_LOWPASS_NO_OFFSET(fb,adc) ((((115*fb)>>7)+((13*(((adc)-ADC_OFFSET_HALF)))>>7)))

/**
 * spo2_filter() - Applies digital filters to adc sample
 * @sample:		source and(!) target sample that contains (previous) filtered values
 * @adc_sample:	source sample that cntains raw adc readings
 *
 * This function applies digital filters to the values from `adc_sample` while
 * additionally using the old values from the previous filtered `sample`.
 */
void spo2_filter(_spo2_input_sample *sample, _spo2_adc_sample *adc_sample);

#endif

