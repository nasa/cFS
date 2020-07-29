##############################################################################
## $Id: README.txt 1.3 2017/06/21 16:45:41EDT mdeschu Exp  $
##
## Purpose: CFS SCH application unit test instructions, results, and code coverage
##
## Author: Charles Zogby
##
##############################################################################


-------------------------
SCH Unit Test Instructions
-------------------------
This unit test was run in a virtual machine running CentOS and uses the ut-assert stubs and default hooks for the 
cFE, OSAL and PSP. The ut-assert framework, stubs, and hooks are located in the directory cfe/tools/ut-assert and 
are configuration managed in MKS in the FSW tools repository on server tlserver3.

To run the unit test enter the following commands at the command line prompt in
unit test directory.

make clean
make 
make run
make gcov

SCH 2.2.1.0 Unit Test Results (default configuration of SCH_LIB_PRESENCE = 1)

Tests Executed:    142
Assert Pass Count: 633
Assert Fail Count: 0

gcov: '../src/sch_cmds.c' 100.00%  269
gcov: '../src/sch_api.c' 100.00%  12
gcov: '../src/sch_app.c' 100.00%  337
gcov: '../src/sch_custom.c' 100.00%  85

==========================================================================
sch_cmds.c - 100.00% coverage

==========================================================================
sch_api.c - 100.00% coverage

==========================================================================
sch_app.c - 100.00% coverage

==========================================================================
sch_custom.c - 100.00% coverage

==========================================================================

SCH 2.2.1.0 Unit Test Results (alternate configuration of SCH_LIB_PRESENCE = 0):


Tests Executed:    142
Assert Pass Count: 633
Assert Fail Count: 0

gcov: '../src/sch_cmds.c' 100.00%  269
gcov: '../src/sch_api.c' 100.00%  12
gcov: '../src/sch_app.c' 100.00%  336
gcov: '../src/sch_custom.c' 100.00%  85

==========================================================================
sch_cmds.c - 100.00% coverage

==========================================================================
sch_api.c - 100.00% coverage

==========================================================================
sch_app.c - 100.00% coverage

==========================================================================
sch_custom.c - 100.00% coverage

==========================================================================
