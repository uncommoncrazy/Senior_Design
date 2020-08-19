/*
 * TCB.h
 *
 *  Created on: Aug 18, 2020
 *      Author: Marco
 */

#ifndef RTOS_FILES_MARC_OS_H_
#define RTOS_FILES_MARC_OS_H_
#define NUMTHREADS 3
#define NUMTHREADS 100
#include<F28x_Project.h>
typedef struct tcbType {
    int32 * sp;
    tcbType * next;
} TCB;

extern TCB tcbs[NUMTHREADS];
extern TCB * RunPt;
extern int32 Stacks[NUMTHREADS][NUMTHREADS];
void SetInitialStack(int i);
int OS_AddThreads(void(*t0)(void),void(*t1)(void),void(*t2)(void));
int StartCritical();
void EndCritical();
void Task0(void);
void Task1(void);
void Task2(void);
void OS_Init();
void Profile_Init();
void OS_Launch(float sysTicFreq);
#endif /* RTOS_FILES_MARC_OS_H_ */
