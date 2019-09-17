/*
 *      Copyright (c) 2018, United States government as represented by the
 *      administrator of the National Aeronautics Space Administration.
 *      All rights reserved. This software was created at NASA Glenn
 *      Research Center pursuant to government contracts.
 *
 *      This is governed by the NASA Open Source Agreement and may be used,
 *      distributed and modified only according to the terms of that agreement.
 */

/**
 * \file   os-impl-console-directwrite.c
 * \author joseph.p.hickey@nasa.gov
 *
 * Purpose:
 *      For many platforms the "OS_printf" output can be sent to the console
 *      using a standard file descriptor provided by the C library using
 *      the write() call.
 *
 * NOTE: This is a "template" file and not a directly usable source file.
 *       It must be adapted/instantiated from within the OS-specific
 *       implementation on platforms that wish to use this template.
 */

/****************************************************************************************
                                    INCLUDE FILES
 ***************************************************************************************/

/* Handled by includer */

/****************************************************************************************
                                CONSOLE OUTPUT
 ***************************************************************************************/

                        
/*----------------------------------------------------------------
 *
 * Function: OS_ConsoleOutput_Impl
 *
 *  Purpose: Implemented per internal OSAL API
 *           See description in os-impl.h for argument/return detail
 *
 *-----------------------------------------------------------------*/
void  OS_ConsoleOutput_Impl(uint32 local_id)
{
    uint32 StartPos;
    uint32 EndPos;
    long WriteSize;
    OS_console_internal_record_t *console;
    int fd;

    console = &OS_console_table[local_id];
    fd = OS_impl_console_table[local_id].out_fd;
    StartPos = console->ReadPos;
    EndPos = console->WritePos;
    while (StartPos != EndPos)
    {
        if (StartPos > EndPos)
        {
            /* handle wrap */
            WriteSize = console->BufSize - StartPos;
        }
        else
        {
            WriteSize = EndPos - StartPos;
        }

        WriteSize = write(fd,
                &console->BufBase[StartPos],
                WriteSize);

        if (WriteSize <= 0)
        {
            /* write error */
            /* This debug message _might_ go to the same console,
             * but might not, so its worth a shot. */
            OS_DEBUG("%s(): write(): %s\n", __func__, strerror(errno));
            break;
        }

        StartPos += WriteSize;
        if (StartPos >= console->BufSize)
        {
            /* handle wrap */
            StartPos = 0;
        }
    }

    /* Update the global with the new read location */
    console->ReadPos = StartPos;
} /* end OS_ConsoleOutput_Impl */





