# McMaster NEUDOSE - Core Flight System 

This repository is the most up-to-date version of the flight software that is being used on the McMaster NEUDOSE mission. It is forked from NASA's Core Flight System (cFS) github and will likely commit many of the changes NASA engineers contribute to that repo. 


## Release Notes
**cFS Version PB - Preliminary Base**

This version includes the CFE, PSP, OSAL submodules from NASA, as well as all sample apps included in the cFS Bundle. It also includes cFS apps that the McMaster NEUDOSE team will use: the Housekeeping (HK) and Scheduler (SCH) apps. 


## Setup
If you're on Windows, we'll be running a Virtual Machine to run cFS. I like using Virtual Box (https://www.virtualbox.org). Go ahead and download whatever VM software you like. 

We'll be using Ubuntu 18.04.4 for our operating system on our virtual machine due to it's long-term support. Ubuntu can be downloaded here: https://releases.ubuntu.com/18.04/

In your chosen VM software, create a new virtual machine and install the Ubuntu you just downloaded. I would dynamically allocate around 15gb if you have the space since the operating system is a little large. This step will take a while as Ubuntu first loads. If you have any troubles with this step, ask Paula Bosca.

Make sure you have "make", download it with 

    sudo apt-get install make

Making sure you have "cmake", download it with 
    
    sudo apt-get install cmake
Also git
    
    sudo apt-get install git


To setup the McMaster cFS release directly from the latest set of interoperable repositories:

    git clone https://github.com/McMasterNEUDOSE/cFS.git
    cd cFS
    git submodule init
    git submodule update

Copy in the default makefile and definitions:

    cp cfe/cmake/Makefile.sample Makefile
    cp -r cfe/cmake/sample_defs sample_defs

## Build and Run

The cFS Framework including sample applications will build and run on the pc-linux platform support package (should run on most Linux distributions), via the steps described in https://github.com/nasa/cFE/tree/master/cmake/README.md.  Quick-start is below:

To prep, compile, and run on the host (from cFS directory above) as a normal user (best effort message queue depth and task priorities):

    make BUILDTYPE=release OMIT_DEPRECATED=true prep
    make
    make install
    cd build/exe/cpu1/
    sudo ./core-cpu1

Should see startup messages, and CFE_ES_Main entering OPERATIONAL state.  Note the code must be executed from the build/exe/cpu1 directory to find the startup script and shared objects.

!Note the sudo at beginning of the last command! Try it first without sudo privileges. You might received errors about setting certain scheduling parameters, this is because some of the cFS stuff is setting basic OS parameters and the vm/windows/ubuntu might not like that. If you get the error, include the sudo in the command and it should work. 

Now you should have cFS running! You'll see a bunch of app initialization!

To exit the program at any time, just use ctrl-C

Note! Unit tests can be added with `ENABLE_UNIT_TESTS=true`, run with `make test`, and coverage reported with `make lcov`.

## Running cFS Ground Station (Send commands, receive telemetry)

The ground station GUI has dependencies on a bunch of different packages, so we need to make sure the setup has all of them. 
Run the following commands to download the necessary packages (python3, PyQt5, Zmq)

    sudo apt-get install python3
    sudo apt-get install python3-pyqt5
    sudo apt-get install python3-zmq
    sudo apt-get install libcanberra-gtk-module

If you get an error that certain packages couldn't install, try running 
    
    sudo apt-get update

then re-running the command that broke 

Now, to make the actual system, run
  
    ( cd Subsystems/cmdUtil/ && make )

And to run the GUI

    python3 GroundSystem.py

Now you should see the Ground System GUI! 

Try sending telemetry commands. If you get an error about a file not found in cmdUtil in the terminal window or you just don't see the packet received numbers going up, try the following commands

    cd Subsystems/cmdUtil
    make
    cd ../..
    python3 GroundSystem.py

Now, everything should work smoothly! With that, you should have cFS setup and the Ground System GUI up and running! 


## Enabling Telemetry 

1. Select "Start Command System"
2. Select "Enable Tlm"
3. Enter IP address of system executing cFS, 127.0.0.1 if running locally

Should see telemetry, can send noops and see command counters increment

 
 
***
***
 
From the NASA repo (kept for merging and upstream change monitoring) 

# Core Flight System - BUNDLE

This repository is a bundle of submodules that make up the Core Flight System (cFS) framework.  Note the "lab" apps are intended as examples only, and enable this bundle to build, execute, receive commands, and send telemetry.  This is not a flight distribution, which is typically made up of the cFE, OSAL, PSP, and a selection of flight apps that coorespond to specific mission requirements.

This bundle has not been fully verified as an operational system, and is provided as a starting point vs an end product.  Testing of this bundle consists of building, executing, sending setup commands and verifying receipt of telemetry.  Unit testing is also run, but extensive analysis is not performed.  All verification and validation per mission requirements is the responsibility of the mission (although attempts are made in the cFS Framework to provide a testing framework to facilitate the process).

The cFS Framework is a core subset of cFS.  There are additional OSALs, PSPs, and tools as listed below available from a variety of sources.

## Release Notes

cFS 6.7.0 Suite: NOT FULLY RELEASED
  - cFE 6.7.0 is released under the Apache 2.0 license, see [LICENSE](https://github.com/nasa/cFE/blob/v6.7.0/LICENSE).
    - The license covers cFE, PSP, framework apps, and framework tools as marked
  - OSAL 5.0.0 is pending release
  - Release documentation pending

**cFS 6.6.0a Suite: OFFICIAL RELEASE**:
  - cFE 6.6.0a is released under Apache 2.0 license, see [LICENSE](https://github.com/nasa/cFE/blob/v6.6.0a/LICENSE-18128-Apache-2_0.pdf)
  - OSAL 4.2.1a is released under the NOSA license, see [LICENSE](https://github.com/nasa/osal/blob/osal-4.2.1a/LICENSE)
  - [Release notes](https://github.com/nasa/cFE/blob/v6.6.0a/docs/cFE_release_notes.md)
  - [Version description document](https://github.com/nasa/cFE/blob/v6.6.0a/docs/cFE_6_6_0_version_description.md)
  - [Test results](https://github.com/nasa/cFE/tree/v6.6.0a/test-and-ground/test-review-packages/Results)

Other elements listed below are released under a varitey of licenses as detailed in their respective repositories.

## Known issues

Historical version description documents contain references to internal repositories and sourceforge, which is no longer in use.  Not all markdown documents have been updated for GitHub.

See related repositories for current open issues.

## Major future work

  - Certification framework with automated build verification tests of framework requirements
    - Executable on real/emulated/simulated/ or dockerized targets
    - Add PSP coverage testing framework
    - Add PSP and cFE functional testing framework for APIs
    - Scrub OSAL coverage and functional tests 
  - Open source automated build verification execution framework for emulated targets (likely docker based)
  - Provide capability for mission customization of core services
  - Deployment quality of life improvements (configuration, transition to CMake source selection vs compiler directives)
  - Update OS support (VxWorks 7, RTEMS 5)
  - Time services refactor
  - Documentation (updated tracability, APIs/ICDs, general update)
  - Symmetric multi-processing APIs
  - Electronic Data Sheet integration option and improvements to packet layouts for portability/consistancy
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

The cFS-GroundSystem tool can be used to send commands and receive telemetry (see https://github.com/nasa/cFS-GroundSystem/tree/master/Guide-GroundSystem.txt, the Guide-GroundSystem.txt).  Note it depends on PyQt4 and PyZMQ:

1. Install PyQt4 and PyZMQ on your system
2. Compile cmdUtil and start the ground system executable

       cd tools/cFS-GroundSystem/Subsystems/cmdUtil
       make
       cd ../..
       python3 GroundSystem.py

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
