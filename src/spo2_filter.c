#include "spo2_filter.h"


void spo2_filter(_spo2_input_sample *sample, _spo2_adc_sample *raw_data)
{
	/*
	 * 5Hz low-pass filtering and removing the 2.5 V offset 
	 * from the amplified AC input values
	 */
	sample->ird_ac = AC_LOWPASS_NO_OFFSET(sample->ird_ac, (uint16_t)raw_data->ird_ac);
	sample->red_ac = AC_LOWPASS_NO_OFFSET(sample->red_ac, (uint16_t)raw_data->red_ac);

	/*
	 * 0.5 Hz low-pass filtering the input adc DC values
	 */
	sample->ird_dc = DC_LOWPASS(sample->ird_dc, (uint16_t)raw_data->ird_dc);
	sample->red_dc = DC_LOWPASS(sample->red_dc, (uint16_t)raw_data->red_dc);

	/*
	 * 0.5Hz high-pass filtered AC values on top of 
	 * the DC values without amplification
	 */
	sample->ird_acdc = ((uint16_t)raw_data->ird_dc) - sample->ird_dc;
	sample->red_acdc = ((uint16_t)raw_data->red_dc) - sample->red_dc;
}
