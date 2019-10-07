

/**
 * main.c
 */
/*Descriptions
 * This codes use given driver to write name and course code on the lcd
 */
#include<F28x_Project.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCDDriver.h"
#include "ADCDriver.h"
#include "AIC23.h"
#include "IODriver.h"
#include "audioFX.h"
#include "DFTLib.h"

#define     left       0
#define     right       1
Uint16 audioFlags;
Uint16 audioIsReady;
Uint16 recordFlag;
Uint16 playFlag;
int16 audioIn;
Uint16 audioOut;
__interrupt void MSBR_isr(void);
Uint16 Buttons[3]={0,0,0};
Uint16  index = 0;
Uint16 adcSignal = 0;
int16 channel1=0;
int16 channel2=0;
Uint16 chanl=0;
Uint16  interruptStore=0;
#define BURST 1
#define TRANSFER 0

volatile int16 *DMA_CH6_Dest;
volatile int16 *DMA_CH6_Source;
__interrupt void local_D_INTCH4_ISR(void);

int main(void)
{
    float frequency = 0.0;
    Uint16 * const I2C_Data = LCDBuffer();
    InitSysCtrl();
    InitGpio();
    Init_IO();
    Init_LCD(I2C_Data);
    Init_SRAM();

    EALLOW;
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EALLOW;
    PieVectTable.DMA_CH6_INT= &local_D_INTCH4_ISR;
    PieCtrlRegs.PIEIER7.bit.INTx6 = 1;   // Enable PIE Group 7, INT 4 (DMA CH4)
    IER |= M_INT7;
    //Init_McBSPbInterrupt(&MSBR_isr);
   //Enable group 1 interrupts
    EDIS;
    DMAInitialize();
    dma_AudioBuffers();
    //dma_streamAudio();
    InitSPIA();
    InitMcBSPb();
    InitAIC23();

    //dma_streamAudio();
    //generateTables();
    //audioFlags |= DFTflag;
    init_fft(Stages);
    while(1){
        switch(audioFlags&DFTflag){
           case (DFTflag):
               fft();
               frequency = Maxfreq(46875.0);
               break;
       }
    }


}
__interrupt void local_D_INTCH4_ISR(void)
{
    EALLOW;

    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP7; // ACK to receive more interrupts
                                            // from this PIE group
    EDIS;
    GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1;
    audioFlags|=DFTflag;
    //restart DMA -> Dave added this :}
    dma_bufferSwap();
    //StartDMACH6();

}
/*__interrupt void MSBR_isr(void)
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
          loadAudio();
          PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
          IER|=interruptStore;
       audioFlags|=audioReady;
       PieCtrlRegs.PIEACK.all |= PIEACK_GROUP6;
       IER|=interruptStore;
}
*/
