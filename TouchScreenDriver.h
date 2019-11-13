/*
 * TouchScreenDriver.h
 *
 *  Created on: Oct 7, 2019
 *      Author: Claudine
 */

#ifndef TOUCHSCREENDRIVER_H_
#define TOUCHSCREENDRIVER_H_
#include "OneToOneI2CDriver.h"
#define FT62XX_ADDR           0x38
#define FT62XX_G_FT5201ID     0xA8
#define FT62XX_REG_NUMTOUCHES 0x02

#define FT62XX_NUM_X             0x33
#define FT62XX_NUM_Y             0x34

#define FT62XX_REG_MODE 0x00
#define FT62XX_REG_CALIBRATE 0x02
#define FT62XX_REG_WORKMODE 0x00
#define FT62XX_REG_FACTORYMODE 0x40
#define FT62XX_REG_THRESHHOLD 0x80
#define FT62XX_REG_POINTRATE 0x88
#define FT62XX_REG_FIRMVERS 0xA6
#define FT62XX_REG_CHIPID 0xA3
#define FT62XX_REG_VENDID 0xA8

#define FT62XX_VENDID 0x11
#define FT6206_CHIPID 0x06
#define FT6236_CHIPID 0x36
#define FT6236U_CHIPID 0x64
extern Uint16 touches;
extern Uint16 touchX[2], touchY[2], touchID[2];
extern Uint16 TSreadBuff[16], TSwriteBuff[4];
// calibrated for Adafruit 2.8" ctp screen
#define FT62XX_DEFAULT_THRESHOLD 128
typedef struct point_t{
    int16 x,y;
} TouchPoint;
Uint16 TS_init(Uint16 thresh);
Uint16 touched();
TouchPoint getTouchPoint();
Uint16 TS_readRegister8(Uint16 r);
void TS_writeRegister8(Uint16 r, Uint16 d);
void readData();
#endif /* TOUCHSCREENDRIVER_H_ */
