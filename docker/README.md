# Docker

This directory contains Dockerfiles for the containers used in this project.

---
## List of images
- [base](./base/)
  - base image for all other images
- [idf](./idf/)
  - idf framework for the esp32 firmware
- [qemu](./qemu/)
  - qemu for architecture simulation

---
## Makefile
<!-- LIST OF MAKEFILE TARGETS -->
- `make help`
  - show Makefile options
- `make build`
  - build all versions of all images
- `make clean`
  - delete all volumes of all versions of all images
- `make distclean`
  - delete all all versions of all images

---
## Run a container
1. change into the version directory of the image
2. run `make run`
   - you may specify additional options, see `make help` for more information
   - you may specify `EXEC` to run a single command
     - i.e. `make run EXEC='echo hello world'`

---
## Adding new images
- If you want to add a new image,
please copy and modify the [Template](./.template/)

---
## Structure
```
.
├── README.md              : mandatory README.md you are currently reading
├── Makefile               : mandatory Makefile executing targets in all subdirs
│
├── <image name>           : mandatory unique name of the image
│   ├── README.md          : mandatory general description of the image
│   │                        and elaboration of differences between version
│   ├── Makefile           : mandatory Makefile executing targets in all subdirs
│   │
│   ├── <image version>    : mandatory unique image version
│   │   ├── README.md      : mandatory version specific description of the image,
│   │   │                    should list all features
│   │   ├── Makefile       : mandatory Makefile for integration of the image
│   │   ├── Dockerfile     : mandatory Dockerfile to build image
│   │   └── ...
│   │
│   ├── <image version>
│   │   └── ...
│   └── ...
│
├── <image name>
│   └── ...
└── ...
```

---
## Rules
- *Docker images* of the LifeSensor project should have a dedicated directory here.
- *Docker images* can be based on each other (see [base](./base/) image).
- *Docker images* may compete for the same functionality.
  - Different [*parts*](../parts/) may use different *docker images*
- *Docker images* should follow the directory scheme defined by the [*template image*](./.template/)
  - *Docker images* may contain any additional files & directories
