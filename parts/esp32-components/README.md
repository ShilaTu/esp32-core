<!-- Part <TITLE> -->
# Part Esp32 Components

<!-- SHORT DESCRIPTION OF YOUR PART -->
"Component" is a term for structures used by the espressif idf framework,
separating code into libraries and defining dependencies.

<!-- ---
## Features
LIST OF FEATURES -->

---
## Makefile
<!-- LIST OF MAKEFILE TARGETS -->
- `make help`
  - show Makefile options
- `make all`
  - build & test all variants
- `make build`
  - build all variants
- `make test`
  - test all variants
- `make clean`
  - clean all variants
- `make distclean`
  - distclean all variants

---
## Variants
<!-- PLEASE LIST ALL VARIANTS OF YOUR PART    -->
<!-- INCLUDING A SHORT DESCRIPTION OF CHANGES -->
<!-- LATEST VERSION SHOULD BE ON TOP!         -->
* [ulp_adc](./ulp_adc/)
  * Utilize the Ultra-low-power Co-processor to sample the ADCs of the ESP
* [SpO2](./spo2/)
  * Driver for the original remo2hbo SpO2 hardware 
* [linux](./linux/)
  * Source code that originates from the linux kernel 
* [lifesensor_common](./lifesensor_common/)
  * Commonly used structures and defines 
* [channel](./channel/)
  * Event abstraction for FreeRTOS


---
## Adding a new component
See the [.template/](./.template/) component.
