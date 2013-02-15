/* File: test.h
 *
 * Purpose:
 * This is a simple header file used to remove a lot of the task definitions
 * from the example1.c file so as to make that code easier to read
 *
 * 
 *
 *
 */

/* Task 0 */
#define TASK_0_STACK_SIZE 1024
#define TASK_0_PRIORITY   230

uint32 task_0_stack[TASK_0_STACK_SIZE];

void task_0(void);

/* Task 1 */

#define TASK_1_STACK_SIZE 1024
#define TASK_1_PRIORITY   231

uint32 task_1_stack[TASK_1_STACK_SIZE];

void task_1(void);

/* Task 2 */

#define TASK_2_STACK_SIZE 1024
#define TASK_2_PRIORITY   232

uint32 task_2_stack[TASK_2_STACK_SIZE];

void task_2(void);

/* Task 3 */

#define TASK_3_STACK_SIZE 1024
#define TASK_3_PRIORITY   233

uint32 task_3_stack[TASK_3_STACK_SIZE];

void task_3(void);

/* Task 4 */

#define TASK_4_STACK_SIZE 1024
#define TASK_4_PRIORITY   234

uint32 task_4_stack[TASK_4_STACK_SIZE];

void task_4(void);

/* Task 5 */

#define TASK_5_STACK_SIZE 1024
#define TASK_5_PRIORITY   235

uint32 task_5_stack[TASK_5_STACK_SIZE];

void task_5(void);

/* Task 6 */

#define TASK_6_STACK_SIZE 1024
#define TASK_6_PRIORITY   236

uint32 task_6_stack[TASK_6_STACK_SIZE];

void task_6(void);

/* Task 7 */

#define TASK_7_STACK_SIZE 1024
#define TASK_7_PRIORITY   237

uint32 task_7_stack[TASK_7_STACK_SIZE];

void task_7(void);

/* Task 8 */

#define TASK_8_STACK_SIZE 1024
#define TASK_8_PRIORITY   238

uint32 task_8_stack[TASK_8_STACK_SIZE];

void task_8(void);

/* Task 9 */

#define TASK_9_STACK_SIZE 1024
#define TASK_9_PRIORITY   239

uint32 task_9_stack[TASK_9_STACK_SIZE];

void task_9(void);

/* Task 10 */

#define TASK_10_STACK_SIZE 1024
#define TASK_10_PRIORITY   240

uint32 task_10_stack[TASK_10_STACK_SIZE];

void task_10(void);

/* Task 11 */

#define TASK_11_STACK_SIZE 1024
#define TASK_11_PRIORITY   241

uint32 task_11_stack[TASK_11_STACK_SIZE];

void task_11(void);

/* Task 12 */

#define TASK_12_STACK_SIZE 1024
#define TASK_12_PRIORITY   242

uint32 task_12_stack[TASK_12_STACK_SIZE];

void task_12(void);

/* Task 13 */

#define TASK_13_STACK_SIZE 1024
#define TASK_13_PRIORITY   243

uint32 task_13_stack[TASK_13_STACK_SIZE];

void task_13(void);

/* Task 14 */

#define TASK_14_STACK_SIZE 1024
#define TASK_14_PRIORITY   244

uint32 task_14_stack[TASK_14_STACK_SIZE];

void task_14(void);
/* Task 15 */

#define TASK_15_STACK_SIZE 1024
#define TASK_15_PRIORITY   245

uint32 task_15_stack[TASK_15_STACK_SIZE];

void task_15(void);

/* Task 16 */

#define TASK_16_STACK_SIZE 1024
#define TASK_16_PRIORITY   246

uint32 task_16_stack[TASK_16_STACK_SIZE];

void task_16(void);

/* Task 17 */

#define TASK_17_STACK_SIZE 1024
#define TASK_17_PRIORITY   247

uint32 task_17_stack[TASK_17_STACK_SIZE];

void task_17(void);

/* Task 18 */

