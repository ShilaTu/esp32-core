#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "ls_task.h"
#include "spo2.h"
#include "spo2_driver.h"


ls_task spo2_task;


/**
 * do_spo2() - spo2 task worker function
 * @pvParameters:	void* to pass function parameters.
 *
 * This worker function retrieves an input data sample from the ADCs. This
 * is the place where further processing of the data will happen.
 */
static void do_spo2(void *pvParameters);

/**
 * spo2_read() - read a single input data sample
 * @data:	space for sample readings
 *
 * Read out a single data sample using the different peripherals needed.
 */
static void spo2_read(uint32_t *data);

/**
 * print_values() - print aquired values
 * @data:	space for sample readings
 *
 * Print values to serial out that have been aquired through the peripherals
 * for demonstration and/or debug purpuses.
 */
static void print_values(uint32_t *data);


void
spo2
(void)
{
	spo2_init_peripherals();

	spo2_task.handle = xTaskCreateStatic(
		do_spo2,
		SPO2_TASK_NAME,
		LS_TASK_STACK_SIZE,
		NULL,
		tskIDLE_PRIORITY,
		spo2_task.stack,
		&spo2_task.data
	);
}


static
void
do_spo2
(void *pvParameters)
{
	uint32_t data[4] = { 0 };

	for(;;)
	{
		spo2_read(data);
		print_values(data);
		vTaskDelay(SPO2_READ_PERIOD);
	}
}

static
void
spo2_read(uint32_t *data)
{
	spo2_read_adc(data);
}

static
void
print_values
(uint32_t *data)
{
	printf(
		"RED-DC=%-4d IRD-DC=%-4d RED-AC=%-4d IRD-AC=%-4d\n",
		data[0], data[1], data[2], data[3]
	);
}
