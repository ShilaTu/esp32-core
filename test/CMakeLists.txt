cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS "../components" )

set(LIFESENSOR_COMPONENTS "")

file(GLOB component_dirs ${EXTRA_COMPONENT_DIRS}/*)
foreach(component_dir ${component_dirs})
  if(EXISTS ${component_dir}/CMakeLists.txt)
    get_filename_component(component "${component_dir}" NAME)
    list(APPEND LIFESENSOR_COMPONENTS ${component})
  endif()
endforeach()

set(
  TEST_COMPONENTS
    ${LIFESENSOR_COMPONENTS}
  CACHE STRING
    "List of components to test"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(lifesensor_test)
