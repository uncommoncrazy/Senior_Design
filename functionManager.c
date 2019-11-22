/*
 * functionManger.h
 *
 *  Created on: Nov 19, 2019
 *      Author: Marco
 */
#include 'functionManager.h'
void   initTimer(void(*isr)){
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.TIMER0_INT = isr;
    T
//
   IER |= M_INT1;

//
// Enable TINT0 in the PIE: Group 1 __interrupt 7
//
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

//
// Enable global Interrupts and higher priority real-time debug events:
//
   EINT;   // Enable Global __interrupt INTM
   EDIS;    //
}
