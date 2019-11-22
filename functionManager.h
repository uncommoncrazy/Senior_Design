/*
 * functionManager.h
 *
 *  Created on: Nov 18, 2019
 *      Author: Marco
 */

#ifndef FUNCTIONMANAGER_H_
#define FUNCTIONMANAGER_H_
#define NumOfFunctions 20

extern void (*functions[NumOfFunctions]);
extern int32 counters[NumOfFunctions];
extern Uint16 order[NumberOfFuntions];
extern Uint16 NextFunction;
void   initTimer(void(*isr));
void   orginizefunctions();
void   calculateTime();
#endif /* FUNCTIONMANAGER_H_ */
