# Main build
This directory contains the main build of the project.

## Build & Flash & Run

* via Docker
  2. Building
     * `make build`
       1. creates idf docker image if needed
       2. starts idf docker container to build project
  3. Choosing device
     * `make dev`
       * configures which serial port to use
  4. Flashing
     * `make flash`
       1. creates idf docker image if needed
       2. starts idf docker container to flash project to connected ESP via specified device by `make dev`
  5. Monitoring
     * `make monitor`
       1. creates idf docker image if needed
       2. starts idf docker container to connect to ESP via specified device by `make dev`
  6. Static Code analysis
      * `make check-code`
        1. creates format docker image if needed
        2. starts format docker container to run `cppcheck` on all src and header files
  7. Code formatting
      * `make format-code`
        1. creates format docker image if needed
        2. starts format docker container to run `clang-format-9` on all src and header files
* via local esd-idf toolchain
  2. Building
     * `idf.py build`
  3. Flashing
     * `idf.py flash -p <device>`
  4. Monitoring
     * `idf.py monitor -p <device>`
