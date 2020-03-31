# LifeSensor -- Esp32-core
ESP32 firmware for the lifesensor project.

## Components
See [components/README.md](components/README.md) for an overview of features. 

## Project flavors
This repository features multiple idf projects for different purposes.
You can choose which project should be used by specifying the make `PROJECT`
variable if using make or `cd <PROJECT>` if running idf locally.

<dl>
<dt>lifesensor</dt><dd>main project</dd>
<dt>lifesensor_test</dt><dd>unit tests of components</dd>
</dl>

## Building

* via Docker
  * Run `make help` to see all options
  1. Building
     * `make build`
       1. creates idf docker image if needed
       2. starts idf docker container to build project
  2. Choosing device
     * `make dev`
       * configures which serial port to use
  3. Flashing
     * `make flash`
       1. creates idf docker image if needed
       2. starts idf docker container to flash project to connected ESP via specified device by `make dev`
  4. Monitoring
     * `make monitor`
       1. creates idf docker image if needed
       2. starts idf docker container to connect to ESP via specified device by `make dev`
* via local esd-idf toolchain
  1. Choosing project flavor
     * `cd lifesensor`
  2. Building
     * `idf.py build`
  3. Flashing
     * `idf.py flash -p <device>`
  4. Monitoring
     * `idf.py monitor -p <device>`

## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md)
