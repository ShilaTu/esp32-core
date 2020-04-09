#include "esp32/ulp.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_cntl.h"

#include "spo2.h"
#include "ulp.h"
#include "macro/queue.h"


/*
 * ulp_isr() - Ultra-low-power coprocessor interrupt service routine
 * @args	void pointer to isr arguments
 *
 * After registering this function as RTC ISR using rtc_isr_register()
 * the function will will the spo2 queue everytime a new sample from the ulp
 * is available.
 */
static void ulp_isr(void *args);


void
ulp_init
(lifesensor_queue_t *spo2_queue)
{
	/* write ulp binary to RTC_SLOW_MEM */
	size_t program_size = (ulp_bin_end - ulp_bin_start) / sizeof(uint32_t);
	ESP_ERROR_CHECK(ulp_load_binary(0, ulp_bin_start, program_size));

	/* set reload period */
	ESP_ERROR_CHECK(ulp_set_wakeup_period(0, ULP_SAMPLING_PERIOD_US));

	/* register ulp isr */
	ESP_ERROR_CHECK(rtc_isr_register(
		&ulp_isr,
		(void*)spo2_queue,
		RTC_CNTL_SAR_INT_ST_M
	));

	/* enable rtc interupt */
	REG_SET_BIT(RTC_CNTL_INT_ENA_REG, RTC_CNTL_ULP_CP_INT_ENA_M);

	/* run ulp program */
	ESP_ERROR_CHECK(ulp_run(&ulp_entry - RTC_SLOW_MEM));
}


static
void
ulp_isr
(void *args)
{
	xQueueSendFromISR(((lifesensor_queue_t*)args)->handle, (void*)ulp_sample, pdFALSE);
}
