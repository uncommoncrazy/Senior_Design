/*
 * audioFX.c
 *
 *  Created on: Mar 5, 2019
 *      Author: Claudine
 */
#include "audioFX.h"
Uint32  fxCounter=0;
Uint16  reverbflags = 0;
Uint16  reverbflag = 0;
Uint16 numberOfReverbs=0;
int16 DMA_audioIn[2] = {0};
int16 *audioInput = &DMA_audioIn;
int16 *audioOutput = &audioOut;


Uint16 echoflag = 0;
circularBuffer * audioBuffer(double timeSec, double sampleRatekHz){
    circularBuffer * audio =  (circularBuffer *)far_malloc(sizeof( circularBuffer));
    audio->rIndex=0;
    audio->wIndex=0;
    audio->SRAMaddress=0;
    Uint32 samples=(Uint32)(timeSec*sampleRatekHz*1000);
    audio->size =(samples< bufferLimit)? samples : bufferLimit;
    audio->buffer = new_TransferBuffer(1);
    return audio;
}
void record( circularBuffer * audioStream){
    // check if any writes left to buffer
       GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
       if(wr(audioStream, audioIn)>=audioStream->size){
           audioFlags &= ~Rec;
           audioStream->wIndex = 0;
       }
       audioOut = audioIn;
       GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
       audioFlags &= ~audioReady;

}
void mixRecord( circularBuffer * audioStream){
      GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;

      int16 mix = (int16)rd(audioStream);
       //mix = ((float)((int16)audioIn)+mix)/2.0;
       mix+=audioIn;
       if(wr(audioStream,(Uint16)(mix/2))>=audioStream->size){
           audioFlags &= ~MixRec;
           audioStream->wIndex = 0;
       }
       audioOut = (Uint16)((int16) mix);
       GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;

       audioFlags &= ~audioReady;
}
void play(circularBuffer * audioStream){
     GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
     if(audioStream->rIndex>=audioStream->size){
            audioFlags &= ~Playing;
            audioStream->rIndex = 0;
     }
       audioOut = rd(audioStream);
       GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;


       audioFlags &= ~audioReady;
}
int16 mix(circularBuffer * audioBuffers, Uint16 ammount){
    float mix = 0;
    for( Uint16 stream = 0; stream<ammount; stream++)
        mix += (int16)circ_read(audioBuffers+stream);
    mix /= (float)(ammount);
    return (int16)mix;

}
void interpolate(circularBuffer * audioSample, int16 samples, int16 * buffer ){
    *buffer = rd(audioSample);
    int16 nextSample = SRAM_Rd(audioSample->SRAMaddress+audioSample->rIndex);
    float slope = (float)(nextSample-*buffer)/(float)(samples+1);
    float sample = *buffer;
    for(int16 newSample=1; newSample <= samples; newSample++){
        sample += slope;
        buffer[newSample]=(int16)(sample);
    }
}
void interpolation(circularBuffer * audioSample, int16 samples, int16 * buffer){

    if(audioSample->rIndex>=audioSample->size){
        audioFlags &= ~Interpolate;
        audioSample->rIndex = 0;
        fxCounter = 0;
    }
    if(!(fxCounter%(samples+1)))
          interpolate(audioSample, samples, buffer);
    audioOut = buffer[fxCounter%(samples+1)];
    audioFlags &= ~audioReady;
    fxCounter++;
}
void decimation(circularBuffer * audioSample, int16 samples){

    if(audioSample->wIndex>=audioSample->size){
        audioFlags &= ~Decimate;
        audioSample->wIndex = 0;
        fxCounter = 0;
    }
    if(!(fxCounter%(samples+1))){
        wr(audioSample, audioIn);
    }
    audioOut = audioIn;
    audioFlags &= ~audioReady;
    fxCounter++;
}
circularBuffer * reverbBuffer (Uint16 * ms10Delays, float sampleRatekHz ){
    circularBuffer * reverbBuff =  (circularBuffer *)far_malloc(sizeof( circularBuffer)*numberOfReverbs);
    for(Uint16 i=0; i < numberOfReverbs; i++){
            (reverbBuff+i)->rIndex=0;
            (reverbBuff+i)->wIndex=0;
            (reverbBuff+i)->SRAMaddress=0;
            (reverbBuff+i)->size =(Uint32)(sampleRatekHz*1000);
            *(ms10Delays+i)= (Uint16)(0.01*sampleRatekHz*1000*(*(ms10Delays+i)));
    }
    return reverbBuff;
}
void clearReverb(circularBuffer * reverbBuff){
    for(Uint16 i=0; i < numberOfReverbs; i++){
        (reverbBuff+i)->rIndex=0;
        (reverbBuff+i)->wIndex=0;

    }
}
void looprReverb(circularBuffer * reverbBuff){
    for(Uint16 i=0; i < numberOfReverbs; i++){
        if((reverbBuff+i)->rIndex>=(reverbBuff+i)->size)
            (reverbBuff+i)->rIndex=0;
        if((reverbBuff+i)->wIndex>=(reverbBuff+i)->size)
                (reverbBuff+i)->wIndex=0;
    }
}

