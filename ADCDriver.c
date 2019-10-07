/*
 * ADCDriver.c
 *
 *  Created on: Feb 23, 2019
 *      Author: Claudine
 */
#include<F28x_Project.h>
#include <stdio.h>
#include <stdlib.h>
#include "ADCDriver.h"
void Init_ADC(void)
{

    EALLOW;
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    //Set pulse positions to late
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
    //power up the ADCs
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    //delay for 1ms to allow ADC time to power up
    DELAY_US(1000);
    EDIS;
}
void Init_ADCInterrupt(void(*adc_isr))
{
    EALLOW;
    PieVectTable.ADCA1_INT = adc_isr; //function for ADCA interrupt 1
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;
    IER |= M_INT1; //Enable group 1 interrupts
}
void SetupADCTimer1(void)
{
    //Select the channels to convert and end of conversion flag
    //
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;  //SOC0 will convert internal
                                         //connection A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 100; //sample window is 100
                                                      //SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 2; //trigger on cpu1timmer
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    EDIS;
    CpuTimer1.RegsAddr->TCR.bit.TSS=0;
}
float convertADC(Uint16 adcValue)
{
   float result =  (float)(adcValue)*Resolution;
   float offset = 0.004;
   if(adcValue==4095)
       offset = 0.0;

    return result - offset;
}
float trConvertADC(Uint16 adcValue)
{
    return (float)(adcValue>>trashBits)*trResolution - (float)(adcValue>>trashBits)*trResolution*0.08;

}

