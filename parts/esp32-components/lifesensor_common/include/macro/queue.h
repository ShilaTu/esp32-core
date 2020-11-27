#ifndef __MACRO_QUEUE__
#define __MACRO_QUEUE__

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>


/**
 * struct lifesensor_queue_t - generic queue struct
 * @queue:	Holds the FreeRTOS queue.
 * @handle: Provides a reference to the queue after initialization.
 * @buffer: Points to the subsequent queue buffer.
 *
 * This struct serves as an abstraction for operating with FreeRTOS queues such
 * that it includes necessary fields for managing a queues data. It is intended
 * to be used with the LIFESENSOR_QUEUE() macro.
 */
typedef struct {
	QueueHandle_t handle;
	StaticQueue_t queue;
	uint8_t buffer[];
} lifesensor_queue_t;


/**
 * LIFESENSOR_QUEUE() - Generic queue struct declaration macro
 * @type:		The variable type the queue will hold.
 * @item_count	Provide how many items of `type` fit into the queue..
 *
 * The macro is used for easier queue struct allocation. The resulting
 * queue struct should be initialized through the corresponding
 * lifesensor_queue_init() function macro.
 *
 * NOTE:	`queue` houses the actual queue struct of type `lifesensor_queue_t`.
 * 			`lifesensor_queue_t`s buffer attribute of undefined length will
 * 			point to the buffer declared here with size `buffersize`.
 *
 */
#define LIFESENSOR_QUEUE(type, item_count)                                  \
	struct {                                                                \
		lifesensor_queue_t queue;                                           \
		type buffer[item_count];                                            \
	}


/**
 * lifesensor_queue_init() - Initialize a queue generated with LIFESENSOR_QUEUE
 * @anon_queue:		Pass a pointer to an instacne created with LIFESENSOR_QUEUE.
 *
 * This is a wrapper for `xTaskCreateStatic()` which initializes several
 * internal variables. It provides another layer of abstraction to not bother
 * much with the FreeRTOS internals and hides redundant code.
 *
 * Please consult https://www.freertos.org/xTaskCreateStatic.html
 */
#define lifesensor_queue_init(anon_queue)                                   \
{                                                                           \
	(*anon_queue).queue.handle = xQueueCreateStatic(                        \
		sizeof((*anon_queue).buffer) / sizeof((*anon_queue).buffer[0]),     \
		sizeof((*anon_queue).buffer[0]),                                    \
		(*anon_queue).queue.buffer,                                         \
		&(*anon_queue).queue.queue                                          \
	);                                                                      \
}

#endif
