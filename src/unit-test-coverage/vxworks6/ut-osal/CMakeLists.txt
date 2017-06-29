include_directories(inc)
include_directories(${OSAL_SOURCE_DIR}/src)
# Flag to common_types.h to not add any special attributes/pragmas to prototypes
add_definitions(-DOSAPI_NO_SPECIAL_ATTRIBS)
aux_source_directory(src SRCFILES)
foreach(FILE ${SRCFILES})
  get_filename_component(MODULE ${FILE} NAME_WE)
  add_library(ut_osal_${MODULE} STATIC ${FILE})
  set_target_properties(ut_osal_${MODULE} PROPERTIES COMPILE_FLAGS "${UT_C_FLAGS}")
endforeach()
  