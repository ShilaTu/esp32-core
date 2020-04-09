# Lifesensor common component
For ease of declaration and definition of groups of tasks and queues the *lifesensor_macros* offer a set of easy to use task and queue creation shortcuts.

## Macros

### Concept
The `LIFESENSOR_TASK()` and `LIFESENSOR_QUEUE()` macro offer an easy interface to descibe the struct for a functional unit of the lifesensor. Each task or queue created with these shortcuts have to be initialized with the `lifesensor_task_init()` and respectively the `lifesensor_queue_init()` function.

`LIFESENSOR_TASK(stacksize)`  
Construct a struct for a task. The `stacksize` is given in counts of `StackType_t` not in bytes. You can make use of `MACRO_DEFAULT_STACK_SIZE`. The structs `task` member will be the actual `lifesensor_task_t` which in turn contains the `StaticTask_t`.

`LIFESENSOR_QUEUE(type, itemcount)`  
Construct a struct for a queue. The `type` attribute must be a valid datatype and will be used with `sizeof`. `itemcount` is the number of items of `type` which fit in the queue to be constructed. The structs `queue` memver will be the actual `lifesensor_queue_t` variable. Which in turn contains the `StaticQueue_t`.

`lifesensor_task_init(anon_task, name, func, args, prio)`  
Initiazes a task created with `LIFESENSOR_TASK()` by passing in a pointer to it as first argument. For the `name` parameter a `const char *` is expected. `func` is expected to be a function pointer with its arguments passed in `args` as a `void *`. You can use `tskIDLE_PRIORITY` for the task priotity in the `prio` argument.

`lifesensor_queue_init(anon_queue)`  
Initalizes a queue created with `LIFESENSOR_QUEUE()` by passing in a pointer to it as first argument. Generally this function should be called before passing a queue to i.e. a task as argument.


### Examples
```C
typedef struct {
	LIFESENSOR_QUEUE(spo2_adc_sample_t, 16) adc_queue;
	LIFESENSOR_TASK(4096) adc_task;
} spo2_t;


spo2_t spo2;
```

```C
void app_main()
{
	lifesensor_queue_init(spo2.adc_queue);
	lifesensor_task_init(
		spo2.adc_task,
		"spo2_adc",
		spo2_runner,
		&spo2.adc_queue.queue,
		tskIDLE_PRIORITY
	);
}

```

### Related
none
