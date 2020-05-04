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
 * \file     ut-adaptor-queues.h
 * \ingroup  adaptors
 * \author   joseph.p.hickey@nasa.gov
 *
 */

#ifndef INCLUDE_UT_ADAPTOR_QUEUES_H_
#define INCLUDE_UT_ADAPTOR_QUEUES_H_

#include <common_types.h>
#include <OCS_msgQLib.h>

extern void*  const UT_Ref_OS_impl_queue_table;
extern size_t const UT_Ref_OS_impl_queue_table_SIZE;


/*****************************************************
 *
 * UT FUNCTION PROTOTYPES
 *
 * These are functions that need to be invoked by UT
 * but are not part of the implementation API.
 *
 *****************************************************/

int32 UT_Call_OS_VxWorks_QueueAPI_Impl_Init(void);


#endif  /* INCLUDE_UT_ADAPTOR_QUEUES_H_ */

