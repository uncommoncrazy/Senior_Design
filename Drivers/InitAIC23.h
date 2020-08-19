/*
 * InitAIC23.h
 *
 *  Created on: Feb 24, 2019
 *      Author: Claudine
 */

#ifndef INITAIC23_H_
#define INITAIC23_H_
#define CodecSPI_CLK_PULS {EALLOW; GpioDataRegs.GPASET.bit.GPIO18 = 1; GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;}
#define CodecSPI_CS_LOW {EALLOW; GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;}
#define CodecSPI_CS_HIGH {EALLOW; GpioDataRegs.GPASET.bit.GPIO19 = 1;}
#define volumeMax  0x7f
#define volDefault  0x79
#define volumeMute  0x30;
#define volumeSteps 79;
void SmallDelay();

void InitMcBSPb();
void Init_McBSPbInterrupt(void(*Msbr_isr));

void InitSPIA();
void InitAIC23();
void SpiTransmit(uint16_t data);

void InitBigBangedCodecSPI();
void BitBangedCodecSpiTransmit(Uint16 data);



#endif /* INITAIC23_H_ */
