cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS "components" )

get_filename_component(PROJECT_PATH "." ABSOLUTE)
get_filename_component(PROJECT_NAME ${PROJECT_PATH} NAME)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(${PROJECT_NAME})
