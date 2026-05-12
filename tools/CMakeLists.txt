# CMake snippet for building the host-side tools.
project(CFETOOLS C)

add_subdirectory(tblCRCTool)
add_subdirectory(commandline-tools)

if (CFE_EDS_ENABLED)
    # build the EDS tool set which is used later in the build
    message(STATUS "Setting up EDS toolchain build...")
    add_subdirectory(eds/edslib)
    add_subdirectory(eds/tool)
    add_subdirectory(eds/cfecfs)
else(CFE_EDS_ENABLED)
    add_subdirectory(elf2cfetbl)
endif(CFE_EDS_ENABLED)
