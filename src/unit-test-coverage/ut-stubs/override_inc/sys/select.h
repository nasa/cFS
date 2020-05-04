/*
 * 
 *    Copyright (c) 2020, United States government as represented by the
 *    administrator of the National Aeronautics Space Administration.
 *    All rights reserved. This software was created at NASA Goddard
 *    Space Flight Center pursuant to government contracts.
 * 
 *    This is governed by the NASA Open Source Agreement and may be used,
 *    distributed and modified only according to the terms of that agreement.
 * 
 */


/**
 * \file     select.h
 * \ingroup  override_inc
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_SELECT_H_
#define INCLUDE_SELECT_H_

#include <OCS_sys_select.h>

/* ----------------------------------------- */
/* constants normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* types normally defined in sys/select.h */
/* ----------------------------------------- */

/* ----------------------------------------- */
/* mappings for declarations in sys/select.h */
/* ----------------------------------------- */

#define fd_set  OCS_fd_set
#define select  OCS_select

#define FD_SET      OCS_FD_SET
#define FD_ISSET    OCS_FD_ISSET
#define FD_CLR      OCS_FD_CLR
#define FD_ZERO     OCS_FD_ZERO

#endif  /* INCLUDE_SELECT_H_ */

