#ifndef __ULP__
#define __ULP__

#include <stdint.h>

#define ULP_MAX_SAMPLING_RATE 750
#define ULP_SAMPLING_PERIOD_US (1000000/ULP_MAX_SAMPLING_RATE)


/* 
 * init_ulp_program() - Initialize Ultra-Low-Power core
 *
 * This function is called once after power-on reset, to load ULP program into
 * RTC memory, configure the ADC and run the ULP program.
 */
void ulp_init();


extern const uint8_t ulp_bin_start[] asm("_binary_ulp_adc_bin_start");
extern const uint8_t ulp_bin_end[] asm("_binary_ulp_adc_bin_end");

extern uint32_t ulp_entry;
extern uint32_t ulp_sample[4];

#endif

