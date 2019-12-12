#ifndef _ULP_ADC_EXAMPLE_H_
#define _ULP_ADC_EXAMPLE_H_


/* This function is called once after power-on reset, to load ULP program into
 * RTC memory and configure the ADC.
 */
void init_ulp_program();

/* This function is called every time before going into deep sleep.
 * It starts the ULP program and resets measurement counter.
 */
void start_ulp_program();


extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[]   asm("_binary_ulp_main_bin_end");


/* These variable should be exported from ulp assembly adc.S via preprocessor 
 * directives and correspond to variables in asssembly without ulp_ prefix
 */
extern uint32_t ulp_low_thr;
extern uint32_t ulp_high_thr;
extern uint32_t ulp_sample_counter;
extern uint32_t ulp_entry;
extern uint32_t ulp_last_result;

#endif /* _ULP_ADC_EXAMPLE_ */