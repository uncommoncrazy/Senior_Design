/*
 * AudioEffectsLite.h
 *
 *  Created on: Dec 1, 2019
 *      Author: Marco
 */

#ifndef AUDIOEFFECTSLITE_H_
#define AUDIOEFFECTSLITE_H_
#include<F28x_Project.h>

#define AudioBuffSize 0xc000
#define Audio_Out 0
#define Audio_Left 1
#define Audio_Right 2
#define ADSRGate    GpioDataRegs.GPADAT.bit.GPIO25

#ifdef __cplusplus
#pragma DATA_SECTION("DelayLine")
#else
#pragma DATA_SECTION(AudioBuffer,"DelayLine")
#endif

extern Uint16 AudioBuffer[AudioBuffSize];
extern Uint16 AudioIsReady,ReverbDelay,BufferIndex,AudioBufferCtrl,BufferFilled,LeftIn,RightIn;
extern int16  AudioOut;
extern float ReverbDecay,a;
void setReverbDelay(float delay);
void setReverbDecay(float decay);
void reverb();
#endif /* AUDIOEFFECTSLITE_H_ */
