/*
 * TFTLCD_Driver.h
 *
 *  Created on: Oct 5, 2019
 *      Author: Claudine
 */

#ifndef TFTLCD_DRIVER_H_
#define TFTLCD_DRIVER_H_
#include "IODriver.h"

void startLCD();
void resetLCD();
void drawPixel(int16 x, int16 y, Uint16 color);
void drawHorzLine(int16 x, int16 y, int16 length, Uint16 color);
void drawVertLine(int16 x, int16 y, int16 length,Uint16 color);
void fillRect(int16 x, int16 y, int16 w, int16 h, Uint16 color);
void fillScreen(Uint16 color);
void setRegister8(Uint16 *ptr, Uint16 value);
void setRegister16(Uint16 *ptr, Uint16 value);
void setRotation(Uint16 x);
void setWindow(int32 x1, int32 y1, int32 x2, int32 y2);
void pushColors(Uint16 *data, Uint16 len, Uint16 first);
Uint16 genColor(Uint16 r, Uint16 g, Uint16 b);
void writeRegister24(Uint16 a, Uint32 d);
void writeRegister32(Uint16 a, Uint32 d);
Uint16 readColor(int16 x, int16 y);
#endif
