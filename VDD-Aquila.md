# Version Description Document #
#### core Flight System Framework ####
#### BUILD: 6.7.0 ####
#### RELEASE DATE: 6/29/2020 ####
#### RELEASED BY: Goddard Space Flight Center ####

## 1.0 FSW VERSION DESCRIPTION ##
### 1.1 PURPOSE AND SUMMARY ###
This is a release of the core Flight System (cFS) Framework elements as follows:
- **core Flight Executive: [cFE](https://github.com/nasa/cFE)** - Version 6.7.0
- **Operating System Abstraction Layer: [OSAL](https://github.com/nasa/osal)** - Version 5.0.0
  - posix
  - rtems (supports 4.11)
  - vxworks (supports 6.9)
- **Platform Support Package: [PSP](https://github.com/nasa/PSP)** - Version 1.4.0 
  - pc-linux
  - pc-rtems
  - mcp750-vxworks
- Lab Tools
  - **Sample Ground System: [cFS-GroundSystem](https://github.com/nasa/cFS-GroundSystem)** - Version 2.1.0
  - **ELF to cFE Table Converter: [elf2cfetbl](https://github.com/nasa/elf2cfetbl)** - Version 3.1.0
  - **Table Cyclic Redundancy Check Tool: [tblCRCTool](https://github.com/nasa/tblCRCTool)** - Version 1.1.0
- Lab Applications
  - **Command Ingest: [ci_lab](https://github.com/nasa/ci_lab)** - Version 2.3.0
  - **Sample Application: [sample_app](https://github.com/nasa/sample_app)** - Version 1.1.0
  - **Sample Library: [sample_lib](https://github.com/nasa/sample_lib)** - Version 1.1.0
  - **Schedular: [sch_lab](https://github.com/nasa/sch_lab)** - Version 2.3.0
  - **Telemetry Output: [to_lab](https://github.com/nasa/to_lab)** - Version 2.3.0

Backwards or cross compatibility is not supported between the cFE, OSAL, and PSP for this release.
Recommendation is to update all or none.
Application verification against this version are documented on the https://github.com/nasa/cFS/.
Lab Tools and Lab Applications are also dependent on the latest releases and not generally
backwards compatible.  The released bundle associates the versions that have been tested together.

There are no requirements changes or major behavioral changes in this release.

#### Summary of FSW changes ####
- Refactor OSAL to reduce code duplication
- Added OSAL file system abstraction APIs, marked old wrappers as deprecated
- Added OSAL socket and select APIs
- Support asynchronous console output
- Deconflicted performance IDs in lab applications
- Fixed various typos in variables, comments, prints
- Removed CVS flags
- Replaced deprecated references
- Resolved various build and static analysis warnings
- Improved 64 bit support in tools (now default configuration)
- Updated to VxWorks 6.9 support
- Updated to RTEMS 4.11 support
- Startup now fails if OS API initialization fails
- Update PSP timebase support
- Removed CFE_PSP_SUBMINOR_VERSION define
- Minor OS API type fixes (const where needed, signed vs unsigned, etc)
- Minor bug fixes
  - PSP
    - Resolved race condition in PSP timer callbacks
    - Fixed MCP750 FPU exception behavior
    - String manipulation fixes
    - Removed OS_printf calls prior to OS_API_Init
  - OSAL
    - Initialize structure passed into mq_open
    - Fix Lookup on nonexistent symbol in RTEMS
    - Avoid continuous loop in timebase thread
  - cFE
    - String manipulation fixes
    - Eliminated potential lockup from CFE_TIME_GetReference call from interrupt context
    - Fix Unknown State Error After App Exit

#### Document and Other Non-FSW Updates ####
- Removed non-ascii characters in text files
- Improvements to version control ignore lists
- Scripts set as executable
- Removed unused files/directories
- Removed classic build support in favor of cmake (and updated support where needed)
- Improve cmake support for using c++ flags
- Updated directory names
- Added unit test stub functions where needed
- Update unit test framework
- Removed proprietary ground system related files
- Added naming convention document

#### Marked for Deprecation (will be removed in future release) ####
Define OSAL_OMIT_DEPRECATED and CFE_OMIT_DEPRECATED_6_6 to check for compliance.
In general, replacements are detailed in associated API headers or are no longer needed by underlying software.

#### Unit Testing On Host (Ubuntu 18.04.4 LTS) ####
At the cFS level, the following tests can be run on the host:
- OSAL tests (osal/src/tests)
  - count-sem-test, sem-speed-test, mutex-test, bin-sem-timeout-test, queue-timeout-test, bin-sem-flush-test, 
  osal-core-test, file-api-test, bine-sem-test, symbol-api-test, timer-test
- OSAL unit tests (osal/src/unit-tests)
  - osal_core_UT, osal_loader_UT, osal_filesys_UT, osal_file_UT, osal_network_UT, osal_timer_UT
- cFE unit tests (cfe/fsw/cfe-core/unit-tests)
  - es_UT, sb_UT, evs_UT, tbl_UT, time_UT, fs_UT

Procedure:
- Start with standard setup detailed on cFS README (set PERMISSIVE mode if needed)
- (Re)do prep, build, install, test, and report coverage with unit tests enabled
  ```
  make distclean
  make ENABLE_UNIT_TESTS=TRUE SIMULATION=native prep
  make
  make install
  make test
  make lcov
  ```
- Note the osal_timer_UT test frequently fails on Linux, re-execute should eventually pass (fixed for next OSAL
  release nasa/OSAL#336)

Current cFE coverage rate (includes unit test code and stubs, and not a true coverage against baseline of all files):
- lines......: 92.7% (20388 of 21997 lines)
- functions..: 95.9% (827 of 862 functions)

See the console and results files attached to the release (ENABLE_UNIT_TESTS_*).

At the OSAL level, the following tests can be run on the host:
- OSAL coverage tests for the shared layer (osal/src/unit-test-coverage/shared):
  - shared-binsem, shared-clock, shared-common, shared-countsem, shared-dir, shared-errors, shared-file, 
  shared-filesys, shared-fpu, shared-heap, shared-idmap, shared-interrupts, shared-module, shared-mutex, 
  shared-network, shared-printf, shared-queue, shared-select, shared-sockets, shared-task, shared-timebase, 
  shared-time
- Note other OSALS coverage tests (posix-ng/vxworks6/vxworks-ng) were not executed on this release

Procedure:
  - From the base cFS directory:
  ```
  /bin/rm -rf build_osal_cov
  mkdir build_osal_cov
  cd build_osal_cov
  cmake -DCMAKE_BUILD_TYPE=debug -DOSALCOVERAGE_TARGET_OSTYPE='shared' -DOSAL_INCLUDEDIR=../bsp/pc-linux/config/ ../osal/src/unit-test-coverage/
  make
  lcov --capture --initial --directory ./ --output-file coverage_base.info
  make test
  lcov --capture --directory ./ --output-file coverage_test.info
  lcov --add-tracefile coverage_base.info --add-tracefile coverage_test.info --output-file coverage_total.info
  lcov --remove coverage_total.info 'unit-test-coverage/*' --output-file coverage_filtered.info
  genhtml coverage_filtered.info --output-directory lcov
  ```
Current OSAL coverage rate of shared layer (based on including the c code in ut wrappers):
- lines......: 94.6% (1822 of 1925 lines)
- functions..: 99.5% (198 of 199 functions)

See the console and results files attached to the release (OSAL_COVERAGE_*).

#### Unit Testing On Target (MCP750, VxWorks 6.9) ####
The same set of OSAL unit tests (built with ENABLE_UNIT_TESTS=TRUE) as above were cross compiled for the MCP750, VxWorks 6.9
target and executed following the steps below:
- Brought in toolchain file (from master repo, toolchain-ppc-vxworks6.9.cmake)
- Added `SET(TGT1_SYSTEM ppc-vxworks6.9)` to targets.cmake
  ```
  /opt/WindRiver/wrenv.sh -pvxworks-6.9
  make ENABLE_UNIT_TESTS=TRUE prep
  make
  make install
  ```
- Loaded files to target `ncftpput ... CF:0/apps-rc build/exe/cpu1/*.exe`
- Connected to target `minicom cpu3`
- Execute tests
  ```
  ld < bin-sem-flush-test.exe
  RunTest
  unld("bin-sem-flush-test.exe)
  ```
  ... (and so on for all tests)
NOTE - some tests require a reboot to function correctly (unld doesn't clean up after the test)

All passed except sem-speed-test.exe (exception at interrupt level), and 2 of the timer tests were slightly under tolerance
(2 of 25 failed).  These are fixed for the next OSAL release in nasa/OSAL#336 and nasa/OSAL#275.

See the concole file attached to the release (TARGET_TESTS_console.txt).  Note license headers were
updated after this run, for example output use only.

#### Build Verification Testing ####
The cFE 6.7.0 build verification was performed against the code tag 6.7.0-bv.  It was tested by Walt Moleski
in the cFS Lab using the mcp750 target with VxWorks 6.9.  Testing was completed on 10/8/2019.  Note header 
and document changes may occur after the 6.7.0-bv tag, and are included in the official release.  This verification
does not imply any fitness for use per the associated licencing.

There were 17 Build Verification tests executed and they all passed. Three (3) alternate image configuration
tests were NOT executed since they required a separate compilation of the cFE using different values for 
several configuration parameters as well as introducing changes to the BSP/PSP code. They were the Alternate 
Image test (cfe_AltImage), Object Creation Failure test (cfe_OSObjFailure), and the user-defined exception 
handler test (cfe_myeh).

There were several requirements that were not tested. They are:
- Device Driver requirements are not implemented.
  - cES1324; cES1325; cES1326; cES1326.1; cES1327, cES1508.3
- PSP/BSP and mission configuration dependent requirements:
  - cES1515.1; cES1702.2; cES1702.3; cES1703.2; cES1703.3
  - Note cES1702.2 and cES1703.3 were tested when PSP issue #77 was resolved with custom setup (not part of
  build verification)
- Time client configuration requirements that are not supported by the test hardware:
  - cTIME2012.1; cTIME2013; cTIME2014; cTIME2701; cTIME2702; cTIME2703

#### Other Targets ####
This code was also built for and executed on the following targets:
-	Ubuntu 18.04 x86 64 bit
-	Docker Alpine Linux x86 64 bit ( using OSAL_DEBUG_PERMISSIVE_MODE )
-	Raspberry Pi armv6 32 bit ( equivalent of Debian 9)
-	Raspberry Pi Docker armv6 32 bit Alpine Linux ( using OSAL_DEBUG_PERMISSIVE_MODE )

### 1.2 NEW/CHANGED FSW IN THIS VERSION ###

#### 1.2.1 cFE Closed to Version 6.7.0: https://github.com/nasa/cfe/issues?q=milestone%3A6.7.0 ####
- 6.7.0 Updates managed in Babelfish (no GitHub pull references)
  - Merge and Commit are Babelfish references, Issues are GitHub references (transfered from Babelfish)

Merge | Commit | Issue | Summary | Contributor
-- | -- | -- | -- | --
43717d3 | 1d4b669 | [#353](https://github.com/nasa/cFE/issues/353) | Replace Deprecated Memory API References | jphickey
116fb1b | 8fde72f | [#348](https://github.com/nasa/cFE/issues/348) | Update Enhanced Version Reporting | skliper
116fb1b | d7bc04b | [#349](https://github.com/nasa/cFE/issues/349) | Remove ENHANCED_BUILD Switch | jphickey
b34d281 | a5319c4 | [#345](https://github.com/nasa/cFE/issues/345) | Remove Conditional Backwards Compatibility | jphickey
43717d3 | 04d32e5 | [#350](https://github.com/nasa/cFE/issues/350) | Use C99 Boolean Types | jphickey
43717d3 | bd1ede0 | [#319](https://github.com/nasa/cFE/issues/319) | Update Headers for Copyright/License | skliper
b9373ec | af19b29 | [#347](https://github.com/nasa/cFE/issues/347) | Remove CVS Flags | skliper
b9373ec | dddcc4a | [#269](https://github.com/nasa/cFE/issues/269) | Fix Doxygen Comments | skliper
b9373ec | 5bdf7eb | [#330](https://github.com/nasa/cFE/issues/330) | Remove Duplicate CFE_PLATFORM_ES_APP_KILL_TIMEOUT | skliper
1d9be2b | 961b83f | [#340](https://github.com/nasa/cFE/issues/340) | Remove Duplicate Time State Flag Logic | skliper
1d9be2b | fdb31c4 | [#282](https://github.com/nasa/cFE/issues/282) | Restore Last Load File from CDS | skliper
ceaefb3 | bc1240d | [#329](https://github.com/nasa/cFE/issues/329) | Replace Deprecated References | jphickey
e7b422c | 7136c43 | [#116](https://github.com/nasa/cFE/issues/116) | Cleanup From Coverage Testing | jphickey
a1154fa | 65fa061 | [#89](https://github.com/nasa/cFE/issues/89) | Fix Unknown State Error After App Exit | jphickey
44b9f7b | 72f96eb | [#262](https://github.com/nasa/cFE/issues/262) | Set Address Validity FALSE if Tlm Address Size Too Small | jphickey
44b9f7b | a335c57 | [#310](https://github.com/nasa/cFE/issues/310) | Ring Buffer for Time Reference Data | jphickey
NA | 47014d5 | [#279](https://github.com/nasa/cFE/issues/279) | Simplify app record State Tracking | jphickey
007b4ac | fcfcb29 | [#280](https://github.com/nasa/cFE/issues/280) | Clean ES Static Linkage Support | jphickey
6c713dd | 5c719c4 | [#270](https://github.com/nasa/cFE/issues/270) | Resolve Build Warnings Code Review Updates | jphickey
6c713dd | 4bd7b9b | [#281](https://github.com/nasa/cFE/issues/281) | Fix CFE_SB API types | jphickey
6c713dd | 1ee6bb5 | [#276](https://github.com/nasa/cFE/issues/276) | Fix String Manipulation | jphickey
6c713dd | 768e685 | [#270](https://github.com/nasa/cFE/issues/270) | Resolve Build Warnings | jphickey

#### 1.2.2 OSAL Closed to Version 5.0.0: https://github.com/nasa/osal/issues?q=milestone%3A5.0.0 ####
- 5.0.0 Updates managed in Babelfish (no GitHub pull references)
  - Merge and Commit are Babelfish references, Issues are GitHub references (transfered from Babelfish)

Merge | Commit | Issue | Summary | Contributor
-- | -- | -- | -- | --
63ed8a2 | fd8ce5e | [#231](https://github.com/nasa/osal/issues/231) | VxWorks Refactor Code Review Updates | jphickey
d70e914 | 382d5ce | [#258](https://github.com/nasa/osal/issues/258) | Replace Deprecated References | skliper
63ed8a2 | 634c86e | [#231](https://github.com/nasa/osal/issues/231) | Complete VxWorks Refactor | jphickey
22d33aa | b686fd2 | [#251](https://github.com/nasa/osal/issues/251) | Mark boolean/osalbool Deprecated | jphickey
d70e914 | 8f58348 | [#260](https://github.com/nasa/osal/issues/260) | Remove Unused Variables | jphickey
22d33aa | 75161ec | [#257](https://github.com/nasa/osal/issues/257) | Update Abstraction Comments | skliper
072d25b | f9b9aa5 | [#223](https://github.com/nasa/osal/issues/223) | Remove Conditional Backwards Compatibility | jphickey
05a9f42 | ff68cb3 | [#253](https://github.com/nasa/osal/issues/253) | Remove CVS Flags | skliper
3382fb8 | de84853 | [#249](https://github.com/nasa/osal/issues/249) | Remove Custom Fixed Size Types | jphickey
d5c9c27 | 48b36e5 | [#245](https://github.com/nasa/osal/issues/245) | Support Asynchronous Console Output | jphickey
41b9819 | 3a9d79c | [#108](https://github.com/nasa/osal/issues/108) | Improve Filesystem Abstraction APIs | jphickey
3382fb8 | 3c2a4f6 | [#247](https://github.com/nasa/osal/issues/247) | Update host_module_id to Use cpuaddr | jphickey
2995761 | fc64d0f | [#67](https://github.com/nasa/osal/issues/67) | Clean Priority Inheritance Conditionals | jphickey
2995761 | 7190406 | [#64](https://github.com/nasa/osal/issues/64) | Remove Backtrace Hooks | jphickey
41b9819 | 614cdd3 | [#242](https://github.com/nasa/osal/issues/242) | Add Search Lookups to Shared Layer | jphickey
2995761 | dc4b353 | [#243](https://github.com/nasa/osal/issues/243) | Remove system Call in OS_ShellOutputToFile | jphickey
d5c9c27 | ea7dc6d | [#246](https://github.com/nasa/osal/issues/246) | Add Timeout in timebase Wait | jphickey
2995761 | c1c0e3b | [#244](https://github.com/nasa/osal/issues/244) | Fix Lookup on Nonexistent Symbol in RTEMS | jphickey
NA | ff0e833 | [#234](https://github.com/nasa/osal/issues/234) | Resolve Build Errors | jphickey
38635a5 | 92b7dfa | [#205](https://github.com/nasa/osal/issues/205) | Initialize Structure Passed to mq_open | sseeger
NA | 0885ffb | [#227](https://github.com/nasa/osal/issues/227) | Resolve cppcheck Warnings | jphickey
NA | 6df485b | [#28](https://github.com/nasa/osal/issues/28) | Add Socket and Select APIs | jphickey
NA | ea71258 | [#28](https://github.com/nasa/osal/issues/28) | Refactor POSIX and RTEMS With Shared Layer | jphickey
b4e2c25 | dde418e | [#215](https://github.com/nasa/osal/issues/215) | Use C99 Boolean Types | jphickey
18f7064 | 1ce6536 | [#35](https://github.com/nasa/osal/issues/35) | Update Filesystem Abstraction APIs | jphickey
18f7064 | dde418e | [#212](https://github.com/nasa/osal/issues/212) | Fix OS API types | jphickey

#### 1.2.3 PSP Closed to Version 1.4.0: https://github.com/nasa/psp/issues?q=milestone%3A1.4.0 ####
- 1.4.0 Updates managed in Babelfish (no GitHub pull references)
  - Merge and Commit are Babelfish references, Issues are GitHub references (transfered from Babelfish)

Merge | Commit | Issue | Summary | Contributor
-- | -- | -- | -- | --
5df03c6 | 67f3d1f | [#109](https://github.com/nasa/psp/issues/109) | Use C99 bool/true/false | jphickey
5df03c6 | 9f873e0 | [#105](https://github.com/nasa/psp/issues/105) | Scrub cfe_platform_cfg.h References | jphickey
5df03c6 | 8ebf0aa | [#104](https://github.com/nasa/psp/issues/104) | Remove CFE_PSP_SUBMINOR_VERSION Define | skliper
b54acc6 | 5ac3a2a | [#106](https://github.com/nasa/psp/issues/106) | Remove ENHANCED_BUILD Switch | jphickey
5df03c6 | 39c90cd | [#98](https://github.com/nasa/psp/issues/98) | Update Headers for Copyright/License | skliper
bf35c18 | 94f4474 | [#101](https://github.com/nasa/psp/issues/101) | Remove CVS Flags | skliper
183a8f3 | 8a38cd6 | [#28](https://github.com/nasa/psp/issues/28) | PSP Panic if OS_API_Init Fails | jphickey
183a8f3 | cd39b8c | [#99](https://github.com/nasa/psp/issues/99) | Remove OS_printf Calls Prior to OS_API_Init | jphickey
22276f8 | cdee882 | [#84](https://github.com/nasa/psp/issues/84) | Replace Deprecated References | jphickey
22276f8 | 59f6d56 | [#27](https://github.com/nasa/psp/issues/27) | Integrate Timebase | jphickey
22276f8 | 6c46c00 | [#85](https://github.com/nasa/psp/issues/85) | Update for RTEMS 4.11 | jphickey
d922b34 | 13df2ae | [#90](https://github.com/nasa/psp/issues/90) | Fix String Manipulation | jphickey
7b806e1 | 5950142 | [#77](https://github.com/nasa/psp/issues/77) | Fix MCP750 FPU Exception Behavior | sseeger
ba64c8e | 43b74ee | [#82](https://github.com/nasa/psp/issues/82) | Resolve Build Warnings | jphickey
22276f8 | 52bdad0 | [#68](https://github.com/nasa/psp/issues/68) | Pass ModuleID to PSP Module Init Function | jphickey

#### 1.2.4 cFS-GroundSystem Closed to Version 2.1.0: https://github.com/nasa/cFS-GroundSystem/issues?q=milestone%3A2.1.0 ####
- 2.1.0 Integration Candidate: https://github.com/nasa/cFS-GroundSystem/pull/26

Pull | Issue | Summary | Contributor
-- | -- | -- | --
[#25](https://github.com/nasa/cFS-GroundSystem/pull/25) | [#24](https://github.com/nasa/cFS-GroundSystem/issues/24) | Use SendUdp Header File | avan989
[#23](https://github.com/nasa/cFS-GroundSystem/pull/23) | [#4](https://github.com/nasa/cFS-GroundSystem/issues/4) | Removed Unused Variable | avan989
[#22](https://github.com/nasa/cFS-GroundSystem/pull/22) | [#12](https://github.com/nasa/cFS-GroundSystem/issues/12) | Resolve Build Warnings | avan989
[#19](https://github.com/nasa/cFS-GroundSystem/pull/19) | [#5](https://github.com/nasa/cFS-GroundSystem/issues/5) | Replace gethostbyname Use | avan989

#### 1.2.5 elf2cfetbl Closed to Version 3.1.0: https://github.com/nasa/elf2cfetbl/issues?q=milestone%3A3.1.0 ####
- 3.1.0 Integration Candidate: https://github.com/nasa/elf2cfetbl/pull/13

Pull | Issue | Summary | Contributor
-- | -- | -- | --
[#11](https://github.com/nasa/elf2cfetbl/pull/11) | [#8](https://github.com/nasa/elf2cfetbl/issues/8) | Replace Deprecated References | skliper

- Imported Change From Babelfish

Pull | Issue | Summary | Contributor
-- | -- | -- | --
[#10](https://github.com/nasa/elf2cfetbl/pull/10) | [#7](https://github.com/nasa/elf2cfetbl/issues/7) | 64 Bit and Updated Machine Support | the-other-james

#### 1.2.6 tblCRCTool Closed to Version 1.1.0: https://github.com/nasa/tblCRCTool/issues?q=milestone%3A1.1.0 ####
- 1.1.0 Integration Candidate: https://github.com/nasa/tblCRCTool/pull/12
  - Note the updates were refactored in the integration candidate, so the original pull requests aren't referenced.

Pull | Issue | Summary | Contributor
-- | -- | -- | --
[#12](https://github.com/nasa/tblCRCTool/pull/12) | [#10](https://github.com/nasa/tblCRCTool/issues/10) | Cmake Build Compatibility | avan989
[#12](https://github.com/nasa/tblCRCTool/pull/12) | [#2](https://github.com/nasa/tblCRCTool/issues/2) | Resolve Build Warnings | avan989
[#12](https://github.com/nasa/tblCRCTool/pull/12) | [#1](https://github.com/nasa/tblCRCTool/issues/1) | Close File Descriptor | avan989

#### 1.2.7 ci_lab Closed to Version 2.3.0: https://github.com/nasa/ci_lab/issues?q=milestone%3A2.3.0 ####
- 2.3.0 Integration Candidate: https://github.com/nasa/ci_lab/pull/12

Pull | Issue | Summary | Contributor
-- | -- | -- | --
[#11](https://github.com/nasa/ci_lab/pull/11) | [#3](https://github.com/nasa/ci_lab/issues/3) | Deconflict Performance IDs | avan989
[#9](https://github.com/nasa/ci_lab/pull/9) | [#4](https://github.com/nasa/ci_lab/issues/4) | Remove CVS Flags | avan989
[#8](https://github.com/nasa/ci_lab/pull/8) | [#7](https://github.com/nasa/ci_lab/issues/7) | Replace Deprecated References | skliper

#### 1.2.8 sample_app Closed to Version 1.1.0: https://github.com/nasa/sample_app/issues?q=milestone%3A1.1.0 ####
- 1.1.0 Integration Candidate: https://github.com/nasa/sample_app/pull/11

Pull | Issue | Summary | Contributor
-- | -- | -- | --
[#9](https://github.com/nasa/sample_app/pull/9) | [#3](https://github.com/nasa/sample_app/issues/3) | Remove CVS Flags | avan989
[#7](https://github.com/nasa/sample_app/pull/7) | [#6](https://github.com/nasa/sample_app/issues/6) | Replace Deprecated References | skliper

#### 1.2.9 sample_lib Closed to Version 1.1.0: https://github.com/nasa/sample_lib/issues?q=milestone%3A1.1.0 ####
- 1.1.0 Integration Candidate: https://github.com/nasa/sample_lib/pull/6

Pull | Issue | Summary | Contributor
-- | -- | -- | --
[#4](https://github.com/nasa/sample_lib/pull/4) | [#1](https://github.com/nasa/sample_lib/issues/1) | Remove CVS Flags | avan989

#### 1.2.10 sch_lab Closed to Version 2.3.0: https://github.com/nasa/sch_lab/issues?q=milestone%3A2.3.0 ####
- 2.3.0 Integration Candidate: https://github.com/nasa/sch_lab/pull/13

Pull | Issue | Summary | Contributor
-- | -- | -- | --
[#10](https://github.com/nasa/sch_lab/pull/10) | [#1](https://github.com/nasa/sch_lab/issues/1) | Deconflict Performance IDs | avan989
[#8](https://github.com/nasa/sch_lab/pull/8) | [#3](https://github.com/nasa/sch_lab/issues/3) | Remove CVS Flags | avan989
[#7](https://github.com/nasa/sch_lab/pull/7) | [#6](https://github.com/nasa/sch_lab/issues/6) | Replace Deprecated References | skliper

#### 1.2.11 to_lab Closed to Version 2.3.0: https://github.com/nasa/to_lab/issues?q=milestone%3A2.3.0 ####
- 2.3.0 Integration Candidate: https://github.com/nasa/to_lab/pull/13

Pull | Issue | Summary | Contributor
-- | -- | -- | --
[#12](https://github.com/nasa/to_lab/pull/12) | [#1](https://github.com/nasa/to_lab/issues/1) | Deconflict Performance IDs | avan989
[#11](https://github.com/nasa/to_lab/pull/11) | [#3](https://github.com/nasa/to_lab/issues/3) | TO_Subscription_t Typo | avan989
[#9](https://github.com/nasa/to_lab/pull/9) | [#4](https://github.com/nasa/to_lab/issues/4) | Remove CVS Flags | avan989
[#8](https://github.com/nasa/to_lab/pull/8) | [#7](https://github.com/nasa/to_lab/issues/7) | Replace Deprecated References | skliper

### 1.3 MISSING PLANNED FEATURES AND KNOWN PROBLEMS ###
Major future work is detailed on the https://github.com/nasa/cfs README.  

Note this is not a complete list of known problems, and is not kept up to date in this VDD.
Other bugs may be identified and resolved as part of future development and testing.
See the individual repositories for a complete list.

Significant bugs not resolved in this release:
- https://github.com/nasa/cFE/issues/456: CFE_ES_PerfLogAdd needs better mutual exclusion
- https://github.com/nasa/cFE/issues/411: Exception and Reset log possible race conditions
- https://github.com/nasa/cFE/issues/410: Initialize system log implementation doesn't match requirements
- https://github.com/nasa/cFE/pull/376: Lockup on log file write
- https://github.com/nasa/cFE/issues/298: CFE_FS_Decompress infinite loop on truncated gzip file
- https://github.com/nasa/cFE/issues/295: CFE_ES_ScanAppTable possible race conditions
- https://github.com/nasa/cFE/issues/274: Number of child tasks never decremented
- https://github.com/nasa/cFE/issues/121: Invalid memory handle when restarting/deleting app with table
- https://github.com/nasa/cFE/issues/76: CFE_ES_ProcessCoreException is not interrupt-safe
- https://github.com/nasa/osal/pull/518: pthread_mutex_timedlock needs absolute time
- https://github.com/nasa/osal/pull/349: OS_SocketAccept fails
- https://github.com/nasa/osal/pull/278: Possible deadlock of timer callback
- https://github.com/nasa/osal/pull/277: Correct interval time calculations
- https://github.com/nasa/osal/pull/274: Possible deadlock of timer callbacks
- https://github.com/nasa/PSP/pull/147: Microsecond conversion lost resolution

### 1.4 DEVELOPMENT TOOL VERSIONS ASSOCIATED WITH THIS FSW VERSION ###
- Linux
  - Ubuntu 18.04.4 LTS
  - gcc 7.5.0
- RTOS
  - VxWorks 6.9
  - ccppc 4.3.3

### 1.5 EXTERNAL DOCUMENT DEPENDENCIES ###
NA - All documents associated with this release are included in the repository or attached to the release.

## 2.0 DELIVERED PRODUCTS ##

All delivered products are contained in or linked by the bundle repo at https://github.com/nasa/cFS.  Reference below are relative this bundle.  The "release package" includes files linked to the release.

Software Element | Changed | Version/Date | Location
-- | -- | -- | --
Executable(s) | N/A | N/A | Not open source deliverable
Installation Procedures | Yes | 6.7.0 | Included with release
Source Code of this FSW Build | Yes | 6.7.0 | Included with release
FSW Build Plan | N/A | N/A | Maintenance mode, rolling ~6 month releases based on funding/contributions
Design Docs | Yes | 6.7.0 | See element doc directories, additional design documents available by request
Ground System T&C Database | No | 2.1.0 | See [cFS-GroundSystem](https://github.com/nasa/cFS-GroundSystem)
Ground System Scripts | N/A | N/A | Not open source deliverable
Ground Tools | Yes | 6.7.0 | Included with release
Unit Test Procedures | Yes | N/A | See Unit Testing in this doc
Unit Test Data | Yes | 6.7.0 | Included with release
Unit Test Results | Yes | 6.7.0 | Included with release
FSW Make Files | Yes | 6.7.0 | Included with release
Linker & Complier Configuration Files | Yes | 6.7.0 | Included with release

## 3.0 INSTALLATION PROCEDURES ##
Quick start steps are detailed on the https://github.com/nasa/cfs README.

## 4.0 CONFIGURATION SUMMARY AND VERSION IDENTIFICATION ##
Default configuration is provided in cfe/cmake/sample_defs directory.

Version numbers are all reported upon execution and available in telemetry.
