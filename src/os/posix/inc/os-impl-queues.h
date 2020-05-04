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
 * \file     os-impl-queues.h
 * \ingroup  posix
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_OS_IMPL_QUEUES_H_
#define INCLUDE_OS_IMPL_QUEUES_H_

#include <osconfig.h>
#include <mqueue.h>

/* queues */
typedef struct
{
    mqd_t id;
} OS_impl_queue_internal_record_t;

/* Tables where the OS object information is stored */
extern OS_impl_queue_internal_record_t     OS_impl_queue_table         [OS_MAX_QUEUES];


#endif  /* INCLUDE_OS_IMPL_QUEUES_H_ */

