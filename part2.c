

/**
 * part2.c
 */
/*Descriptions
 * This codes uses dma to accomplish Double buffering to input and ouput audio while performing DFTs
 *on ready to be processed buffers
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
__interrupt void local_D_INTCH4_ISR(void);
float freq = 0.0;
float mag = 0.0;
int main(void)
{
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
    EDIS;
    DMAInitialize();
    dma_AudioBuffers();
    InitSPIA();
    InitMcBSPb();
    InitAIC23();
    generateTables();
    while(1){
        switch(audioFlags&DFTflag){
           case (DFTflag):
               calculateDFT();
               freq=findMax(46875.0);
               mag = getMaxMag();
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
    dma_bufferSwap();
}
