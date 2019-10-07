/*
 * DFTLib.h
 *
 *  Created on: Mar 30, 2019
 *      Author: Claudine
 */

#ifndef DFTLIB_H_
#define DFTLIB_H_
#include "audioFx.h"
#include "math.h"
#include "string.h"
#include "FPU.h"
#define Stages      8
#define DFTsize     256
#define DFTflag     1<<9
#define TWO_PI      (2.0*3.14159265358979323846)
#ifdef __cplusplus
#pragma DATA_SECTION("RFFTIn")
#else
#pragma DATA_SECTION(RFFT_InBuff,"RFFTIn")
#endif
extern float RFFT_InBuff[512];
#ifdef __cplusplus
#pragma DATA_SECTION("RFFTOut")
#else
#pragma DATA_SECTION(RFFT_OutBuff,"RFFTOut")
#endif
extern float RFFT_OutBuff[512];
#ifdef __cplusplus
#pragma DATA_SECTION("RFFTTwiddle")
#else
#pragma DATA_SECTION(RFFT_Twiddle,"RFFTTwiddle")
#endif
extern float RFFT_Twiddle[512];
#ifdef __cplusplus
#pragma DATA_SECTION("RFFTMag")
#else
#pragma DATA_SECTION(RFFT_Mag,"RFFTMag")
#endif
extern float RFFT_Mag[256];
#ifdef __cplusplus
#pragma DATA_SECTION("Audio")
#else
#pragma DATA_SECTION(audio1,"Audio")
#endif
extern int16 audio1[512];
#ifdef __cplusplus
#pragma DATA_SECTION("Audio")
#else
#pragma DATA_SECTION(audio2,"Audio")
#endif
extern int16 audio2[512];
#ifdef __cplusplus
#pragma DATA_SECTION("DFT")
#else
#pragma DATA_SECTION(sinT,"DFT")
#endif
extern float sinT[512];
#ifdef __cplusplus
#pragma DATA_SECTION("DFT")
#else
#pragma DATA_SECTION(cosT,"DFT")
#endif
extern float cosT[512];
#ifdef __cplusplus
#pragma DATA_SECTION("DFT")
#else
#pragma DATA_SECTION(DTF1,"DFT")
#endif
extern float DFT1[512];
#ifdef __cplusplus
#pragma DATA_SECTION("DFT")
#else
#pragma DATA_SECTION(DFT2,"DFT")
#endif
extern float DFT2[512];

extern float cosT[512];
extern RFFT_F32_STRUCT rfft;

extern int16 * processAudio;
extern int16 * audioInBuff;
extern float * processDFT;
extern float * DFT;
extern float XkReal;
extern float XkImg;
extern Uint16 DSPCounter;
extern Uint16 AudioCounter;

void generateTables();
void calculateDFT();
void clearDFT();
void dma_AudioBuffers();
void dma_bufferSwap();
void init_fft(Uint16 stages);
void copytofloat(int16 * source, float * destination, Uint16 size);
void fft();
float findMax(float sampleRate);
float getMaxMag();
float Maxfreq(float sampleRate);
float MaxMag();
#endif /* DFTLIB_H_ */
