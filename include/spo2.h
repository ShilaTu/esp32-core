#ifndef __SPO2__
#define __SPO2__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define SPO2_STACK_SIZE 0x1000


/**
 * struct _spo2_task - life-sensor task struct
 * @tcb:	task related data
 * @handle:	a handle to reference the task
 * @stack:	the address of the tasks' stack
 *
 * This struct serves as a shortcut for operating with FreeRTOS tasks such
 * that it includes necessary fields for managing threads.
 */
typedef struct {
	StaticTask_t tcb;
	TaskHandle_t handle;
	StackType_t stack[SPO2_STACK_SIZE];
} _spo2_task;


/**
 * spo2_init() - SpO2 (Pulsoxy) meassurement device interface initialization
 *
 * spo2_init() initializes peripherals (4xADC, 1xPWM, 2xGPIO) that is necessary
 * to operate together with the pulsoxy board which does preprocessing of input
 * data. After initialization a thread is started which periodically reads in
 * input data.
 */
void spo2_init(const char *name, _spo2_task *spo2_task);

#endif 
