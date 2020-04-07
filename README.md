# LifeSensor -- Esp32-core
ESP32 firmware for the lifesensor project.

## Components
See [components/README.md](components/README.md) for an overview of features. 

## Project flavors
This repository features multiple idf projects for different purposes.

<dl>
<dt>lifesensor</dt><dd>main project</dd>
<dt>lifesensor_test</dt><dd>unit tests of components</dd>
</dl>

## Build & Flash & Run

* via Docker
  1. Choose flavor
     * i.e. `cd lifesensor`
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
  1. Choose flavor
     * i.e. `cd lifesensor`
  2. Building
     * `idf.py build`
  3. Flashing
     * `idf.py flash -p <device>`
  4. Monitoring
     * `idf.py monitor -p <device>`

## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md)
