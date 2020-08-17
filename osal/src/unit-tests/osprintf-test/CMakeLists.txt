# CMake snippet for OSAL printf test

set(TEST_MODULE_FILES
  ut_osprintf.c
  ut_osprintf_c.c
  ut_osprintf_d.c
  ut_osprintf_f.c
  ut_osprintf_i.c
  ut_osprintf_ld.c
  ut_osprintf_lf.c
  ut_osprintf_li.c
  ut_osprintf_lu.c
  ut_osprintf_lx.c
  ut_osprintf_lx_uc.c
  ut_osprintf_misc.c
  ut_osprintf_offset.c
  ut_osprintf_offset_dummy.c
  ut_osprintf_p.c
  ut_osprintf_printf.c
  ut_osprintf_s.c
  ut_osprintf_u.c
  ut_osprintf_x.c
  ut_osprintf_x_uc.c
)

add_stubs(TEST_STUBS os)
add_osal_ut_exe(osal_osprintf_UT ${TEST_MODULE_FILES} ${TEST_STUBS})

