# ULP ADC component
<!-- Describe what this component does. -->
This component utilizes the ultra-low-power co-processor of the ESP to sample ADCs.

## Concept
<!-- Describe the underlying concept. -->
The ULP samples the ADCs in parallel to the ESP and copies the values into normal memory via an ISR.

## How to use
<!-- Add code snippets here to show common use cases. -->

### Initialization
```c
#include "ulp.h"
void app_main()
{
	ulp_init(&spo2_queue);
}
```

## Related
<!-- List of related things -->
- [ULP programming](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/ulp.html)