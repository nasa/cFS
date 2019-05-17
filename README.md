# Core Flight System - BUNDLE

This repository is a bundle of submodules that make up the Core Flight System (cFS) framework.  Note the "lab" apps are intended as examples only, and enable this bundle to build, execute, receive commands, and send telemetry.  This is not a flight distribution, which is typically made up of the cFE, OSAL, PSP, and a selection of flight apps that coorespond to specific mission requirements.

This bundle has not been fully verified as an operational system, and is provided as a starting point vs an end product.  Testing of this bundle consists of building, executing, sending setup commands and verifying receipt of telemetry.  Unit testing is also run, but extensive analysis is not performed.  All verification and validation per mission requirements is the responsibility of the mission (although attempts are made in the cFS Framework to provide a testing framework to facilitate the process).

The cFS Framework is just a core subset of cFS, there are additional OSALs, PSPs, tools as listed below provided from a variety of sources.

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
  - elf2cfetbl doesn't build 64bit tables, there is a patch (contact forum below)
  - TIME services can lock up in rare cases/contexts, there is a patch (contact forum below)

## Major future work

  - Certification framework including PSP and OSAL white box testing
  - Open source automated build framework
  - Unit test modernization/standardization
  - Deployment quality of life improvements (configuration)
  - Documentation (updated tracability, APIs/ICDs, general update)
  - Symmetric multi-processing APIs
  - Electronic Data Sheet integration
  - Toolchain updates
  - Next Gen OSAL

## Getting Help

The cFS community page http://coreflightsystem.org should be your first stop for getting help. Please post questions to the http://coreflightsystem.org/questions/. There is also a forum at http://coreflightsystem.org/forums for more general discussions.

Official cFS page: http://cfs.gsfc.nasa.gov

## Setup

We suggest that users fork the repositories included here and modify as needed for their specific missions.

To setup cFS directly from the latest release:

    git clone https://github.com/nasa/cFS.git
    cd cFS
    git submodule init
    git submodule update

If running on a standard linux build, likely need to increase message queues.  Recommendation is to set /proc/sys/fs/mqueue/msg_max to 256 or greater.

## Build and Run

The cFS Framework including sample applications will build and run on the pc-linux platform support package (should run on most Linux distributions), via the steps described in https://github.com/nasa/cFE/tree/master/cmake/README.md.  Quick-start is below:

To compile and run (from cFS directory above):

    cp cfe/cmake/Makefile.sample Makefile
    cp -r cfe/cmake/sample_defs sample_defs
    make prep
    make
    make install
    ./build/exe/cpu1/core-cpu1
    (should see startup messages, and CFE_ES_Main entering OPERATIONAL state)

The cFS-GroundSystem tool can be used to send commands and receive telemetry (see https://TBD, the Guide-GroundSystem.txt).  Note it depends on PyQt4 and PyZMQ:

    (install PyQt4 and PyZMQ on your system)
    cd tools/cFS-GroundSystem/Subsystems/cmdUtil
    make
    cd ../..
    python GroundSystem.py
    (select "Start Command System")
    (select "Enable Tlm")
    (enter IP address to connect to, 127.0.0.1 if running locally)
    (should see telemetry, can send noops and see command counters increment)

## Compatible list of cFS apps tested against current release

## Other cFS related elements/tools/apps (not tested), user submitted catalog

  - TBD Ground station 
  - TBD Other Apps 
  - TBD Tools
  - TBD OSALs
  - TBD PSPs
