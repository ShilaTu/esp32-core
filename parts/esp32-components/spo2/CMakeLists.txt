idf_component_register(
  SRC_DIRS
    src
  INCLUDE_DIRS
    include
  REQUIRES
    ulp_adc
    lifesensor_common
)

# COMPONENT COMPILE FLAGS
target_compile_options(${COMPONENT_LIB} PRIVATE
  -Wall
  -Werror
)

# SRC SPECIFIC COMPILE FLAGS
#set_source_files_properties(
#  src/counter.c
#  PROPERTIES COMPILE_FLAGS
#    -Wno-unused-variable
#)
