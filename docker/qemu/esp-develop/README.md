<!-- Docker image <TITLE> (version <VERSION>)-->
# Docker image qemu (version esp-develop)

<!-- SHORT DESCRIPTION -->
This is the qemu image used to simulate the esp32.

---
## Features
<!-- LIST OF FEATURES -->
- installs xtensa-architecture capable [qemu](https://github.com/espressif/qemu) & dependencies

---
## Makefile
<!-- LIST OF MAKEFILE TARGETS -->
- `make help`
  - show Makefile options
- `make all`
  - build image & run container
- `make build`
  - build image
- `make run`
  - run container
- `make clean`
  - delete volumes
- `make distclean`
  - delete image

---
## Image arguments
<!-- LIST OF BUILD ARGUMENTS -->
- `QEMU_VERSION`
  - qemu version to use

---
See the [Dockerfile](./Dockerfile) or [Makefile](./Makefile) for more information