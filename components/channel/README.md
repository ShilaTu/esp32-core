# Channel component
<!-- Describe what this component does. -->
Channels are objects which enable iteration over other Channel objects with the same name/identifier.

Each Channel object contains a context pointer, a callback function and
an additional flags field.
The callback is issued with a data pointer and a timeout value by the caller and the callee specific context and flags.
This interface is intended to enable dynamic management of FreeRTOS queues,
where a queue can be registered as a context
of a Channel and therefore fed by the Channels callback.

## Concept
<!-- Describe the underlying concept. -->
This implementation resembles a map, mapping identifiers to lists. 
Channels are implemented as an item of two separate lists.
The "unique" list holds items with different identifiers,
the "same" list holds items that share the same identifier. 
Each list item itself is a valid list containing only itself;
multiple items can be combined to a larger list.

This implementation constructs a single valid "unique" list,
starting with a globally defined root node.
All "same" lists of all items are considered valid.
Adding to this structure implies iterating the "unique" list,
until either the same identifier is found (and the new item is added to the "same" list of the found item) or the end of the "unique" list reached (and it's appended as new "unique" list item).
Each added Channel object is now part of a "same" list which can be directly iterated to find other items.

exemplary structure:
```
             root node---.  O        O
                         v  |        |
unique identifier list-> O--O--O--O--O  
                            |     |
                            O     O
                            ^
     same identifier list---'   
```
## Examples
<!-- Add code snippets here to show common use cases. -->

### Simple Input channel
```c
#include "channel.h"

QueueHandle_t queue;
Channel_in input;

void app_main() {
  queue = xQueueCreateStatic(/*...*/);
  channel_init_input(&input, "input", queue);
}
```

### Custom Input channel
```c
#include "channel.h"

static void *latest_data;

BaseType_t callback (void *ctx,
                    const void *data,
                    const TickType_t timeout,
                    const BaseType_t flags) {
  latest_data = data;
}

Channel_in input;

void app_main() {
  channel_init(&input, "input", data, 0, &callback);
}
```

### Output channel
```c
#include "channel.h"

Channel_out output;

void app_main() {
  channel_init_output(&output, "output");
}
```

### Simple broadcast without error handling
```c
#include "channel.h"

Channel_out output;
char data[] = "test";

void app_main() {
  channel_init_output(&output, "output");

  Channel_broadcast broadcast;
  channel_broadcast_init(&broadcast, &output, data, 0);
  while(!channel_broadcast_finished(&broadcast)) {
    channel_broadcast(&broadcast);
  }
}
```


<!-- ## Related -->
<!-- List of related things -->
