# McMaster NEUDOSE - Core Flight System 

This repository is the most up-to-date version of the flight software that is being used on the McMaster NEUDOSE mission. It is forked from NASA's Core Flight System (cFS) github and will likely commit many of the changes NASA engineers contribute to that repo. 


## Release Notes
cFS Version PB - Preliminary Base
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
