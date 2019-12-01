/*
 * AudioEffectsLite.c
 *
 *  Created on: Dec 1, 2019
 *      Author: Marco
 */

#include "AudioEffectsLite.h"

Uint16 AudioBuffer[AudioBuffSize] = {0};
Uint16 AudioIsReady=0,ReverbDelay=0,BufferIndex=0,AudioBufferCtrl=0,BufferFilled=0,LeftIn=0,RightIn=0;
int16  AudioOut=0;
float ReverbDecay = 0.0,a = 0.0;
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
    if(!BufferIndex){
        BufferIndex=AudioBuffSize-1;
        BufferFilled=1;
    }
    AudioBuffer[BufferIndex--]=audioSample;
}
void reverb(){
    float x = (int16)AudioBuffer[BufferIndex+1];
    Uint16 n = BufferIndex+ReverbDelay;
    if(n>=AudioBuffSize)n-=AudioBuffSize;
    float xd = (int16) AudioBuffer[n];
    AudioOut = (int16)(1.2*(a*x + xd*ReverbDecay));
};

;
