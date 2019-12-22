#include "spo2.h"


_spo2_task spo2_task;


/**
 * app_main() - main thread after boot process
 *
 * The prototype should be part of the esp32 idf framework. This is the entry
 * point of the life-sensor application which is used to setup necessary memory
 * objects and start necessary threads.
 */
void app_main()
{
	spo2_init("SpO2", &spo2_task);
}
