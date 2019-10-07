#ifndef LCDDRIVER_H_
#define LCDDRIVER_H_
#include "OneToOneI2CDriver.h"
#define     upperNib    0xf0
#define     lowerNib    0x0f
#define     EnableHi    0x0C
#define     EnableLo    0x08
#define     RS          0x01
#define     RSEnableHi  0x0C | RS
#define     RSEnableLo  0x08 | RS
#define     LCD_init1   0x33
#define     LCD_init2   0x32
#define     Mode        0x28
#define     DCP         0x0f
#define     ClearDisp   0x01
#define     Line2       0xC0
#define     Home        0x02
#define     LCD_addr    0x27
Uint16 * LCDBuffer(void);
void LCD_Command(Uint16 Command, Uint16 * const Buffer);
void LCD_float(float decimal, Uint16 * const Buffer);
void LCD_Data(char data, Uint16 * const Buffer);
void Init_LCD( Uint16 * const Buffer);
void LCD_String(char * str, Uint16 * const Buffer);
void LCD_thousandsfloat(float decimal, Uint16 * const Buffer);
#endif 
