This directory holds the unit tests for the TO application.

To build and run the unit tests:
1. Be sure an appropriate *_to_types.h is in the apps/inc directory by:
  a. cd ../examples
  b. ./setup.sh -m CFS_TST udp     (see below)
  c. cd ../unit_test
2. make clean
3. make
4. make run
5. make gcov

Background:
The unit tests also expect (like the apps) to find the 
 apps/inc/CFS_TST_ci_types.h
 apps/inc/CFS_TST_to_types.h
where the mission name, CFS_TST, is assumed by default.

These are put into place by the [ci/to]/fsw/examples/setup.py scripts.
Choose the appropriate name for your code to compile if you aren't 
using "CFS_TST".
