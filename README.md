[![Build Linux](https://github.com/nasa/cfs/actions/workflows/build-cfs.yml/badge.svg)](https://github.com/nasa/cfs/actions/workflows/build-cfs.yml)
[![Build RTEMS 5](https://github.com/nasa/cFS/actions/workflows/build-cfs-rtems5.yml/badge.svg)](https://github.com/nasa/cFS/actions/workflows/build-cfs-rtems5.yml)

# Core Flight System - BUNDLE

The Core Flight System (cFS) is a generic flight software architecture framework used on flagship spacecraft, human spacecraft, cubesats, and Raspberry Pi.  This repository is a bundle of submodules that make up the cFS framework.  Note the "lab" apps are intended as examples only, and enable this bundle to build, execute, receive commands, and send telemetry.  This is not a flight distribution, which is typically made up of the cFE, OSAL, PSP, and a selection of flight apps that correspond to specific mission requirements.

This bundle has not been fully verified as an operational system, and is provided as a starting point vs an end product.  Testing of this bundle consists of building, executing, sending setup commands and verifying receipt of telemetry.  Unit testing is also run, but extensive analysis is not performed.  All verification and validation per mission requirements is the responsibility of the mission (although attempts are made in the cFS Framework to provide a testing framework to facilitate the process).

The cFS Framework is a core subset of cFS, with an extensive ecosystem of applications and tools available throughout the community.

## Distributions

This is the open-source version of cFS, released under an Apache 2.0 license. The open source cFS is limited to the framework and common apps, libraries, and tools, which includes and is limited to: cFE, OSAL, PSP, Command Ingest (Lab), Telemetry Output (Lab), Scheduler (Lab), Sample App, Sample Lib, Data Storage, File Manager, HouseKeeping, Health and Safety, Memory Dwell, CFDP File Transfer, CheckSum, Limit Checker, Memory Manager, Stored Command, cFS Ground System, elf2cfetbl, and tblCRCTool. Changes to the open repositories are limited to bug fixes and minor enhancements to those components.

A Government-use (Distro C) version of cFS with features for a full flight mission is available through a Software User Agreement. For more information about government version features or to explore partnerships, please [contact the cFS team](<mailto:cfs-program@lists.nasa.gov>).

## Contact

cFS/cFE is owned and maintained by NASA Goddard Space Flight Center's Flight Software Systems Branch.

The cFS team provides training, support, and subject matter expertise to cFS users across industry and the government. New features/developments can also be done by the cFS team which has significant experience in helping flight projects implement mission-ready flight SW using cFS for various classes of missions, including human-rated (class A) missions. A support agreement may be needed.

To connect with the cFS team, please send an email to [cfs-program@lists.nasa.gov](<mailto:cfs-program@lists.nasa.gov>).

More information is available on the [cFS Website](<https://cfs.gsfc.nasa.gov>).

## Table of Contents

- [References](#references-documentation)
- [Release Notes](#release-notes)
- [Open Source Roadmap](#open-source-roadmap)
- [Upcoming Release](#upcoming-release)
- [Getting Help](#getting-help)
- [Setup](#setup)
- [Build and Run](#build-and-run)
- [Related Capability](#related-capability)

## References Documentation

- [cFE User's Guide](<https://github.com/nasa/cFS/blob/gh-pages/cfe-usersguide.pdf>)
- [OSAL User's Guide](<https://github.com/nasa/cFS/blob/gh-pages/osal-apiguide.pdf>)
- [Combined Mission documentation](<https://github.com/nasa/cFS/blob/gh-pages/mission-doc.pdf>)
- [cFE App Developer's Guide](<https://github.com/nasa/cFE/blob/main/docs/cFE%20Application%20Developers%20Guide.md>)
- [Training documentation](<https://ntrs.nasa.gov/citations/20240000217>)
- [cFS Overview](<https://cfs.gsfc.nasa.gov/cFS-OviewBGSlideDeck-ExportControl-Final.pdf>)

## Release Notes

See [releases](<https://github.com/nasa/cFS/releases>) for release history and associated artifacts related to the cFS BUNDLE.

**Aquila: OFFICIAL RELEASE**:

- Released under Apache 2.0
- Includes cFE 6.7.0 (cFE, PSP, framework apps, and framework tools as marked) and OSAL 5.0.0
- [Version description document, including release notes](<https://github.com/nasa/cFS/blob/v6.7.0a/VDD-Aquila.md>)
- [Release artifacts](<https://github.com/nasa/cFS/releases/tag/v6.7.0a>)

Other elements listed in [Related Capability](#related-capability) are released under a variety of licenses as detailed in their respective repositories.

## Open-Source Roadmap

The following is a list of features and updates planned for cFS (in no particular order). Have other ideas? Want to partner with us? Please let us know at [cfs-program@lists.nasa.gov](<mailto:cfs-program@lists.nasa.gov>).

- "Bundler" tool to create custom bundles from known capability developed across the community (essentially an interactive catalog of apps that have been verified against a particular version of cFS)
- VxWorks 7 support, including running applications as real-time processes (RTPs)
- RTEMS 6 support
- Automated build verification execution framework for emulated targets
- Time services refactor/simplication
- Symmetric multi-processing (SMP) APIs
- Electronic Data Sheet (EDS) integration option and improvements to packet layouts for portability/consistency

## Upcoming Release

Note: Official releases will be created through the NASA software release process. The cFS team plans to return to a more regular official release cadence in the near future.

### Next major release development features

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
- Type safety enhancements
- Added abstract configuration registry
- RTEMS 5/6 support updates
- Added support for fractional seconds in epoch
- Bug fixes
  
### Known Issues

Historical version description documents contain references to internal repositories and sourceforge, which is no longer in use.  Not all markdown documents have been updated for GitHub.

See related repositories for current open issues.

## Getting Help

### Discussions

You can [start a new discussion](<https://github.com/nasa/cFS/discussions/new>) for discussions, questions, or ideas, in the cFS repository under the Discussions tab.

### Join the mailing list

To **subscribe to our mailing list,** send an email to [cfs-community-join@lists.nasa.gov](<mailto:cfs-community-join@lists.nasa.gov>) with the word *subscribe* in the subject line.

The cfs-community mailing list includes cFS users and developers. The cFS Product Team also uses the mailing list to share information on current and future releases, bug findings and fixes, enhancement requests, community meetings, etc.

If you'd like to unsubscribe, send an email with the word *unsubscribe* to [cfs-community-leave@lists.nasa.gov](<mailto:cfs-community-leave@lists.nasa.gov>). Mailing list requests are typically processed within 5 minutes.

### Contact the cFS Product Team

You can email the cFS Product Team at [cfs-program@lists.nasa.gov](<mailto:cfs-program@lists.nasa.gov>) to explore partnerships and other arrangements for in-depth support.

## Setup

Ensure the following software are installed: Make, CMake, GCC, and Git.  To setup the cFS BUNDLE directly from the latest set of interoperable repositories (with your working directory set to where you want to put cFS):

    git clone https://github.com/nasa/cFS.git
    cd cFS
    git submodule init
    git submodule update

Copy in the default makefile and definitions:

    cp cfe/cmake/Makefile.sample Makefile
    cp -r cfe/cmake/sample_defs sample_defs

## Build and Run

The cFS Framework including sample applications will build and run on the pc-linux platform support package (should run on most Linux distributions), via the steps described in [the cFE cmake readme](<https://github.com/nasa/cFE/tree/master/cmake/README.md>).  Quick-start is below:

To prep, compile, and run on the host (from cFS directory above) as a normal user (best effort message queue depth and task priorities):
    make distclean
        (For a clean build on subsequent runs)
    make SIMULATION=native prep
    make
    make install
    cd build/exe/cpu1/
    ./core-cpu1

Should see startup messages, and CFE_ES_Main entering OPERATIONAL state.  Note the code must be executed from the build/exe/cpu1 directory to find the startup script and shared objects.

Note: The steps above are for a debug, permissive mode build and includes deprecated elements.  For a release build, recommendation is `make BUILDTYPE=release OMIT_DEPRECATED=true prep`.  Unit tests can be added with `ENABLE_UNIT_TESTS=true` during the prep step, run with `make test`, and coverage reported with `make lcov`. Functional tests can be run by including `ENABLE_UNIT_TESTS=true` during prep and including the cfe_testcase app in the runtime app configuration (.scr file).

### Send commands, receive telemetry

The cFS-GroundSystem tool can be used to send commands and receive telemetry.  For details on using and setting up the Ground System, see the [Guide-GroundSystem](https://github.com/nasa/cFS-GroundSystem/blob/main/Guide-GroundSystem.md).  Note it depends on PyQt5 and PyZMQ:

1. Install PyQt5 and PyZMQ on your system.  Some systems may also require installing libcanberra-gtk-module.

2. Compile cmdUtil and start the ground system executable

       cd tools/cFS-GroundSystem/Subsystems/cmdUtil
       make
       cd ../..
       python3 GroundSystem.py

3. Select "Start Command System" from main window
4. Select "Enable Tlm"
5. Enter IP address of system executing cFS, 127.0.0.1 if running locally
6. Select "Start Telemetry System" from main window

Should see telemetry, can send noops and see command counters increment.

### Adding Custom Apps

See the [cFE Application Developer's Guide](https://github.com/nasa/cFE/blob/main/docs/cFE%20Application%20Developers%20Guide.md) for information about creating new apps. It is encouraged to use sample_app or skeleton_app as a starting point. Once the application has been created, it must be added to targets.cmake to be compiled and to the .scr file to be dynamically loaded at runtime (in a dynamically loaded environment).

## Related Capability

- Distributions
  - cFS-101: Virtual machine distribution at <https://github.com/nasa/CFS-101>
- Other Ground station software
  - cFS-EDS-GroundStation: Ground station implemented via EDS at <https://github.com/nasa/cFS-EDS-GroundStation>
- Other Apps
  - CS: Checksum application at <https://github.com/nasa/CS>
  - CF: CFDP application at <https://github.com/nasa/CF>
  - DS: Data Store application at <https://github.com/nasa/DS>
  - FM: File Manager application at <https://github.com/nasa/FM>
  - HK: Housekeeping application at <https://github.com/nasa/HK>
  - HS: Health and Safety application at <https://github.com/nasa/HS>
  - LC: Limit Checker application at <https://github.com/nasa/LC>
  - MD: Memory Dwell application at <https://github.com/nasa/MD>
  - MM: Memory Manager application at <https://github.com/nasa/MM>
  - SBN: Software Bus Network application at <https://github.com/nasa/SBN>
  - SC: Stored Commands application at <https://github.com/nasa/SC>
  - SCA: Stored Command Absolute application at <https://github.com/nasa/SCA>
  - Skeleton App: A bare-bones application to which you can add your business logic at <https://github.com/nasa/skeleton_app>
- Other Interfaces
  - SIL: Simulink Interface Layer at <https://github.com/nasa/SIL>
  - ECI: External Code Interface at <https://github.com/nasa/ECI>
  - SBN-Client: External code interface to SBN at <https://github.com/nasa/SBN-Client>
- Other Libraries
  - cFS_IO_LIB: IO library at <https://github.com/nasa/CFS_IO_LIB>
  - cFS_LIB: at <https://github.com/nasa/cfs_lib>
  - EdsLib: CCSDS SOIS Electronic Data Sheet Tool and Library at <https://github.com/nasa/EdsLib>
  - fs_lib: File services library at <https://github.com/nasa/fs_lib>
- Other Tools
  - CTF: cFS Test Framework at <https://github.com/nasa/CTF>
  - CCDD: Command and Data Dictionary Tool at <https://github.com/nasa/CCDD>
  - Perfutils-java: Java based performance analyzer for cFS at <https://github.com/nasa/perfutils-java>
  - gen_sch_tbl: Tool to generated SCH app tables at <https://github.com/nasa/gen_sch_tbl>
  - CryptoLib: Software-only CCSDS Space Data Link Security Protocol - Extended Procedures (SDLS-EP) at <https://github.com/nasa/CryptoLib>
