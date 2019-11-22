

/**
 * main.c
 */
/*
 *  this code streams from input jack of codec to output jack
 */
#include<F28x_Project.h>
#include <stdio.h>
#include <stdlib.h>
#include "InitAIC23.h"
#include "OneToOneI2CDriver.h"
//#define     ADCA_BASE    0x7400
#define     LEDPins     0x00ff
#define     nLEDPins    0xffffff00
#define     PBPins      0x0001C000
#define     SwitchPins  0x00000f00
#define     Switch      8
#define     PB          9
#define     PU          (PBPins|SwitchPins)
#define     upperNib    0xf0
#define     lowerNib    0x0f
#define     EnableHi    0x0C
#define     EnableLo    0x08
#define     RS          0x01
#define     RSEnableHi  0x0C | RS
#define     RSEnableLo  0x08 | RS
#define     LCD_init1   0x33
#define     LCD_init2   0x32
#define     Mode        0x28
#define     DCP         0x0f
#define     ClearDisp   0x01
#define     Line2       0xC0
#define     LCD_addr    0x27
#define     HighByte    7
#define     LowByte     9
#define     SRAM_SizeW  0x1ffff
#define     inst_Read   3<<8
#define     inst_Write  2<<8
#define     left        0
#define     right       1
int16 audioIn;
Uint16 audioOut;
Uint16 audioFlags;
//Uint16  channel = left;
int16 channel1=0;
#define audioReady 1
int16 channel2=0;
Uint16 channel=0;
Uint16  interruptStore=0;
__interrupt void MSBR_isr(void);
int main(void)
{
    InitSysCtrl();

    InitSPIA();

    InitMcBSPb();

    InitAIC23();
    EALLOW;
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EALLOW;
    PieVectTable.MCBSPB_RX_INT = &MSBR_isr;
    PieCtrlRegs.PIEIER6.bit.INTx7 = 1;
    IER |= M_INT6; //Enable group 1 interrupts
    EINT;
    while(1);
}


//__interrupt void MSBR_isr(void)
//{
//        // right channel transmission
//       interruptStore=IER;
//       IER &= M_INT6;
//          channel1 = McbspbRegs.DRR1.all;
//          channel2 = McbspbRegs.DRR2.all;
//          audioIn = channel1;
//          McbspbRegs.DXR1.all = channel1;
//          McbspbRegs.DXR2.all = channel1;
//          audioFlags|=audioReady;
//          PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
//          IER|=interruptStore;
//       audioFlags|=audioReady;
//       PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
//       IER|=interruptStore;
//}
__interrupt void MSBR_isr(void)
{
        // right channel transmission
       if(channel)
       {
           McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
           McbspbRegs.DXR2.all = McbspbRegs.DRR2.all;
           channel = left;
       }
       // left channel transmission
       else
       {
           McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
           McbspbRegs.DXR2.all = McbspbRegs.DRR2.all;
           channel = right;
       }
       PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
}
