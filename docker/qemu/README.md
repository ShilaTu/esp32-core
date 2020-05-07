<!-- Docker image <TITLE> -->
# Docker image qemu

<!-- SHORT DESCRIPTION -->
This is the qemu image used to simulate various architectures.

---
## Features
<!-- LIST OF FEATURES -->
- installs qemu

---
## Makefile
<!-- LIST OF MAKEFILE TARGETS -->
- `make help`
  - show Makefile options
- `make build`
  - build all versions
- `make clean`
  - delete volumes of all versions
- `make distclean`
  - delete images of all version

---
## Versions
<!-- PLEASE LIST ALL VERSIONS OF YOUR PART    -->
<!-- INCLUDING A SHORT DESCRIPTION OF CHANGES -->
<!-- LATEST VERSION SHOULD BE ON TOP!         -->
- [esp-develop](./esp-develop/)
  - qemu fork for xtensa (esp32) architecture