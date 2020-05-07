<!-- Docker image <TITLE> (version <VERSION>) -->
# Docker image base (version v1)

<!-- SHORT DESCRIPTION  -->
This is the base image of all other images.

## Features
<!-- LIST OF FEATURES -->
- installs `sudo`, `make`, `git`
- creates user `developer`
- adds `developer` to sudoers

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
- `UID`
  - sets uid of `developer`
- `GID`
  - sets gid of `developer`

---
See the [Dockerfile](./Dockerfile) or [Makefile](./Makefile) for more information
