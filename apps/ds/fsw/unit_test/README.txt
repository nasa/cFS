##############################################################################
## $Id: README.txt 1.4.1.1 2017/02/06 01:43:33EST mdeschu Exp  $
##
## Purpose: CFS DS application unit test instructions, results, and code coverage
##
## Author: Charles Zogby
##
##############################################################################


-------------------------
DS Unit Test Instructions
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

NOTE: Some tests are dependent on changing certain values in ./ds_platform_cfg.h.  Because of this, the test suite should be 
run twice, in the following two configurations: default (DS_FILE_HEADER_TYPE = 1, DS_MOVE_FILES = FALSE) and 
alternate (DS_FILE_HEADER_TYPE = 2, DS_MOVE_FILES = TRUE).  Expected results for each configuration are shown in 
./ds_test_log_defaultconfig.txt and ./ds_test_log_altconfig.txt.

DS 2.5.0.0 Unit Test Results (with default configuration of ds_platform_cfg.h: DS_FILE_HEADER_TYPE = 1, DS_MOVE_FILES = FALSE):

Tests Executed:    207
Assert Pass Count: 645
Assert Fail Count: 0

gcov: '../src/ds_file.c' 100.00%  178
gcov: '../src/ds_app.c' 100.00%  146
gcov: '../src/ds_cmds.c' 100.00%  439
gcov: '../src/ds_table.c' 100.00%  319


==========================================================================
ds_file.c - 100.00% coverage

==========================================================================
ds_app.c - 100.00% coverage

==========================================================================
ds_cmds.c - 100.00% coverage

==========================================================================
ds_table.c - 100.00% coverage

==========================================================================

DS 2.5.0.0 Unit Test Results (with modified ds_platform_cfg.h: DS_FILE_HEADER_TYPE = 2, DS_MOVE_FILES = TRUE):

Tests Executed:    206
Assert Pass Count: 652
Assert Fail Count: 0

gcov: '../src/ds_file.c' 100.00%  200
gcov: '../src/ds_app.c' 100.00%  146
gcov: '../src/ds_cmds.c' 99.54%  439
gcov: '../src/ds_table.c' 100.00%  319


==========================================================================
ds_file.c - 100.00% coverage

==========================================================================
ds_app.c - 100.00% coverage

==========================================================================
ds_cmds.c - 99.54% coverage

Note: The section of code not covered in this configuration is covered in the other configuration.

==========================================================================
ds_table.c - 100.00% coverage

==========================================================================

