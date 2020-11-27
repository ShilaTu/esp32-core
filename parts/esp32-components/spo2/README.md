# SpO2 component
<!-- Describe what this component does. -->
This component is a port of the original remo2hbo SpO2 firmware.
It controls the analog filters and uses the ESP internal ADCs to sample the output.

## Concept
<!-- Describe the underlying concept. -->
Instantiate the SpO2 task & queue and initialize them to start the task.

## How to use
<!-- Add code snippets here to show common use cases. -->

### Instantiation
```c
#include "spo2.h"
static uint8_t spo2_queue_buffer[SPO2_QUEUE_BUFFER_SIZE];
static StackType_t spo2_task_stack[SPO2_TASK_STACK_SIZE];

_spo2_queue spo2_queue = {
	.length = SPO2_QUEUE_LENGTH,
	.item_size = SPO2_QUEUE_ITEM_SIZE,
	.buffer = spo2_queue_buffer
};

_spo2_task spo2_task = {
	.name = SPO2_TASK_NAME,
	.priority = tskIDLE_PRIORITY,
	.stack = spo2_task_stack
};
```

### Initialization
```c
#include "spo2.h"
void app_main() {
  spo2_init(&spo2_task, &spo2_queue);
}
```

## Related
<!-- List of related things -->
- [remo2hbo SpO2 Schematics](https://github.com/cadus/Hardware-Circuit-Boards/blob/master/Schematics%20and%20PCB%20as%20PDF%20and%20PNG/Pulsoxy_schematic.pdf)
- [remo2hbo SpO2 firmware](https://github.com/cadus/Hardware-Circuit-Boards/tree/master/Source_codes/SPO2)