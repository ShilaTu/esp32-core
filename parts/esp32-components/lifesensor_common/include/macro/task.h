#ifndef __MACRO_TASK__
#define __MACRO_TASK__

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define MACRO_DEFAULT_STACK_SIZE 0x1000


/**
 * struct lifesensor_task_t - generic task struct
 * @tcb:    Holds the FreeRTOS tcb.
 * @handle: Provides a reference to the task.
 * @stack:  Stores the address of the tasks' stack.
 *
 * This struct serves as an abstraction for operating with FreeRTOS tasks such
 * that it includes necessary fields for managing a tasks data. It is intended
 * to be used together with the LIFESENSOR_TASK() macro.
 */
typedef struct {
	TaskHandle_t handle;
	StaticTask_t tcb;
	StackType_t stack[];
} lifesensor_task_t;


/**
 * LIFESENSOR_TASK() - Generic task struct declaration macro
 * @stacksize:  Determines the stack depth used.
 *
 * The macro is used for easier task struct allocation. The resulting
 * task struct should be initialized through the corresponding 
 * `lifesensor_task_init()` function macro.
 *
 * NOTE:	`task` houses the actual task struct of type `lifesensor_task_t`.
 * 			`lifesensor_task_t`s stack attribute of undefined length will
 * 			point to the stack declared here with size `stacksize`.
 *
 */
#define LIFESENSOR_TASK(stacksize)                                          \
	struct {                                                                \
		lifesensor_task_t task;                                             \
		StackType_t stack[stacksize];                                       \
	}


/**
 * lifesensor_task_init() - Initialize a task generated with LIFESENSOR_TASK
 * @anon_task:	Pass a pointer to instance created with LIFESENSOR_TASK.
 * @name:		Name the task to recognize it later.
 * @func:		Provide a function pointer to execute in task.
 * @args:		Function arguments that will be fed to `func`.
 * @prio:		Set the desired task priority.
 *
 * This is a wrapper for `xTaskCreateStatic()` which initializes several
 * internal variables. It provides another layer of abstraction to not bother
 * much with the FreeRTOS internals and hides redundant code.
 *
 * Please consult https://www.freertos.org/xTaskCreateStatic.html
 */

#define lifesensor_task_init(anon_task, name, func, args, prio)             \
{                                                                           \
	(*anon_task).task.handle = xTaskCreateStatic(                           \
		func,                                                               \
		name,                                                               \
		sizeof((*anon_task).stack) / sizeof((*anon_task).stack[0]),         \
		args,                                                               \
		prio,                                                               \
		(*anon_task).stack,                                                 \
		(StaticTask_t*)(&(*anon_task).task.tcb)                             \
	);                                                                      \
}

#endif
