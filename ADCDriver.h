/*
 * ADCDriver.c
 *
 *  Created on: Feb 23, 2019
 *      Author: Claudine
 */

#ifndef ADCDRIVER_C_
#define ADCDRIVER_C_
#define Resolution      3.3/(4095.0)
#define trResolution   3.3/(128.0)
#define trashBits       5
void Init_ADC(void);
void Init_ADCInterrupt(void(*adc_isr));
void SetupADCSoftware(void);
void SetupADCTimer1(void);
float convertADC(Uint16 adcValue);
float trConvertADC(Uint16 adcValue);
#endif /* ADCDRIVER_C_ */
