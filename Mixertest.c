// Marco Munoz
/**
 * main.c
 */
/*Descriptions
 * This codes use given adc & lcd drivers to write voltage to lcd
 */
#include<F28x_Project.h>
#include <stdio.h>
#include <stdlib.h>
#include "DisplayLibrary.h"
#include "InitAIC23.h"
#include "ADCDriver.h"
__interrupt void adca1_isr(void);
Uint16 channel1,channel2;
int16 audioOut,audioReady;
Uint16 adcSignal = 0;
__interrupt void MCBSP_isr(void);
float conversion;

int main(void)
{
    InitSysCtrl();
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer1, 200, 10000);
    InitSPIA();

    InitMcBSPb();

    InitAIC23();
    Init_ADC();

    EALLOW;
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EALLOW;
    PieVectTable.MCBSPB_RX_INT = &MCBSP_isr;
    PieCtrlRegs.PIEIER6.bit.INTx7 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;

    PieVectTable.ADCA1_INT = &adca1_isr; //function for ADCA interrupt
    IER |= M_INT1; //Enable group 1 interrupts

    EALLOW;
    IER |= M_INT6; //Enable group 1 interrupts
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM
  //  PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    SetupADCTimer1();
    Uint16 lastconversion = 0;
    Init_LCDPins();
    startLCD();
 //   Uint16 ID=getID();
    Uint16 color[2];
    color[0] = genColor(0xff, 0xff, 0xff);
    color[1] = genColor(0, 0, 0);
    Text ADCValue = { .string = "Voltage = ", .color =color[0], .x =200, .y=0};
    while(1){
        if(audioReady){
            //drawText(ADCValue);
            //drawThousands(adcSignal>>2,200,100, color[0]);
           fillScreen(color[1]);
           //adcSignal = AdcaResultRegs.ADCRESULT0;
           conversion=convertADC(adcSignal)/3.3;
           drawChar('A', color[0], 200, drawFloat(conversion,200, 0, color[0]));
            //audioOut = (int16)(((float)((int16)(channel1))*((float)((int16)(channel2)))/32768.0));


            audioReady=0;

            //DELAY_US(100000);

        }

    }
}
__interrupt void adca1_isr(void)
{
    adcSignal = AdcaResultRegs.ADCRESULT0;
    EALLOW;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
    CpuTimer1.RegsAddr->TCR.bit.TIF = 1;
}
__interrupt void MCBSP_isr(void)
{
        EALLOW;
        // right channel transmission
     //interruptStore=IER;
   //        IER &= M_INT6;
     //      interruptStore=IER;
       //       IER &= M_INT6;
              channel1 = McbspbRegs.DRR1.all;
              channel2 = McbspbRegs.DRR2.all;
              McbspbRegs.DXR1.all = audioOut;
              McbspbRegs.DXR2.all = audioOut;
              audioReady=1;
              audioOut = (int16)(((float)((int16)(channel1))+(float)((int16)(channel2)))*0.5*conversion);
              // fill audio buffer
              PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
             // IER|=interruptStore;
//        // right channel transmission
//       if(channel)
//       {
//           McbspbRegs.DXR1.all = McbspbRegs.DRR1.all;
//           McbspbRegs.DXR2.all = McbspbRegs.DRR2.all;
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
}

