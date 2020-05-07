<!-- Docker image <TITLE> (version <VERSION>) -->
# Docker image template (version v1)

<!-- SHORT DESCRIPTION -->
This template should be used to create a new docker image version.
1. Copy this template directory
2. Give your new image a meaningful name.
3. Append your new version to the version list in [../README.md](../README.md)
4. Edit this README
   - Change title & version!
5. Edit the [Dockerfile](./Dockerfile)
6. Edit the [Makefile](./Makefile)

---
## Features
<!-- LIST OF FEATURES -->
- installs `software one`
- installs `software two`

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
- `FOO`
  - modifies something
- `BAR`
  - modifies something else

---
See the [Dockerfile](./Dockerfile) or [Makefile](./Makefile) for more information
