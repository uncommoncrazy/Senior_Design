/*
 * DFTLib.c
 *
 *  Created on: Mar 30, 2019
 *      Author: Marco Munoz
 */
#include "DFTLib.h"
//sine and cos time tables
float sinT[512] = {0.0};
float cosT[512]= {0.0};
//double buffer dft output
float DFT1[512]={0.0};
float DFT2[512]={0.0};
// double buffer audio
int16 audio1[512]={0};
int16 audio2[512]={0};
// buffer for ti fft functions
float RFFT_InBuff[512]={0.0};
float RFFT_OutBuff[512]={0.0};
float RFFT_Twiddle[512]={0.0};
float RFFT_Mag[256]={0.0};
// pointers to audio buffers
int16 * processAudio =audio1;
int16 * audioInBuff = audio2;
// pointers to DFT buffers
float * processDFT =DFT1 ;
float * DFT = DFT2;
//variables for DFT calulations
float XkReal = 0.0;
float XkImg = 0.0;
Uint16 DSPCounter = 0;
Uint16 AudioCounter = 0;
// RFFT struct for fft calculations
RFFT_F32_STRUCT rfft;
//fill cose& sine table for DFT calculations
void generateTables(){
    for(int n = 0; n < DFTsize; n++){
        sinT[n] = sinf((TWO_PI*(float)n)/(float)DFTsize);
        cosT[n] = cosf((TWO_PI*(float)n)/(float)DFTsize);
    }
}
// perform DFT calculation on process audio buffer
void calculateDFT(){
    Uint16 theta;
    int16 * currentAudio = processAudio;
    GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
    for(int k = 0; k<DFTsize/2;k++){
        XkReal = 0.0;
        XkImg = 0.0;
        for(int n = 0; n<DFTsize; n++){
            theta = (k*n)&(DFTsize-1);
            XkReal += (float)(currentAudio[n])*sinT[theta];
            XkImg  += (float)(currentAudio[n])*cosT[theta];
        }
        processDFT[k] =  sqrtf(XkReal*XkReal + XkImg*XkImg);

    }
    GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
    clearDFT();
    audioFlags &= ~DFTflag;
}
// fill audioin buffer if full swap buffers
void loadAudio(){
    float * temp = audioInBuff;
    if(AudioCounter&DFTsize){
        AudioCounter=0;
        audioInBuff = processAudio;
        processAudio = temp;
        GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1;
        // trigger dft flag since buffer is full
        audioFlags|=DFTflag;
    }else{
        audioInBuff[AudioCounter++]=audioIn;
    }


}
// swap DFT output buffer
void clearDFT(){
    float * temp = processDFT;
    processDFT = DFT;
    DFT = temp;
    DSPCounter = 0;
}

// set up dma write and read from audio dma_AudioBuffers
void dma_AudioBuffers()
{
    volatile int16 * DMA6Source =  (volatile Uint16 *)&McbspbRegs.DRR1;
    volatile int16 * DMA6Dest = audioInBuff;
    volatile int16 * DMA5Source =  processAudio;
    volatile int16 * DMA5Dest = (volatile Uint16 *)&McbspbRegs.DXR2;
    //DMA from audio input Mcbsp to Buffer
    DMACH6AddrConfig(DMA6Dest,DMA6Source);
    DMACH6BurstConfig(0,0,0);
    DMACH6TransferConfig(DFTsize,0,1);
    DMACH6ModeConfig(DMA_MREVTB,PERINT_ENABLE,ONESHOT_DISABLE,CONT_DISABLE,
                     SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,
                     CHINT_END,CHINT_ENABLE);
     //DMA from buffer to Mcbsp audio out
    DMACH5AddrConfig(DMA5Dest,DMA5Source);
    DMACH5BurstConfig(1,0,1);
    DMACH5TransferConfig(DFTsize,1,-1);
    DMACH5ModeConfig(DMA_MXEVTB,PERINT_ENABLE,ONESHOT_DISABLE,CONT_DISABLE,
                        SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,
                        CHINT_END,CHINT_DISABLE);
    EALLOW;
    CpuSysRegs.SECMSEL.bit.PF2SEL=1;
    EDIS;
    EnableInterrupts();
    StartDMACH6();
    StartDMACH5();
}
// initialize fft struct
void init_fft(Uint16 stages){
    rfft.FFTSize = (1<<stages);
    rfft.FFTStages = stages;
    rfft.InBuf = &RFFT_InBuff[0];
    rfft.OutBuf = &RFFT_OutBuff[0];
    rfft.CosSinBuf = &RFFT_Twiddle[0];
    rfft.MagBuf = &RFFT_Mag[0];
    RFFT_f32_sincostable(&rfft);
}
// copy and cast and interger array to a float array
void copytofloat(int16 * source, float * destination, Uint16 size){
    for(Uint16 i = 0; i<size ; i++)
        destination[i] = (float) source[i];
}
// perform fft calulation
void fft(){
    int16 * currentAudio = processAudio;
    GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
    copytofloat(currentAudio, &RFFT_InBuff[0],rfft.FFTSize);
    RFFT_f32(&rfft);
    RFFT_f32_mag(&rfft);
    GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
    audioFlags &= ~DFTflag;
}
// find frequency with highest amplitude from DFT calculation
float findMax(float sampleRate){
    Uint16 i = maxidx_SP_RV_2(DFT,DFTsize/2);
    return (float)i*(sampleRate/((float) DFTsize-1));
}
// get the highest magnitude from DFT calculation
float getMaxMag(){
    Uint16 i = maxidx_SP_RV_2(DFT,DFTsize/2);
    return 10*log10f(DFT[i]);
}
// find frequency with highest amplitude from FFT calculation

float Maxfreq(float sampleRate){
    int size = rfft.FFTSize;
    Uint16 i = maxidx_SP_RV_2(RFFT_Mag,size/2);
    return (float)i*(sampleRate/((float) size-1));
}
// get the highest magnitude from FFT calculation
float MaxMag(){
    int size = rfft.FFTSize;
    Uint16 i = maxidx_SP_RV_2(RFFT_Mag,size/2);
    return 10*log10f(RFFT_Mag[i]);;
}
// swap buffer when audio in is filled and start DMA tranfer again
void dma_bufferSwap()
{
    int16 * temp = audioInBuff;
    audioInBuff = processAudio;
    processAudio = temp;
    EALLOW;
    DmaRegs.CH6.DST_BEG_ADDR_SHADOW = (Uint32)audioInBuff;
    DmaRegs.CH6.DST_ADDR_SHADOW =     (Uint32)audioInBuff;
    DmaRegs.CH5.SRC_BEG_ADDR_SHADOW = (Uint32)processAudio;
    DmaRegs.CH5.SRC_ADDR_SHADOW =     (Uint32)processAudio;
    EDIS;
    StartDMACH6();
    StartDMACH5();

}
