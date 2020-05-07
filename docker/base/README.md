<!-- Docker image <TITLE> -->
# Docker image base

<!-- SHORT DESCRIPTION -->
This is the base image of all other images.

---
## Features
<!-- LIST OF FEATURES -->
- setup of user `developer`

---
## Makefile
<!-- LIST OF MAKEFILE TARGETS -->
- `make help`
  - show Makefile options
- `make all`
  - build all versions
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
- [v1](./v1/)
  - initial version