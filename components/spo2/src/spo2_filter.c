#include "spo2_filter.h"


/**
 * spo2_ac_lowpass_no_offset() - 5 Hz digital low-pass filter (without offset)
 * @feedback:	previous filter output
 * @adc_value:	current filter input
 *
 * This sigital filter gives a bound at 5 Hz filter. Additionally it removes an
 * offset of 50% of the input voltage. It is used pre-process the red and ird
 * sensor values.
 */
static int32_t spo2_ac_lowpass_no_offset(int32_t feedback, int32_t adc_value);

/**
 * spo2_dc_lowpass() - 0.5 Hz digital low-pass filter
 * @feedback:	previous filter output
 * @adc_value:	current filter input
 *
 * This sigital filter gives a bound at 0.5 Hz filter. It is used pre-process
 * the red and ird sensor values.
 */
static int32_t spo2_dc_lowpass(int32_t feedback, int32_t adc_value);


void
spo2_filter
(spo2_input_sample_t *sample, spo2_adc_sample_t *raw_data)
{
	/*
	 * 5Hz low-pass filtering and removing the 2.5 V offset 
	 * from the amplified AC input values
	 */
	sample->ird_ac = spo2_ac_lowpass_no_offset(sample->ird_ac,
						   (uint16_t)raw_data->ird_ac);
	sample->red_ac = spo2_ac_lowpass_no_offset(sample->red_ac,
			                           (uint16_t)raw_data->red_ac);

	/*
	 * 0.5 Hz low-pass filtering the input adc DC values
	 */
	sample->ird_dc = spo2_dc_lowpass(sample->ird_dc,
					 (uint16_t)raw_data->ird_dc);
	sample->red_dc = spo2_dc_lowpass(sample->red_dc,
					 (uint16_t)raw_data->red_dc);

	/*
	 * 0.5Hz high-pass filtered AC values on top of 
	 * the DC values without amplification
	 */
	sample->ird_acdc = ((uint16_t)raw_data->ird_dc) - sample->ird_dc;
	sample->red_acdc = ((uint16_t)raw_data->red_dc) - sample->red_dc;
}


static
int32_t
spo2_dc_lowpass
(int32_t feedback, int32_t adc_value)
{
	return (126 * feedback >> 7) + (2 * adc_value >> 7);
}

static
int32_t
spo2_ac_lowpass_no_offset
(int32_t feedback, int32_t adc_value)
{
	adc_value = adc_value - ADC_OFFSET_HALF;
	return (115 * feedback >> 7) + (13 * adc_value >> 7);
}

