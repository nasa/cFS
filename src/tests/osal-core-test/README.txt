Explanation:

This osal-core-test.c file is a test to designed to demonstrate the functionality of the functions of osapi.c in the ../os/<OS>/ directory. The tests are separated into different modules- tasks, queues, binary semaphores, mutexes, and *GetInfo functions. 

This separation allows for the modules to be run individually or concurrently. Each module does not affect any of the others.

NOTE: If everything is working and configured properly, zero errors should be reported.

************ TestTasks *************

Tries to create OS_MAX_TASKS + 1 to test creating the maximum and going one over.

Tries to delete the previously created tasks, including one that wasn't created, and ones that were previously deleted.

Tries to create tasks again, with two sharing a name (the second one should fail).

Tries to get the ID of the tasks by their names.

Tries to Delete the tasks created previously.

NOTE: for clairity, the tasks are declared in test1.h

************ TestQueues *************

Tries to create OS_MAX_QUEUES + 1 to test creating the maximum and going one over.

Tries to delete the previously created queues, including one that wasn't created.

Tries to create queues again, with two sharing a name (the second one should fail).

Tries to get the ID of the queues by their names.

Tries to Delete the queues created previously.

************ TestBinaries *************

Tries to create OS_NUM_OF_SEMAPHORES + 1 to test creating the maximum and going one over.

Tries to delete the previously created binary semaphores, including one that wasn't created.

Tries to create binary semaphores again, with two sharing a name (the second one should fail). Some of the semaphores are created empty, so deletion of empty semaphores should fail.

Tries to get the ID of the binary semaphores by their names.

Tries to Delete the binary semaphores created previously.

************ TestMutexes *************

Tries to create OS_NUM_OF_SEMAPHORES + 1 to test creating the maximum and going one over.

Tries to delete the previously created mutexs, including one that wasn't created.

Tries to create mutexs again, with two sharing a name (the second one should fail). 
Tries to get the ID of the mutexs by their names.

Tries to Delete the mutexs created previously.



