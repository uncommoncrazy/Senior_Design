

/**
 * main.c
 */
/*Descriptions
 * This codes use given driver to write name and course code on the lcd
 */
#include<F28x_Project.h>
#include <stdio.h>
#include <stdlib.h>

#include "IODriver.h"
#include "TFTLCD_Driver.h"
//#include "TouchScreenDriver.h"
#include "DisplayLibrary.h"
#define     left        0
#define     right       1
Uint16  channel = left;
Uint16  interruptStore=0;
Uint32  leftChannel = 0;
Uint32  rightChannel =0;
__interrupt void adca1_isr(void);
__interrupt void MSBR_isr(void);
__interrupt void BUT1_isr(void);
__interrupt void BUT2_isr(void);
__interrupt void BUT3_isr(void);
#define cosSize   0000
Uint16 Buttons[3]={0,0,0};
Uint16  index = 0;
Uint16 adcSignal = 0;
int main(void)
{
    InitSysCtrl();
    InitGpio();
    EALLOW;
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EALLOW;

   //Enable group 1 interrupts
    EDIS;
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer1, 200, 1000000);
    EINT;  // Enable Global interrupt INTM

    Init_LCDPins();
    startLCD();
    Uint16 color[2];
    color[0] = genColor(0xff, 0xff, 0xff);
    color[1] = genColor(0, 0, 0);
    Text hello = { .string = "VCO", .color =color[0], .x =20, .y=0};

    fillScreen(color[1]);
    while(1){
       // drawChar('A', color, capitalLetter10, 100, 100);
        //test(100, 100, 20)
        //drawText(hello);
        fillRect(100, 200, 20, 20, color[0]);
//        if(touched()){
//            point=getTouchPoint(0);
//            drawHorzLine( 0, point.y, 300, genColor(0,0xff,0));
//            drawVertLine( point.x, 0, 800, genColor(0,0xff,0));
//        }
//        DELAY_US(10000);
    }
}

//__interrupt void MSBR_isr(void)
//{
//        // right channel transmission
//       interruptStore=IER;
//       IER &= M_INT6;
//       if(channel)
//       {
//           rightChannel = (Uint32)(McbspbRegs.DRR1.all);
//           rightChannel |= ((Uint32)(McbspbRegs.DRR2.all))<<16;
//           McbspbRegs.DXR1.all =0;
//           McbspbRegs.DXR2.all =(Uint16) ((rightChannel>>16)&0xffff);
//           channel = left;
//       }
//       // left channel transmission
//       else
//       {
//           McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
//           McbspbRegs.DXR2.all = McbspbRegs.DRR2.all;
//           channel = right;
//       }
//       PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
//       IER|=interruptStore;
//}
__interrupt void BUT1_isr(void)
{
    Buttons[0] = 1;
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}
__interrupt void BUT2_isr(void)
{
    Buttons[1] = 1;
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;

}
__interrupt void BUT3_isr(void)
{
    Buttons[2] = 1;
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP12;
}
