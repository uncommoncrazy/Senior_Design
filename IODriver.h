/*
 * IODriver.h
 *
 *  Created on: Feb 26, 2019
 *      Author: Claudine
 */

#ifndef IODRIVER_H_
#define IODRIVER_H_
#include <F28x_Project.h>
#define     LCDDataPins     0x00ff
#define     nLCDDataPins    0xffffff00
// LCD control pins GPIOB GPIO: 32-RD, 40-WR, 41-CD, 52-CS
#define     LCDCtrlPins     0x00100301
#define     PBPins      0x0001C000
#define     SwitchPins  0x00000f00
#define     Switch      8
#define     PB          9
#define     PU          (PBPins|SwitchPins)
#define     LCD_Write_Off   GpioDataRegs.GPBDAT.bit.GPIO40=1
#define     LCD_Write_On    GpioDataRegs.GPBDAT.bit.GPIO40=0
#define     LCD_Read_Off    GpioDataRegs.GPBDAT.bit.GPIO32=1
#define     LCD_Read_On     GpioDataRegs.GPBDAT.bit.GPIO32=0
#define     LCD_CD_Data     GpioDataRegs.GPBDAT.bit.GPIO41=1
#define     LCD_CD_Command  GpioDataRegs.GPBDAT.bit.GPIO41=0
#define     LCD_CS_Off      GpioDataRegs.GPBDAT.bit.GPIO52=1
#define     LCD_CS_On       GpioDataRegs.GPBDAT.bit.GPIO52=0
#define     LCD_Delay       DELAY_US(0.38)
#define     STROBE_DELAY    DELAY_US(0.01)
#define     LCD_WR_Strobe   {LCD_Write_On;STROBE_DELAY;LCD_Write_Off;}
#define     LCD_DIR_Write   GpioCtrlRegs.GPADIR.all |= LCDDataPins
#define     LCD_DIR_RD  GpioCtrlRegs.GPADIR.all &= nLCDDataPins

void Init_IO(void);
void Init_butInterrupt(void(*Xi1ISR),void(*Xi2ISR),void(*Xi3ISR));
Uint16  ButtonPressed(Uint16* Current,Uint16* Last);
Uint16 getSwitches();
Uint16 getButton();
void Init_LCDPins();
void LCD_Write8(Uint16 Data);
Uint16 LCD_Read8();
void LCD_WriteReg8(Uint16 address, Uint16 data);
void LCD_WriteReg16(Uint16 address, Uint16 data);
void LCD_WriteRegPair(Uint16 address, Uint16 data);

#endif /* IODRIVER_H_ */
