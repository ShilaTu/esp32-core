cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS "components" )

set(PROJECT_COMPONENTS "")

file(GLOB component_dirs ${EXTRA_COMPONENT_DIRS}/*)
foreach(component_dir ${component_dirs})
  if(EXISTS ${component_dir}/CMakeLists.txt)
    get_filename_component(component "${component_dir}" NAME)
    list(APPEND PROJECT_COMPONENTS ${component})
  endif()
endforeach()

get_filename_component(PROJECT_PATH "." ABSOLUTE)
get_filename_component(PROJECT_NAME ${PROJECT_PATH} NAME)

set(
  TEST_COMPONENTS
    ${PROJECT_COMPONENTS}
  CACHE STRING
    "List of components to test"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(${PROJECT_NAME})
