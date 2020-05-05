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
 * \file     hostLib.h
 * \ingroup  ut-stubs
 * \author   joseph.p.hickey@nasa.gov
 *
 * OSAL coverage stub replacement for hostLib.h
 */

#ifndef INCLUDE_HOSTLIB_H_
#define INCLUDE_HOSTLIB_H_

#include <OCS_hostLib.h>
#include <vxWorks.h>


/* ----------------------------------------- */
/* mappings for declarations in hostLib.h */
/* ----------------------------------------- */
#define hostGetByName       OCS_hostGetByName

#endif  /* INCLUDE_HOSTLIB_H_ */

