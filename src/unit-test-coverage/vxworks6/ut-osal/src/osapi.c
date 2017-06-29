/*
 * File: osapi_adaptor.c
 *
 * Purpose:
 *   Provide access to the unit under test static data and functions
 *
 * History:
 *   07/10/2015  A. Asp, Odyssey Space Research, LLC
 *    * Created
 */

/* pull in the OSAL configuration */
#include "osconfig.h"

/*
 * Now include all extra logic required to stub-out subsequent calls to
 * library functions and replace with our own
 */
#include "osapi_adaptor.h"
#include "stub-map-to-real.h"
/*
 * Strangely, the osapi.c file does this:
 * #define OK    0
 * even though "OK" is a VxWorks-supplied definition...
 */
#undef OK

/*
 * Finally include the target implementation
 */
#include "os/vxworks6/osapi.c"



void OsApi_Adaptor_Reset_Tables()
{
    memset(OS_task_table,      0, sizeof(OS_task_table));
    memset(OS_queue_table,     0, sizeof(OS_queue_table));
    memset(OS_bin_sem_table,   0, sizeof(OS_bin_sem_table));
    memset(OS_count_sem_table, 0, sizeof(OS_count_sem_table));
    memset(OS_mut_sem_table,   0, sizeof(OS_mut_sem_table));
}

SEM_ID OsApi_Adaptor_Get_Os_Task_Table_Sem()
{
    return OS_task_table_sem;
}

SEM_ID OsApi_Adaptor_Get_Os_Queue_Table_Sem()
{
    return OS_queue_table_sem;
}

SEM_ID OsApi_Adaptor_Get_Os_Bin_Sem_Table_Sem()
{
    return OS_bin_sem_table_sem;
}

SEM_ID OsApi_Adaptor_Get_Os_Count_Sem_Table_Sem()
{
    return OS_count_sem_table_sem;
}

SEM_ID OsApi_Adaptor_Get_Os_Mut_Sem_Table_Sem()
{
    return OS_mut_sem_table_sem;
}


void OsApi_Adaptor_Set_Os_Task_Table_Sem(SEM_ID newVal)
{
    OS_task_table_sem = newVal;
}

void OsApi_Adaptor_Set_Os_Queue_Table_Sem(SEM_ID newVal)
{
    OS_queue_table_sem = newVal;
}

void OsApi_Adaptor_Set_Os_Bin_Sem_Table_Sem(SEM_ID newVal)
{
    OS_bin_sem_table_sem = newVal;
}

void OsApi_Adaptor_Set_Os_Count_Sem_Table_Sem(SEM_ID newVal)
{
    OS_count_sem_table_sem = newVal;
}

void OsApi_Adaptor_Set_Os_Mut_Sem_Table_Sem(SEM_ID newVal)
{
    OS_mut_sem_table_sem = newVal;
}


OsApi_Adaptor_OS_task_record_t *OsApi_Adaptor_getTaskTableEntry(uint32 idx)
{
    if (idx < OS_MAX_TASKS)
    {
        return (OsApi_Adaptor_OS_task_record_t *)&OS_task_table[idx];
    }

    return NULL;
}

OsApi_Adaptor_OS_queue_record_t *OsApi_Adaptor_getQueueTableEntry(uint32 idx)
{
    if (idx < OS_MAX_QUEUES)
    {
        return (OsApi_Adaptor_OS_queue_record_t *)&OS_queue_table[idx];
    }

    return NULL;
}

OsApi_Adaptor_OS_bin_sem_record_t *OsApi_Adaptor_getBinSemTableEntry(uint32 idx)
{
    if (idx < OS_MAX_BIN_SEMAPHORES)
    {
        return (OsApi_Adaptor_OS_bin_sem_record_t *)&OS_bin_sem_table[idx];
    }

    return NULL;
}

OsApi_Adaptor_OS_count_sem_record_t *OsApi_Adaptor_getCountSemTableEntry(uint32 idx)
{
    if (idx < OS_MAX_COUNT_SEMAPHORES)
    {
        return (OsApi_Adaptor_OS_count_sem_record_t *)&OS_count_sem_table[idx];
    }

    return NULL;
}

OsApi_Adaptor_OS_mut_sem_record_t *OsApi_Adaptor_getMutSemTableEntry(uint32 idx)
{
    if (idx < OS_MAX_MUTEXES)
    {
        return (OsApi_Adaptor_OS_mut_sem_record_t *)&OS_mut_sem_table[idx];
    }

    return NULL;
}

void OsApi_Adaptor_setTaskTableEntry(uint32 idx, int32 free, int32 id, const char *name,
                                     int32 creator, uint32 stack_size, uint32 priority,
                                     void *delete_hook_pointer)
{
    if (idx < OS_MAX_TASKS)
    {
        OS_task_table[idx].free                = free;
        OS_task_table[idx].id                  = id;
        OS_task_table[idx].creator             = creator;
        OS_task_table[idx].priority            = priority;
        OS_task_table[idx].stack_size          = stack_size;
        OS_task_table[idx].delete_hook_pointer = delete_hook_pointer;
        strcpy(OS_task_table[idx].name, name);
    }
}

void OsApi_Adaptor_setQueueTableEntry(uint32 idx, int32 free, MSG_Q_ID id, uint32 size,
                                      const char *name, int32 creator)
{
    if (idx < OS_MAX_QUEUES)
    {
        OS_queue_table[idx].free               = free;
        OS_queue_table[idx].id                 = id;
        OS_queue_table[idx].max_size           = size;
        OS_queue_table[idx].creator            = creator;
        strcpy(OS_queue_table[idx].name, name);
    }
}

void OsApi_Adaptor_setBinSemTableEntry(uint32 idx, int32 free, SEM_ID id,
                                       const char *name, int32 creator)
{
    if (idx < OS_MAX_BIN_SEMAPHORES)
    {
        OS_bin_sem_table[idx].free             = free;
        OS_bin_sem_table[idx].id               = id;
        OS_bin_sem_table[idx].creator          = creator;
        strcpy(OS_bin_sem_table[idx].name, name);
    }
}

void OsApi_Adaptor_setCountSemTableEntry(uint32 idx, int32 free, SEM_ID id,
                                         const char *name, int32 creator)
{
    if (idx < OS_MAX_COUNT_SEMAPHORES)
    {
        OS_count_sem_table[idx].free           = free;
        OS_count_sem_table[idx].id             = id;
        OS_count_sem_table[idx].creator        = creator;
        strcpy(OS_count_sem_table[idx].name, name);
    }
}

void OsApi_Adaptor_setMutSemTableEntry(uint32 idx, int32 free, SEM_ID id,
                                       const char *name, int32 creator)
{
    if (idx < OS_MAX_MUTEXES)
    {
        OS_mut_sem_table[idx].free             = free;
        OS_mut_sem_table[idx].id               = id;
        OS_mut_sem_table[idx].creator          = creator;
        strcpy(OS_mut_sem_table[idx].name, name);
    }
}


uint32 OsApi_Adaptor_getOsTaskKey()
{
    return OS_task_key;
}

void OsApi_Adaptor_setOsTaskKey(uint32 key)
{
    OS_task_key = key;
}

uint32 OsApi_Adaptor_getOsPrintfEnabled()
{
    return OS_printf_enabled;
}

void OsApi_Adaptor_setOsPrintfEnabled(uint32 val)
{
    OS_printf_enabled = val;
}

