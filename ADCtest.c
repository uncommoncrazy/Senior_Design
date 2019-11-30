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
#include "ADCDriver.h"
__interrupt void adca1_isr(void);

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
    PieVectTable.ADCA1_INT = &adca1_isr; //function for ADCA interrupt 1
    EDIS;
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer1, 200, 1000000);
    IER |= M_INT1; //Enable group 1 interrupts
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    Init_ADC();
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
        if(convertADC(lastconversion&0xfff) != convertADC(adcSignal&0xfff)){
           // fillScreen(color[1]);
            //drawText(ADCValue);
            //drawThousands(adcSignal>>2,200,100, color[0]);
            drawChar('V', color[0], 200, drawFloat(convertADC(adcSignal),200,  drawText(ADCValue), color[0]));
            lastconversion = adcSignal;
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