void reverb(circularBuffer * reverbBuff ,float decay){
    audioOut = audioIn;

    wr(reverbBuff, audioIn);

    for(Uint16 i=0; i < numberOfReverbs; i++){
        if(reverbflags&(1<<i)){
            audioOut = (int16)(((1.0 - decay)*(float)((int16)audioOut)) + ((float)((int16)rd(reverbBuff+i))*decay));

        }
    }
    looprReverb( reverbBuff);
    audioFlags &= ~audioReady;
    fxCounter++;
}
void sreverb(circularBuffer * audio, float decay){
    audioOut = audioIn;
    wr(audio, audioIn);
    float a = 1.0 - decay;
    float x= (int16)audioOut;
       if(reverbflag){
           float xd=(int16)rd(audio);
           audioOut = (int16)(a*x + xd*decay);
       }

       if(audio->wIndex>=audio->size)
           audio->wIndex=0;
       if(audio->rIndex>=audio->size)
           audio->rIndex=0;
       audioFlags &= ~audioReady;
       fxCounter++;
}
void sreverbcheck(Uint16 delay,float sampleRatekHz  ){
    Uint32 delayamnt = (Uint32) (0.01*sampleRatekHz*1000.0*(float)delay);
    if(fxCounter>=delayamnt)
        reverbflag=1;

}

void reverbcheck(Uint16 * delays){
    for(Uint16 i=0; i < numberOfReverbs; i++){
           if(fxCounter>=*(delays+i))
               reverbflags|=1<<i;
       }

}
void echo(circularBuffer * audio, float decay){
    audioOut = audioIn;
    float a = 1.0 - decay;
    float x= (int16)audioOut;
       if(echoflag){
           float y=(int16)rd(audio);
           audioOut = (int16)(a*x + y*decay);
       }
       wr(audio, audioOut);
       if(audio->wIndex>=audio->size)
           audio->wIndex=0;
       if(audio->rIndex>=audio->size)
           audio->rIndex=0;
       audioFlags &= ~audioReady;
       fxCounter++;
}
void echocheck(Uint16 delay,float sampleRatekHz  ){
    Uint32 delayamnt = (Uint32) (sampleRatekHz*250.0*(float)delay);
    if(fxCounter>=delayamnt)
        echoflag=1;

}
void overDrive(){
    float y;
    float x = ((int16)(audioIn))/32768.0;
    float abs;
    float threshHold =(1.0/3.0);
    if(x>=0){
        abs = x;
    }else{
        abs = -x;
    }
    if(threshHold>abs){
        y = 2.0*x;
    }
    if(abs>=threshHold){
        if(x>=0){
            y = ((3.0-(2.0-x*3.0))*(3.0-(2.0-x*3.0)))/3.0;
        }else{
            y = -((3.0-(2.0-x*3.0))*(3.0-(2.0-abs*3.0)))/3.0;
        }
    }
    if(abs>(2.0*threshHold)){
        if(x>=0){
            y = 1.0;
        }else{
            y = -1.0;
        }
    }
    y = y*32768.0*4.0;
   audioOut = (Uint16)((int16)(y));
   audioFlags &= ~audioReady;

}
void dma_streamAudio()
{
    //
    // Refer to dma.c for the descriptions of the following functions.
    //

    //
    //Initialize DMA
    //

    volatile int16 * DMA6Source = (volatile Uint16 *)&McbspbRegs.DRR1;
    volatile int16 * DMA6Dest =  (volatile Uint16 *)&McbspbRegs.DXR2;
  /*volatile int16 * DMA5Source = audioInput+1;
    volatile int16 * DMA5Dest = (volatile Uint16 *)&McbspbRegs.DXR2;
*/
    //
    // configure DMA CH6
    //
    DMACH6AddrConfig(DMA6Dest,DMA6Source);
    DMACH6BurstConfig(1,0,1);
    DMACH6TransferConfig(0,1,1);
    DMACH6ModeConfig(DMA_MREVTB,PERINT_ENABLE,ONESHOT_DISABLE,CONT_DISABLE,
                     SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,
                     CHINT_END,CHINT_ENABLE);
    EALLOW;
    CpuSysRegs.SECMSEL.bit.PF2SEL=1;
    EDIS;
    EnableInterrupts();
    StartDMACH6();

//    StartDMACH5();

}/*
void dma_AudioInOut()
{
    //
    // Refer to dma.c for the descriptions of the following functions.
    //

    //
    //Initialize DMA
    //
    DMAInitialize();

    volatile int16 * DMA6Source = (volatile Uint16 *)&McbspbRegs.DRR2;
    volatile int16 * DMA6Dest = audioInput;
    volatile int16 * DMA5Source = audioOutput;
    volatile int16 * DMA5Dest = (volatile Uint16 *)&McbspbRegs.DXR2;

    //
    // configure DMA CH6
    //
    DMACH6AddrConfig(DMA6Dest,DMA6Source);
    DMACH6BurstConfig(1,1,1);
    DMACH6TransferConfig(0,1,1);
    DMACH6ModeConfig(DMA_MREVTB,PERINT_ENABLE,ONESHOT_DISABLE,CONT_ENABLE,
                     SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,
                     CHINT_END,CHINT_ENABLE);
    DMACH5AddrConfig(DMA5Dest,DMA5Source);
    DMACH5BurstConfig(1,0,1);
    DMACH5TransferConfig(0,1,1);
    DMACH5ModeConfig(DMA_MXEVTB,PERINT_ENABLE,ONESHOT_DISABLE,CONT_ENABLE,
                        SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,SIXTEEN_BIT,
                        CHINT_END,CHINT_ENABLE);
    EALLOW;
    CpuSysRegs.SECMSEL.bit.PF2SEL=1;
    EDIS;
    EnableInterrupts();
    StartDMACH6();
    StartDMACH5();

}
*/
//void mix(circularBuff buffer);
//void reverb(circularBuff buffer, Uint32 amount);
//Svoid echo(circularBuff buffer, Uint32 delay, float feedback);
