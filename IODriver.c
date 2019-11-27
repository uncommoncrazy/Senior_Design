/*
 * IODriver.c
 *
 *  Created on: Feb 26, 2019
 *      Author: Claudine
 */
#include<F28x_Project.h>
#include "IODriver.h"

void Init_IO(void)
{
    EALLOW;
    Uint32 GPATemp = GpioCtrlRegs.GPADIR.all | LCDDataPins;
    GPATemp &= (~PU);
    GpioCtrlRegs.GPADIR.all = GPATemp;
    GpioCtrlRegs.GPAPUD.all &= ~(PU);
}
void Init_butInterrupt(void(*Xi1ISR),void(*Xi2ISR),void(*Xi3ISR))
{
    // XBAR Input 4,5,6
    EALLOW;
    // Select Button for input Xbar
    InputXbarRegs.INPUT4SELECT= 14;
    InputXbarRegs.INPUT5SELECT= 15;
    InputXbarRegs.INPUT6SELECT= 16;
    //Enable Falling Edge Interrupts
    XintRegs.XINT1CR.bit.POLARITY = 0;
    XintRegs.XINT1CR.bit.ENABLE = 1;
    XintRegs.XINT2CR.bit.POLARITY = 0;
    XintRegs.XINT2CR.bit.ENABLE = 1;
    XintRegs.XINT3CR.bit.POLARITY = 0;
    XintRegs.XINT3CR.bit.ENABLE = 1;
    // Assigned Passed in routine address to VectorTable
    PieVectTable.XINT1_INT=Xi1ISR;
    PieVectTable.XINT2_INT=Xi2ISR;
    PieVectTable.XINT3_INT=Xi3ISR;
    //Input 4, XINT1
    PieCtrlRegs.PIEIER1.bit.INTx4=1;
    //Input 5, XINT2
    PieCtrlRegs.PIEIER1.bit.INTx5=1;
    //Input 6, XINT2
    PieCtrlRegs.PIEIER12.bit.INTx1=1;
    IER |= M_INT1;
    IER |= M_INT12;
}
Uint16  ButtonPressed(Uint16* Current,Uint16* Last)
{
    *Current = GpioDataRegs.GPADAT.bit.GPIO16;
    if((*Current ==0)&&(*Last==1)){
        *Last=*Current;
        return 1;
    }
    else{
        *Last=*Current;
        return 0;
    }
}
Uint16 getButton()
{
 Uint16 but  = GpioDataRegs.GPADAT.bit.GPIO16;
    if (but){
        return 0;
    }else{
        return 1;
    }
}

Uint16 getSwitches(){
    Uint16 switches=GpioDataRegs.GPADAT.all&SwitchPins;
    return switches >>8;
}

void Init_LCDPins(){
   EALLOW;
   GpioCtrlRegs.GPADIR.all |= LCDDataPins;
   GpioCtrlRegs.GPBDIR.bit.GPIO33 =1;//RD
   GpioCtrlRegs.GPADIR.bit.GPIO11 =1;//WR
   GpioCtrlRegs.GPADIR.bit.GPIO12 =1;//C/D
   GpioCtrlRegs.GPADIR.bit.GPIO15 =1;//CS
}
void LCD_Write8(Uint16 Data){
    int16 send = Data&1;
    send |= (Data&2)<<1;
    send |= (Data&0b01111100)<<2;
    send |= (Data&0x80)<<3;

    GpioDataRegs.GPASET.all = send & LCDDataPins;
    GpioDataRegs.GPACLEAR.all = ~send & LCDDataPins;
    LCD_WR_Strobe;
}
Uint16 LCD_Read8(){
    LCD_Read_On;
    LCD_Delay;
    Uint16 result = GpioDataRegs.GPADAT.all & LCDDataPins;
    LCD_Read_Off;
    return result;
}
void LCD_WriteReg8(Uint16 address, Uint16 data){
LCD_CD_Command;
LCD_Write8(address);
LCD_CD_Data;
LCD_Write8(data);
}
void LCD_WriteReg16(Uint16 address, Uint16 data){
LCD_CD_Command;
LCD_Write8(address>>8);
LCD_Write8(address);
LCD_CD_Data;
LCD_Write8(data>>8);
LCD_Write8(data);
}
void LCD_WriteRegPair(Uint16 address, Uint16 data){
LCD_CD_Command;
LCD_Write8(address>>8);
LCD_CD_Data;
LCD_Write8(data>>8);
LCD_CD_Command;
LCD_Write8(address);
LCD_CD_Data;
LCD_Write8(data);
}
