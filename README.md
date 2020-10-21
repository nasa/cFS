[![Build Status](https://travis-ci.com/nasa/cFS.svg)](https://travis-ci.com/nasa/cFS)
[![LGTM Alerts](https://img.shields.io/lgtm/alerts/github/nasa/cFS)](https://lgtm.com/projects/g/nasa/cFS/alerts/?mode=list)
[![LGTM Grade](https://img.shields.io/lgtm/grade/python/github/nasa/cFS)](https://lgtm.com/projects/g/nasa/cFS/alerts/?mode=list)
[![LGTM Grade](https://img.shields.io/lgtm/grade/cpp/github/nasa/cFS)](https://lgtm.com/projects/g/nasa/cFS/alerts/?mode=list)

# Core Flight System - BUNDLE

The Core Flight System (cFS) is a generic flight software architecture framework used on flagship spacecraft, human spacecraft, cubesats, and Raspberry Pi.  This repository is a bundle of submodules that make up the cFS framework.  Note the "lab" apps are intended as examples only, and enable this bundle to build, execute, receive commands, and send telemetry.  This is not a flight distribution, which is typically made up of the cFE, OSAL, PSP, and a selection of flight apps that correspond to specific mission requirements.

This bundle has not been fully verified as an operational system, and is provided as a starting point vs an end product.  Testing of this bundle consists of building, executing, sending setup commands and verifying receipt of telemetry.  Unit testing is also run, but extensive analysis is not performed.  All verification and validation per mission requirements is the responsibility of the mission (although attempts are made in the cFS Framework to provide a testing framework to facilitate the process).

The cFS Framework is a core subset of cFS.  There are additional OSALs, PSPs, and tools as listed below available from a variety of sources.

## References Documentation
  - cFE User's Guide: https://github.com/nasa/cFS/blob/gh-pages/cFE_Users_Guide.pdf
  - OSAL User's Guide: https://github.com/nasa/cFS/blob/gh-pages/OSAL_Users_Guide.pdf
  - cFE App Developer's Guide: https://github.com/nasa/cFE/blob/main/docs/cFE%20Application%20Developers%20Guide.md
  - Training documentation: https://ntrs.nasa.gov/citations/20205000691
  - cFS Overview: https://cfs.gsfc.nasa.gov/cFS-OviewBGSlideDeck-ExportControl-Final.pdf

## Release Notes

See [releases](https://github.com/nasa/cFS/releases) for release history and associated artifacts related to the cFS BUNDLE.

**Aquila: OFFICIAL RELEASE**:
  - Released under Apache 2.0
  - Includes cFE 6.7.0 (cFE, PSP, framework apps, and framework tools as marked) and OSAL 5.0.0

**cFS 6.6.0a Suite: OFFICIAL RELEASE**:
  - cFE 6.6.0a is released under Apache 2.0 license, see [LICENSE](https://github.com/nasa/cFE/blob/v6.6.0a/LICENSE-18128-Apache-2_0.pdf)
  - OSAL 4.2.1a is released under the NOSA license, see [LICENSE](https://github.com/nasa/osal/blob/osal-4.2.1a/LICENSE)
  - [Release notes](https://github.com/nasa/cFE/blob/v6.6.0a/docs/cFE_release_notes.md)
  - [Version description document](https://github.com/nasa/cFE/blob/v6.6.0a/docs/cFE_6_6_0_version_description.md)
  - [Test results](https://github.com/nasa/cFE/tree/v6.6.0a/test-and-ground/test-review-packages/Results)

Other elements listed below are released under a variety of licenses as detailed in their respective repositories.

## Known issues

Historical version description documents contain references to internal repositories and sourceforge, which is no longer in use.  Not all markdown documents have been updated for GitHub.

See related repositories for current open issues.

## Major future work

### Caelum (aka 7.0) Major release development plans (Targeting end of 2020 for release candidate)

  - Certification framework with automated build verification tests of framework requirements
    - Executable on real/emulated/simulated/ or dockerized targets
    - Add PSP coverage testing framework (nasa/psp#184, nasa/psp#174)
    - Add PSP and cFE functional testing framework for APIs  (nasa/cfe#779)
    - Scrub OSAL coverage and functional tests
    - Scrub cFE coverage tests
    - Add cFE API functional tests
    - NOTE: Command verification pending tool open source release
  - Documentation (updated traceability, APIs/ICDs, general update)
  - Framework for mission customization of core services
    - Header customization support (nasa/cFE#726)
  - Remove deprecated code
  - Cmd/Tlm structure scrub for alignment/padding/consistency
  - Library query and reporting and ES resource management (nasa/cFE#28, nasa/cFE#797)

### Other (may not make 7.0)
  - Open source automated build verification execution framework for emulated targets (likely docker based)
  - Deployment quality of life improvements (configuration, transition to CMake source selection vs compiler directives)
  - Update OS support (VxWorks 7, RTEMS 5)
  - Time services refactor
  - Symmetric multi-processing APIs
  - Electronic Data Sheet integration option and improvements to packet layouts for portability/consistency
  - Toolchain updates

## Getting Help

For best results, submit issues:questions or issues:help wanted requests to this repo.

Official cFS page: http://cfs.gsfc.nasa.gov

Community email list subscription request: https://lists.nasa.gov/mailman/listinfo/cfs-community

## Setup

Ensure the following software are installed: Make, CMake, GCC, and Git.  To setup the cFS BUNDLE directly from the latest set of interoperable repositories:

    git clone https://github.com/nasa/cFS.git
    cd cFS
    git submodule init
    git submodule update

Copy in the default makefile and definitions:

    cp cfe/cmake/Makefile.sample Makefile
    cp -r cfe/cmake/sample_defs sample_defs

## Build and Run

The cFS Framework including sample applications will build and run on the pc-linux platform support package (should run on most Linux distributions), via the steps described in https://github.com/nasa/cFE/tree/master/cmake/README.md.  Quick-start is below:

To prep, compile, and run on the host (from cFS directory above) as a normal user (best effort message queue depth and task priorities):

    make SIMULATION=native prep
    make
    make install
    cd build/exe/cpu1/
    ./core-cpu1

Should see startup messages, and CFE_ES_Main entering OPERATIONAL state.  Note the code must be executed from the build/exe/cpu1 directory to find the startup script and shared objects.

Note: The steps above are for a debug, permissive mode build and includes deprecated elements.  For a release build, recommendation is `make BUILDTYPE=release OMIT_DEPRECATED=true prep`.  Unit tests can be added with `ENABLE_UNIT_TESTS=true`, run with `make test`, and coverage reported with `make lcov`.

## Send commands, receive telemetry

The cFS-GroundSystem tool can be used to send commands and receive telemetry.  For details on using and setting up the Ground System, see the [Guide-GroundSystem](https://github.com/nasa/cFS-GroundSystem/blob/main/Guide-GroundSystem.md).  Note it depends on PyQt5 and PyZMQ:

1. Install PyQt5 and PyZMQ on your system.  Some systems may also require installing libcanberra-gtk-module.
       
2. Compile cmdUtil and start the ground system executable

       cd tools/cFS-GroundSystem/Subsystems/cmdUtil
       make
       cd ../..
       python3 GroundSystem.py

3. Select "Start Command System"
4. Select "Enable Tlm"
5. Enter IP address of system executing cFS, 127.0.0.1 if running locally

Should see telemetry, can send noops and see command counters increment.

## Compatible list of cFS apps

The following applications have been tested against this release:
  - TBD

## Other cFS related elements/tools/apps/distributions

The following list is user submitted, and not CCB controlled.  They are released by various organizations, under various licenses.

  - Distributions
    - cFS-101: Virtual machine distribution at https://github.com/nasa/CFS-101
    - OpenSatKit: Open source kit for satellite software at https://github.com/OpenSatKit/OpenSatKit
  - Other Ground station software
    - TBD
  - Other Apps
    - CS: Checksum application at https://github.com/nasa/CS
    - CF: CFDP application at https://github.com/nasa/CF
    - CI: Command Ingest application at https://github.com/nasa/CFS_CI
    - DS: Data Store application at https://github.com/nasa/DS
    - FM: File Manager application at https://github.com/nasa/FM
    - HK: Housekeeping application at https://github.com/nasa/HK
    - HS: Health and Safety application at https://github.com/nasa/HS
    - LC: Limit Checker application at https://github.com/nasa/LC
    - MD: Memory Dwell application at https://github.com/nasa/MD
    - MM: Memory Manager application at https://github.com/nasa/MM
    - SC: Stored Commands application at https://github.com/nasa/SC
    - SCA: Stored Command Absolute application at https://github.com/nasa/SCA
    - SCH: Scheduler application at https://github.com/nasa/SCH
    - TO: Telemetry Output application at https://github.com/nasa/CFS_TO
    - Skeleton App: A bare-bones application to which you can add your business logic at https://github.com/nasa/skeleton_app 
  - Other Interfaces
    - SIL: Simulink Interface Layer at https://github.com/nasa/SIL
    - ECI: External Code Interface at https://github.com/nasa/ECI
  - Other Libraries
    - cFS_IO_LIB: IO library at https://github.com/nasa/CFS_IO_LIB
    - cFS_LIB: at https://github.com/nasa/cfs_lib
  - Other Tools
    - CCDD: Command and Data Dictionary Tool at https://github.com/nasa/CCDD
    - Perfutils-java: Java based performance analyzer for cFS at https://github.com/nasa/perfutils-java
    - gen_sch_tbl: Tool to generated SCH app tables
  - Other OSALs
    - TBD
  - Other PSPs
    - TBD
  
