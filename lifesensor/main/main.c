#include "ulp.h"
#include "spo2.h"
#include "spo2_driver.h"


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
	lifesensor_spo2_init(&spo2, "SpO2");
	ulp_init(&spo2.adc_queue.queue);
}
