<!-- Docker image <TITLE> -->
# Docker image template

<!-- SHORT DESCRIPTION -->
This template should be used to create a new image.
1. Copy this template directory
2. Give your new image a meaningful name.
3. Append your new image to the image list in [../README.md](../README.md)
4. Edit this README
   - Change title!
5. Continue editing in [v1/README.md](./1/README.md)

---
## Features
<!-- LIST OF FEATURES -->
- provides template for docker image

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