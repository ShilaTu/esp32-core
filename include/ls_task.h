#ifndef __LIFE_SENSOR_TASK__
#define __LIFE_SENSOR_TASK__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LS_TASK_STACK_SIZE 0x1000


/**
 * struct _ls_task - life-sensor task struct
 * @data:	task related data
 * @handle:	a handle to reference the task
 * @stack:	the address of the tasks' stack
 *
 * This struct serves as a shortcut for operating with FreeRTOS tasks such
 * that it includes necessary fields for managing threads.
 */
struct _ls_task {
	StaticTask_t data;
	TaskHandle_t handle;
	StackType_t stack[LS_TASK_STACK_SIZE];
};

typedef struct _ls_task ls_task;

#endif