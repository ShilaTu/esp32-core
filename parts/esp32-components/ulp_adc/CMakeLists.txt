idf_component_register(
  SRC_DIRS
    src
  INCLUDE_DIRS
    include
  REQUIRES
    ulp
    soc
    driver
    spo2
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

set(ulp_app_name ulp_adc)
set(ulp_s_sources ulp/ulp_program.S)
set(ulp_exp_dep_srcs "ulp.c")

ulp_embed_binary(${ulp_app_name} "${ulp_s_sources}" "${ulp_exp_dep_srcs}")