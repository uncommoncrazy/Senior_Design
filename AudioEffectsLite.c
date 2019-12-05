/*
 * AudioEffectsLite.c
 *
 *  Created on: Dec 1, 2019
 *      Author: Marco
 */

#include "AudioEffectsLite.h"

Uint16 AudioBuffer[AudioBuffSize] = {0};
Uint16 ReverbBuffer[ReverbSize] = {0};
Uint16 AudioIsReady=0, ReverbDelay=0, BufferIndex=0, AudioBufferCtrl=0,
        BufferFilled=0, ReverbFilled = 0, channel1 = 0, channel2 = 0;
int16  AudioOut=0;
float ReverbDecay = 0.0,a = 0.0, masterVolume=0.0;
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
    if(!(BufferIndex&0x3fff))
          ReverbFilled=1;
    if(!(BufferIndex&0x7fff)){
        BufferIndex=AudioBuffSize-1;
        BufferFilled=1;
    }


    ReverbBuffer[BufferIndex&0x3fff]=audioSample;
    AudioBuffer[BufferIndex--]=audioSample;
}
void reverb(){
    float x = (int16)(ReverbBuffer[(BufferIndex+1)&0x3fff]);
    Uint16 n = (BufferIndex+ReverbDelay)&0x3fff;

    float xd = (int16) ReverbBuffer[n];
    AudioOut = (int16)(1.2*(a*x + xd*ReverbDecay));
};
void AmpModulation(){
    float message = ((float)((int16)(channel2)))/0x3fff;
    AudioOut = (float)(channel1)*(message);

}
void PhaseModulation(){
    int16 message = ((int16)(channel2))/4;
    int phase =(BufferIndex+PhaseOffset+message)&0x7fff;
    AudioOut = AudioBuffer[phase];
}