#define TASK_18_STACK_SIZE 1024
#define TASK_18_PRIORITY   248

uint32 task_18_stack[TASK_18_STACK_SIZE];

void task_18(void);

/* Task 19 */

#define TASK_19_STACK_SIZE 1024
#define TASK_19_PRIORITY   249

uint32 task_19_stack[TASK_19_STACK_SIZE];

void task_19(void);

/* Task 20 */

#define TASK_20_STACK_SIZE 1024
#define TASK_20_PRIORITY   250

uint32 task_20_stack[TASK_20_STACK_SIZE];

void task_20(void);



/* Global Data */

/* Task Id's for testing the number of tasks that can be created */
uint32 task_0_id, task_1_id, task_2_id, task_3_id, task_4_id, task_5_id;
uint32 task_6_id, task_7_id, task_8_id, task_9_id, task_10_id, task_11_id;
uint32 task_12_id, task_13_id, task_14_id, task_15_id, task_16_id, task_17_id;
uint32 task_18_id, task_19_id, task_20_id;
/* uint32 extra_id; */

uint32 mutex_id; 

/* Queue ID for testing the number of queues that can be created */
uint32 msgq_0, msgq_1, msgq_2, msgq_3, msgq_4, msgq_5, msgq_6;
uint32 msgq_7, msgq_8, msgq_9;
uint32 msgq_10;

uint32 msgq_id;


uint32 bin_0, bin_1, bin_2, bin_3, bin_4,bin_5, bin_6;
uint32 bin_7, bin_8, bin_9, bin_10;


uint32 mut_0, mut_1, mut_2, mut_3, mut_4,mut_5, mut_6;
uint32 mut_7, mut_8 ,mut_9, mut_10;
uint32 mutex_id;

uint32 shared_resource_x;

#define MSGQ_DEPTH        50
#define MSGQ_SIZE         4

#define PASS              0
#define FAIL              1



/* ********************** TASK 0 **************************** */

void task_0(void)
{   
    OS_TaskRegister();
    
    while(1);

    return; 
} /* end task_0 */


/* ********************** TASK 1 **************************** */

void task_1(void)
{

    OS_TaskRegister();

    while(1);
    
    return; 
} /* end task_1 */


/* ********************** TASK 2 **************************** */

void task_2(void)
{   
    OS_TaskRegister();
    
    while(1);

    return;
} /* end task_2 */

/* ********************** TASK 3 **************************** */

void task_3(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 
} /* end task_3 */
 

/* ********************** TASK 4 **************************** */

void task_4(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 
}/* end task_4 */
 

/* ********************** TASK 0 **************************** */

void task_5(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

}/* end task_5 */
 

/* ********************** TASK 6 **************************** */

void task_6(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_6 */
 

/* ********************** TASK 7 **************************** */

void task_7(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_7 */
 

/* ********************** TASK 8 **************************** */

void task_8(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

    return; 
} /* end task_8 */
 


/* ********************** TASK 9 **************************** */

void task_9(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_9 */
 


/* ********************** TASK 10 **************************** */

void task_10(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_10 */
 
 

/* ********************** TASK 11 **************************** */

void task_11(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_11 */
 


/* ********************** TASK 12 **************************** */

void task_12(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_12 */
 

/* ********************** TASK 13 ****************************  */

void task_13(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

}/* end task_13 */
 

/* ********************** TASK 14 **************************** */

void task_14(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_14 */
 


/* ********************** TASK 15 **************************** */

void task_15(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_15 */
 


/* ********************** TASK 16 **************************** */

void task_16(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_16 */
 


/* ********************** TASK 17 **************************** */

void task_17(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_17 */
 

/* ********************** TASK 18 **************************** */

void task_18(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_18 */
 


/* ********************** TASK 19 **************************** */

void task_19(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_19 */
 


/* ********************** TASK 20 **************************** */

void task_20(void)
{
    OS_TaskRegister();

    while(1);
    
    return; 

} /* end task_20 */
 




