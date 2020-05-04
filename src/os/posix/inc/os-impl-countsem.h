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
 * \file     os-impl-countsem.h
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_COUNTSEM_H_
#define INCLUDE_OS_IMPL_COUNTSEM_H_

#include <osconfig.h>
#include <semaphore.h>

typedef struct
{
    sem_t           id;
} OS_impl_countsem_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_countsem_internal_record_t  OS_impl_count_sem_table     [OS_MAX_COUNT_SEMAPHORES];


#endif  /* INCLUDE_OS_IMPL_COUNTSEM_H_ */

