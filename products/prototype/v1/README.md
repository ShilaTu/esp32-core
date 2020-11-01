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
* via local esd-idf toolchain
  1. Choose flavor
     * i.e. `cd lifesensor`
  2. Building
     * `idf.py build`
  3. Flashing
     * `idf.py flash -p <device>`
  4. Monitoring
     * `idf.py monitor -p <device>`

## Simulating with qemu

**Important:** Qemu port of the xtensa architecture is in active development,
each docker image creation builds with latest version, mileage may vary!

* via Docker
  * simple execution
    1. Choose flavor
       * i.e. `cd lifesensor`
    2. Building
       * `make build`
         1. creates idf docker image if needed
         2. starts idf docker container to build project
    3. Start qemu
       * `make qemu`
         1. creates qemu docker image if needed
         2. starts qemu docker container to simulate project
  * with gdb
    1. Choose flavor
       * i.e. `cd lifesensor`
    2. Building
       * `make build`
         1. creates docker docker image if needed
         2. starts qemu docker container to simulate project
    3. Start qemu and wait for gdb connection
       * `make qemu-gdb`
         1. creates qemu docker image if needed
         2. starts qemu docker container to simulate project
    5. Start gdb and connect to qemu in second terminal
       * `make gdb-qemu`
         1. starts idf docker container to run gdb

## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md)
