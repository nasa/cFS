Travis-CI: [![Build Status](https://travis-ci.com/nasa/PSP.svg)](https://travis-ci.com/nasa/PSP)

# Core Flight System : Framework : Platform Support Package

This repository contains NASA's Platform Support Package (PSP), which is a framework component of the Core Flight System.

This is a collection of APIs abstracting platform specific functionality to be located in the `psp` subdirectory of a cFS Mission Tree. The Core Flight System is bundled at <https://github.com/nasa/cFS>, which includes build and execution instructions.

## Version History

### Development Build: 1.4.11

- Removes non-termination string warnings when building with GCC9.
- Exception handling is now implemented on POSIX. There is no longer a separate handler for SIGINT - it is now treated as an exception and goes through the normal process which ends up "restarting" CFE. On pc-linux causes the process to exit normally. There is now a mechanism to capture the CTRL+C exception code and use it during normal test cycles.
- See <https://github.com/nasa/PSP/pull/160>

#### Development Build: 1.4.10

- Implements full-precision microsecond conversion
- See <https://github.com/nasa/PSP/pull/155>

### Development Build: 1.4.9

- RTEMS builds successfully without errors
- Build script uses a proper CMakeLists.txt instead of the aux_source directory
- Minor updates (see <https://github.com/nasa/PSP/pull/153>)

### Development Build: 1.4.8

- Minor updates (see <https://github.com/nasa/PSP/pull/151>)

### Development Build: 1.4.7

- Fixed some build warnings for MCP750
- Minor updates (see <https://github.com/nasa/PSP/pull/142>)

### Development Build: 1.4.6

- Minor updates (see <https://github.com/nasa/PSP/pull/141>)

### Development Build: 1.4.5

- Simplifies array handling in VxWorks
- Minor updates (see <https://github.com/nasa/PSP/pull/138>)

### Development Build: 1.4.4

- Minor updates (see <https://github.com/nasa/PSP/pull/132>)

### Development Build: 1.4.3

- Minor updates (see <https://github.com/nasa/PSP/pull/130>)

### Development Build: 1.4.2

- Minor updates (see <https://github.com/nasa/PSP/pull/127>)

### Development Build: 1.4.1

- Minor updates (see <https://github.com/nasa/PSP/pull/115>)

### **_1.4.0 OFFICIAL RELEASE_**

- This is a point release from an internal repository
- Changes are detailed in [cFS repo](https://github.com/nasa/cFS) release documentation
- Released as part of cFE 6.7.0, Apache 2.0

### **_1.3.0a OFFICIAL RELEASE_**

- This is a point release from an internal repository
- See [version description document](https://github.com/nasa/PSP/blob/v1.3.0a/doc/PSP%201.3.0.0%20Version%20Description%20Document.pdf)
- Released as part of cFE 6.6.0a, Apache 2.0

The open source release does not include all PSPs that have been developed. Only the three PSPs included are managed by the community CCB. PSPs developed by other organizations can be requested through the mechanisms listed below. Note the framework PSPs delivered may change in the future as platforms become obsolete.

## Known issues

See all open issues and closed to milestones later than this version.

## Getting Help

For best results, submit issues:questions or issues:help wanted requests at <https://github.com/nasa/cFS>.

Official cFS page: <http://cfs.gsfc.nasa.gov>
