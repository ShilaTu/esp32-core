# Template component
<!-- Describe what this component does. -->
This is a template, which can be used to quickly add new components to the project.
Simply copy this directory to create a new component.

A component in this project has following structure:
```
component
├── README.md
├── CMakeLists.txt
├── include
│   ├── public_header.h
│   └── ...
├── src
│   ├── private_header.h
│   ├── ...
│   ├── private_code.h
│   └── ...
└── test
    ├── CMakeLists.txt
    ├── test_header.h
    ├── ...
    ├── test_code.c
    └── ...
```

- `/component` is the name of the component
  - **ATTENTION**: a component will replace any other component with the same name
- `/component/README.md` gives additional information about the component
- `/component/CMakeLists.txt` defines how the component should be build and what to export
- `/component/include/*.h` are public header files for other components   
- `/component/src/*.{c,h}` are the private source files of the component
- `/component/test/CMakeLists.txt` defines how the unit test should be build
- `/component/test/*.{c,h}` are the unit test files of the component

## Concept
<!-- Describe the underlying concept. -->

This component implements a private dummy counter variable which can be manipulated through various simple functions.

## Examples
<!-- Add code snippets here to show common use cases. -->

### Reset
```c
#include "counter.h"
Counter zero = counter_reset();
```

### Increment
```c
#include "counter.h"
Counter inc = counter_increment();
```

### Decrement
```c
#include "counter.h"
Counter dec = counter_decrement();
```

### Add
```c
#include "counter.h"
Counter added = counter_add(5);
```

### Sub
```c
#include "counter.h"
Counter sub = counter_sub(7);
```

## Related
<!-- List of related things -->
- [idf build-system documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html)