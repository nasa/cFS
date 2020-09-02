#!/bin/bash

cp -r cfe/cmake/sample_defs sample_defs
cp -f /cfs/cfe/cmake/sample_defs/global_build_options.cmake ./sample_defs/global_build_options.cmake
make SIMULATION=native ENABLE_UNIT_TESTS=false prep

# turn on bash's job control
set -m

# Start the primary process and put it in the background
./build/exe/cpu1/core-cpu1 >cFS_startup.txt &

# Start the helper process
./build/exe/host/cmdUtil --endian=LE --pktid=0x1806 --cmdcode=2 --half=0x0002

./build/build/tools/cFS-GroundSystem/Subsystems/cmdUtil/cmdUtil --endian=LE --pktid=0x1806 --cmdcode=2 --half=0x0002

Executive Services, CFE_ES_CMD, ES No-Op, 0, 0x1806, LE, 127.0.0.1, 1234, CFE_ES_NOOP_CC
Software Bus, CFE_SB_CMD, SB No-Op, 0, 0x1803, LE, 127.0.0.1, 1234, CFE_SB_NOOP_CC
Table Services, CFE_TBL_CMD, TBL No-Op, 0, 0x1804, LE, 127.0.0.1, 1234, CFE_TBL_NOOP_CC
Time Services, CFE_TIME_CMD, Time No-Op, 0, 0x1805, LE, 127.0.0.1, 1234, CFE_TIME_NOOP_CC
Event Services, CFE_EVS_CMD, EVS No-Op, 0, 0x1801, LE, 127.0.0.1, 1234, CFE_EVS_NOOP_CC
# the my_helper_process might need to know how to wait on the
# primary process to start before it does its work and returns

# now we bring the primary process back into the foreground
# and leave it there
fg %1
