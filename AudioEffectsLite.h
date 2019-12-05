/*
 * AudioEffectsLite.h
 *
 *  Created on: Dec 1, 2019
 *      Author: Marco
 */

#ifndef AUDIOEFFECTSLITE_H_
#define AUDIOEFFECTSLITE_H_
#include<F28x_Project.h>

#define AudioBuffSize 0x8000
#define PhaseOffset 0x4000
#define Audio_Out 0
#define Audio_Left 1
#define Audio_Right 2
#define ADSRGate    GpioDataRegs.GPADAT.bit.GPIO25
#define FX_Mixer 0
#define FX_Reverb 1
#define FX_AM 2
#define FX_PM 3
#ifdef __cplusplus
#pragma DATA_SECTION("DelayLine")
#else
#pragma DATA_SECTION(AudioBuffer,"DelayLine")
#endif
extern Uint16 AudioBuffer[AudioBuffSize];
#ifdef __cplusplus
#pragma DATA_SECTION("Reverb")
#else
#pragma DATA_SECTION(ReverbBuffer,"Reverb")
#endif
extern Uint16 ReverbBuffer[0x1000];
extern Uint16 AudioIsReady,ReverbDelay,BufferIndex,AudioBufferCtrl,BufferFilled,LeftIn,RightIn;
extern int16  AudioOut;
extern Uint16 channel1,channel2;
extern float ReverbDecay,a;
void setReverbDelay(float delay);
void setReverbDecay(float decay);
void reverb();
void AmpModulation();
void PhaseModulation();
#endif /* AUDIOEFFECTSLITE_H_ */
