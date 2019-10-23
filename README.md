Travis-CI: [![Build Status](https://travis-ci.com/nasa/cFS.svg)](https://travis-ci.com/nasa/cFS)

# Core Flight System - BUNDLE

This repository is a bundle of submodules that make up the Core Flight System (cFS) framework.  Note the "lab" apps are intended as examples only, and enable this bundle to build, execute, receive commands, and send telemetry.  This is not a flight distribution, which is typically made up of the cFE, OSAL, PSP, and a selection of flight apps that coorespond to specific mission requirements.

This bundle has not been fully verified as an operational system, and is provided as a starting point vs an end product.  Testing of this bundle consists of building, executing, sending setup commands and verifying receipt of telemetry.  Unit testing is also run, but extensive analysis is not performed.  All verification and validation per mission requirements is the responsibility of the mission (although attempts are made in the cFS Framework to provide a testing framework to facilitate the process).

The cFS Framework is a core subset of cFS.  There are additional OSALs, PSPs, and tools as listed below available from a variety of sources.

## Release Notes

cFE 6.6.0a is released under the Apache 2.0 license, see [LICENSE](https://github.com/nasa/cFE/tree/master/LICENSE-18128-Apache-2_0.pdf).
 - The license covers cFE, PSP, framework apps, and framework tools as marked

OSAL 1.4.2a is released under the NOSA license, see [LICENSE](https://github.com/nasa/osal/blob/master/LICENSE)

Other elements listed below are released under a varitey of licenses as detailed in their respective repositories.

Additional release notes for the cFS Framework are found in [release notes](https://github.com/nasa/cFE/tree/master/docs/cFE_release_notes.md).  See the [version description document](https://github.com/nasa/cFE/tree/master/docs/cFE_6_6_0_version_description.md) for the full version description document.  Test results can be found in [test results](https://github.com/nasa/cFE/tree/master/test-and-ground/test-review-packages/Results).  This is a point release from major development work currently being performed on an internal repository.

## Known issues

Version description document contains references to internal repositories and sourceforge, which is no longer in use.  Markdown document formats have not been updated for GitHub.

The version description document details two build warnings in linux that will be resolved in the future.

Major known issues (not noted in version description document):
Unit test - the nominal os timer test occasionally fails on standard Linux systems (non-RTOS)

## Major future work

  - Certification framework including PSP and OSAL white box testing
  - Open source automated build framework
  - Unit test modernization/standardization
  - Deployment quality of life improvements (configuration)
  - Documentation (updated tracability, APIs/ICDs, general update)
  - Symmetric multi-processing APIs
  - Electronic Data Sheet integration
  - Toolchain updates

## Getting Help

For best results, submit issues:questions or issues:help wanted requests to this repo.

Official cFS page: http://cfs.gsfc.nasa.gov

Community email list subscription request: https://lists.nasa.gov/mailman/listinfo/cfs-community

## Setup

To setup the cFS BUNDLE directly from the latest set of interoperable repositories:

    git clone https://github.com/nasa/cFS.git
    cd cFS
    git submodule init
    git submodule update

Copy in the default makefile and definitions:

    cp cfe/cmake/Makefile.sample Makefile
    cp -r cfe/cmake/sample_defs sample_defs

If running on a standard linux build as a normal user, define OSAL_DEBUG_PERMISSIVE_MODE for best effort message queue depth and task priorities.

    sed -i 's/undef OSAL_DEBUG_PERMISSIVE_MODE/define OSAL_DEBUG_PERMISSIVE_MODE/g' sample_defs/default_osconfig.h

## Build and Run

The cFS Framework including sample applications will build and run on the pc-linux platform support package (should run on most Linux distributions), via the steps described in https://github.com/nasa/cFE/tree/master/cmake/README.md.  Quick-start is below:

To prep, compile, and run (from cFS directory above):

    make prep
    make
    make install
    cd build/exe/cpu1/
    ./core-cpu1

Should see startup messages, and CFE_ES_Main entering OPERATIONAL state.  Note the code must be executed from the build/exe/cpu1 directory to find the startup script and shared objects.

## Send commands, receive telemetry

The cFS-GroundSystem tool can be used to send commands and receive telemetry (see https://github.com/nasa/cFS-GroundSystem/tree/master/Guide-GroundSystem.txt, the Guide-GroundSystem.txt).  Note it depends on PyQt4 and PyZMQ:

1. Install PyQt4 and PyZMQ on your system
2. Compile cmdUtil and start the ground system executable

       cd tools/cFS-GroundSystem/Subsystems/cmdUtil
       make
       cd ../..
       python GroundSystem.py
    
3. Select "Start Command System"
4. Select "Enable Tlm"
5. Enter IP address of system executing cFS, 127.0.0.1 if running locally

Should see telemetry, can send noops and see command counters increment

## Compatible list of cFS apps

The following applications have been tested against this release:
  - TBD

## Other cFS related elements/tools/apps/distributions

The following list is user submitted, and not CCB controlled.  They are released by various orgainizations, under various licenses.

  - Distributions
    - cFS-101: Virtual machine distribution at https://github.com/nasa/CFS-101
    - OpenSatKit: Open source kit for satelite software at https://github.com/OpenSatKit/OpenSatKit
  - Other Ground station software
    - TBD
  - Other Apps
    - CF: CFDP application at https://github.com/nasa/CF
    - HK: Housekeeping application at https://github.com/nasa/HK
    - MD: Memory Dwell application at https://github.com/nasa/MD
    - MM: Memory Manager application at https://github.com/nasa/MM
    - LC: Limit Checker application at https://github.com/nasa/LC
    - CS: Checksum application at https://github.com/nasa/CS
    - FM: File Manager application at https://github.com/nasa/FM
    - SC: Stored Commands application at https://github.com/nasa/SC
    - SCH: Scheduler application at https://github.com/nasa/SCH
    - HS: Health and Safety application at https://github.com/nasa/HS
    - DS: Data Store application at https://github.com/nasa/DS
    - SCA: Stored Command Absolute application at https://github.com/nasa/SCA
  - Other Interfaces
    - SIL: Simulink Interface Layer at https://github.com/nasa/SIL
    - ECI: External Code Interface at https://github.com/nasa/ECI
  - Other Libraries
    - cFS_LIB: at https://github.com/nasa/cfs_lib
    - cFS_IO_LIB: IO library at https://github.com/nasa/CFS_IO_LIB
  - Other Tools
    - CCDD: Command and Data Dictionary Tool at https://github.com/nasa/CCDD
    - Perfutils-java: Java based performance analyzer for cFS at https://github.com/nasa/perfutils-java
    - gen_msgids: Deprecated tool that prints ApIDs
    - gen_sch_tbl: Tool to generated SCH app tables
  - Other OSALs
    - TBD
  - Other PSPs
    - TBD

