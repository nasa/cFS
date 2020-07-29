cmake_minimum_required(VERSION 2.6.4)
project(CFS_SCH C)

include_directories(fsw/public_inc)
include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)
include_directories(fsw/tables)
include_directories(fsw/src)

#required for table id defs
include_directories(${MISSION_SOURCE_DIR}/sample_defs)
include_directories(${eps_MISSION_DIR}/fsw/platform_inc)
include_directories(${comms_MISSION_DIR}/fsw/platform_inc)
include_directories(${payload_MISSION_DIR}/fsw/platform_inc)

aux_source_directory(fsw/src APP_SRC_FILES)

# Create the app module
add_cfe_app(sch ${APP_SRC_FILES})

if (COMMAND add_cfe_tables)
    include_directories(fsw/tables)
    aux_source_directory(fsw/tables APP_TBL_FILES)
    add_cfe_tables(sch fsw/tables/sch_def_schtbl.c fsw/tables/sch_def_msgtbl.c)
endif(COMMAND add_cfe_tables)
