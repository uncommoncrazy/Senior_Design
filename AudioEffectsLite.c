/*
 * AudioEffectsLite.c
 *
 *  Created on: Dec 1, 2019
 *      Author: Marco
 */

#include "AudioEffectsLite.h"

Uint16 AudioBuffer[AudioBuffSize] = {0};
Uint16 ReverbBuffer[0x1000] = {0};
Uint16 AudioIsReady=0, ReverbDelay=0, BufferIndex=0, AudioBufferCtrl=0,
        BufferFilled=0, ReverbFilled = 0, channel1 = 0, channel2 = 0;
int16  AudioOut=0;
float ReverbDecay = 0.0,a = 0.0, AM =1;
void setReverbDelay(float delay){
    if(delay>1.0){
        ReverbDelay = AudioBuffSize;
    }else{
        ReverbDelay = 46700*delay;
    }
};
void setReverbDecay(float decay){;
    a = 1.0 - decay;
    ReverbDecay = decay;
};
void record(Uint16 audioSample){
    if(!(BufferIndex&0x7fff)){
        BufferIndex=AudioBuffSize-1;
        BufferFilled=1;
    }
    if(!(BufferIndex&0xfff))
        ReverbFilled=1;

    ReverbBuffer[BufferIndex&0xfff]=audioSample;
    AudioBuffer[BufferIndex--]=audioSample;
}
void reverb(){
    float x = (int16)ReverbBuffer[BufferIndex+1];
    Uint16 n = BufferIndex+ReverbDelay;
    if(n>=AudioBuffSize)n-=AudioBuffSize;
    float xd = (int16) AudioBuffer[n];
    AudioOut = (int16)(1.2*(a*x + xd*ReverbDecay));
};
void AmpModulation(){
    float message = ((int16)(channel2))/0xffff;
    AudioOut = (float)(channel1)*(1+messager)*0.5;

}
void PhaseModulation(){
    int16 message = ((int16)(channel2))/2;
    int phase =(bufferIndex+PhaseOffset+message)&0x7fff);
    AudiOut = AudioBuffer[phase];
}

