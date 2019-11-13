/*
 * audioFX.h
 *
 *  Created on: Mar 5, 2019
 *      Author: Claudine
 */

#ifndef AUDIOFX_H_
#define AUDIOFX_H_
#include "BufferLib.h"
#define audioBuffSize 10
#define audioReady 1
#define Rec 1<<1
#define Playing 1<<2
#define OverDrive 1<<8;
#define MixRec 1<<3
#define Interpolate 1<<4
#define Decimate 1<<5
#define Reverb 1<<6
#define Echo 1<<7
#define channel 0x8000

extern Uint16 audioFlags;
extern Uint16 audioIsReady;
extern Uint16 recordFlag;
extern Uint16 echoflag;
extern Uint16 reverbflag;
extern Uint32  fxCounter;
extern Uint16 intrpStatus;
extern int16 DMA_audioIn[2];

extern int16 audioIn;
extern int16 *audioInput;
extern int16 *audioOutput;
extern Uint16 audioOut;
extern Uint16 reverbflags;
extern Uint16 numberOfReverbs;
// record data and send to sram
circularBuffer * audioBuffer(double timeSec, double sampleRatekHz);
void record(circularBuffer * buffer);
//void mix(circularBuff buffer);
//void reverb(circularBuff buffer, Uint32 amount);
//void echo(circularBuff buffer, Uint32 amount);
void mixRecord( circularBuffer * audioStream);
void play(circularBuffer * buffer);
void interpolate(circularBuffer * audioSample, int16 samples, int16 * buffer );
int16 mix(circularBuffer * audioBuffers, Uint16 ammount);
void interpolation(circularBuffer * audioSample, int16 samples, int16 * buffer);
void decimation(circularBuffer * audioSample, int16 samples);
void clearReverb(circularBuffer * reverbBuff);
void looprReverb(circularBuffer * reverbBuff);
void sreverb(circularBuffer * audio ,float decay);
void sreverbcheck(Uint16 delay,float sampleRatekHz);
circularBuffer * reverbBuffer (Uint16 * ms10Delays, float sampleRatekHz );
void reverb(circularBuffer * reverbBuff ,float decay);
void echo(circularBuffer * audio, float decay);
void reverbcheck(Uint16 * delays);
void echocheck(Uint16 delay,float sampleRatekHz );
void dma_streamAudio();
void overDrive();
#endif /* AUDIOFX_H_ */
