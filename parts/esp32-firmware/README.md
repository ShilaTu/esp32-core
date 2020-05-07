<!-- Part <TITLE> -->
# Part Esp32 Firmware

<!-- SHORT DESCRIPTION OF YOUR PART -->
This is the firmware for the esp32 that controls the various sensor boards.

---
## Features
<!-- LIST OF FEATURES -->
- modular by design
- consists of components

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
- [prototype](./prototype/)
  - firmware of the current prototype
- [test](./test/)
  - firmware for on-device component testing
- [qemu](./qemu/)
  - firmware for component testing in qemu