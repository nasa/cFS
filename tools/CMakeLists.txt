# CMake snippet for building the host-side tools.
project(CFETOOLS C)

add_subdirectory(cFS-GroundSystem/Subsystems/cmdUtil)
add_subdirectory(elf2cfetbl)
add_subdirectory(tblCRCTool)

