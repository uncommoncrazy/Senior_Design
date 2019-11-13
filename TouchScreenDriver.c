/*
 * TouchScreenDriver.c
 *
 *  Created on: Oct 7, 2019
 *      Author: Claudine
 */
#include "TouchScreenDriver.h"
Uint16 touches = 9;
Uint16 touchX[2]={0}, touchY[2]={0}, touchID[2]={0};
Uint16 TSreadBuff[16]={0}, TSwriteBuff[4]={0};
Uint16 TS_init(Uint16 thresh){
    I2C_O2O_Master_Init(FT62XX_ADDR, 200.0,11.0);
    return 0;
}
Uint16 touched(){
   int t = TS_readRegister8(FT62XX_REG_NUMTOUCHES);
    if(t>2) return 0;
    return t;
}
TouchPoint getTouchPoint(Uint16 n){
    readData();
    TouchPoint point = {0,0};
     if ((touches == 0) || (n > 1)) {
       return point;
     } else {
       point.x=touchX[n];
       point.y=touchY[n];
       return point;
     }
}
Uint16 TS_readRegister8(Uint16 r){
   TSwriteBuff[0]=r;
 I2C_O2O_SendBytes(TSwriteBuff, 1);
 I2C_O2O_ReadBytes(TSreadBuff, 1);
 return TSreadBuff[0];
}
void TS_writeRegister8(Uint16 r, Uint16 d){
    TSwriteBuff[0]=r;
    TSwriteBuff[1]=d;
    I2C_O2O_SendBytes(TSwriteBuff, 2);
}
void readData(){
    TSwriteBuff[0]=0;
    I2C_O2O_SendBytes(TSwriteBuff, 1);
    I2C_O2O_ReadBytes(TSreadBuff, 16);
    touches = TSreadBuff[0x02];

    if ((touches > 2) || (touches == 0)) touches = 0;

    for (Uint16 i=0; i<2; i++) {
      touchX[i] = TSreadBuff[0x03 + i*6] & 0x0F;
      touchX[i] <<= 8;
      touchX[i] |= TSreadBuff[0x04 + i*6];
      touchY[i] = TSreadBuff[0x05 + i*6] & 0x0F;
      touchY[i] <<= 8;
      touchY[i] |= TSreadBuff[0x06 + i*6];
      touchID[i] = TSreadBuff[0x05 + i*6] >> 4;
    }

}
