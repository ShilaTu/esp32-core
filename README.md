# LifeSensor -- Esp32-core
ESP32 firmware for the lifesensor project.

## Components
See [components/README.md](components/README.md) for an overview of features. 

## Building

* via Docker
  * Run `make help` to see all options
  1. Choosing device
     * `make dev`
       * configures which serial port to use
  2. Building
     * `make build`
       1. creates idf docker image if needed
       2. starts idf docker container to build project
  3. Flashing
     * `make flash`
       1. creates idf docker image if needed
       2. starts idf docker container to flash project to connected ESP via specified device by `make dev`
  4. Monitoring
     * `make monitor`
       1. creates idf docker image if needed
       2. starts idf docker container to connect to ESP via specified device by `make dev`
* via local esd-idf toolchain
  1. Building
     * `idf.py build`
  2. Flashing
     * `idf.py flash -p <device>`
  3. Monitoring
     * `idf.py monitor -p <device>`

## Testing
* via docker
  * Run `make help` to see all options
  1. `make test`
     1. creates idf docker image
     2. starts idf docker container to build test project (`make build-test`)
     3. starts idf docker container to flash the test project to ESP via specified device by `make dev` (`make flash-test`)
     4. starts idf docker container to connect to ESP via specified device by `make dev` (`make monitor-test`)
* via local esd-idf toolchain
  1. Change to test project
     * `cd test`
  2. Building
     * `idf.py build`
  3. Flashing
     * `idf.py flash -p <device>`
  4. Monitoring
     * `idf.py monitor -p <device>`

## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md)