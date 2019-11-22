

/**
 * main.c
 */
/*Descriptions
 * This codes use given driver to write name and course code on the lcd
 */
#include<F28x_Project.h>
#include <stdio.h>
#include <stdlib.h>
#include "AIC23.h"
#include "IODriver.h"
#include "audioFX.h"
#define     left       0
#define     right       1
Uint16 audioFlags;
Uint16 audioIsReady;
Uint16 recordFlag;
Uint16 playFlag;
Uint16 audioIn;
Uint16 audioOut;
__interrupt void MSBR_isr(void);
__interrupt void BUT1_isr(void);
__interrupt void BUT2_isr(void);
__interrupt void BUT3_isr(void);
#define cosSize   0000
Uint16 Buttons[3]={0,0,0};
Uint16  index = 0;
Uint16 adcSignal = 0;
int16 channel1=0;
int16 channel2=0;
Uint16 chanl=0;
Uint32 intrpCounter=0;
Uint16  interruptStore=0;
int16 interpBuff[2];
int main(void)
{
    InitSysCtrl();
    InitGpio();
    Init_IO();
    Init_SRAM();
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
    ConfigCpuTimer(Timer, Freq, Period)
    Init_McBSPbInterrupt(&MSBR_isr);
    Init_butInterrupt(&BUT1_isr,&BUT2_isr,&BUT3_isr);
   //Enable group 1 interrupts
    EDIS;
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer1, 200, 1000000);
    EINT;  // Enable Global interrupt INTM
    circularBuffer * audio = audioBuffer(10,46.875);
    while(1){
        displayLed(audioFlags>>1);
            switch(audioFlags){
                case audioReady:
                    GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
                    audioOut = audioIn;
                    audioFlags &= ~audioReady;
                    clearReverb(reverbBuff);
                    audio->wIndex=0;
                    audio->rIndex=0;
                    GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
                    break;
                case audioReady|Echo:
                    echo(audio,0.4);
                    echocheck(2,46.7);
                    break;
                case audioReady|Reverb:
                    sreverb(audio, 0.3);
                    sreverbcheck(7,46.7);
                    break;
                case audioReady|Playing:
                    play(audio);
                    break;
            }
   }
}
__interrupt void MSBR_isr(void)
{
        // right channel transmission
       interruptStore=IER;
       IER &= M_INT6;
       interruptStore=IER;
          IER &= M_INT6;
          channel1 = McbspbRegs.DRR1.all;
          channel2 = McbspbRegs.DRR2.all;
          audioIn = channel1;
          McbspbRegs.DXR1.all = audioOut;
          McbspbRegs.DXR2.all = audioOut;
          audioFlags|=audioReady;
          PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
          IER|=interruptStore;
       audioFlags|=audioReady;
       PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
       IER|=interruptStore;
}
__interrupt void BUT1_isr(void)
{
    audioFlags ^= Echo;
    fxCounter = 0;
    echoflag = 0;
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}
__interrupt void BUT2_isr(void)
{
    audioFlags ^= Reverb;
    fxCounter = 0;
    reverbflag =0;
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;

}
__interrupt void BUT3_isr(void)
{
    audioFlags |= Playing;
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP12;
}
