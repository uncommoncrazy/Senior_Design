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
TouchPoint TS_Position = {0,0};
Uint16 TS_init(Uint16 thresh){
    I2C_O2O_Master_Init(FT62XX_ADDR, 200.0,11.0);
    TS_writeRegister8(FT62XX_REG_THRESHHOLD, 40);
  //  if(TS_readRegister8(FT62XX_REG_VENDID)!=FT62XX_VENDID) return 0;
    Uint16 ID = TS_readRegister8(FT62XX_REG_CHIPID);
    if((ID!=FT6206_CHIPID)&&(ID!=FT6236U_CHIPID)&&(ID!=FT6236_CHIPID)) return 0;
    return 1;
}
void I2C_StatusCheck(){
    switch(I2CA_Status){
    case I2CA_Succesful:
        I2CA_BusyCnt = 0;
        I2CA_StartFailCnt = 0;
        I2CA_MastFailCnt = 0;
        break;
    //catch hanging on busy
    case I2CA_Busy:
        if(I2CA_BusyCnt>0xfff&&I2caRegs.I2CMDR.bit.MST){
            I2caRegs.I2CMDR.bit.STP = 1;
        }else if(I2CA_BusyCnt>0xfff){
            I2caRegs.I2CMDR.bit.IRS = 0;
            I2caRegs.I2CMDR.all=0;
            TS_init(40);
        }
        break;
    case I2CA_StartFail:
       if(I2CA_BusyCnt>0xfff&&I2caRegs.I2CMDR.bit.MST)
       {
           if(I2CA_StartFailCnt>0xfff&&!I2caRegs.I2CMDR.bit.STP){
                 I2caRegs.I2CMDR.bit.STP = 1;
           }
       }
       else if(I2CA_StartFailCnt>0xfff){
          I2caRegs.I2CMDR.bit.IRS = 0;
          I2caRegs.I2CMDR.all=0;
          TS_init(40);
       }
      break;
    }


}
Uint16 touched(){
   int t = TS_readRegister8(FT62XX_REG_NUMTOUCHES);
    if(t>2) return 0;
    return t;
}
void getTouchPoint(Uint16 n){
    readData();
     if ((touches == 0) || (n > 1)) {
         TS_Position.x=0;
         TS_Position.y=0;
     } else {
         TS_Position.x=touchX[0];
         TS_Position.y=touchY[0];
     }
}
Uint16 TS_readRegister8(Uint16 r){
   TSwriteBuff[0]=r;
   do {
       I2CA_Status = I2C_O2O_SendBytes(TSwriteBuff, 1);
   } while(I2CA_Status);
   do {
       I2CA_Status = I2C_O2O_ReadBytes(TSreadBuff, 1);
   } while(I2CA_Status);
 return TSreadBuff[0];
}
void TS_writeRegister8(Uint16 r, Uint16 d){
    TSwriteBuff[0]=r;
    TSwriteBuff[1]=d;
    do {
        I2CA_Status = I2C_O2O_SendBytes(TSwriteBuff, 2);
    } while(I2CA_Status);
}
void readData(){
    TSwriteBuff[0]=0;
    do {
        I2CA_Status = I2C_O2O_SendBytes(TSwriteBuff, 1);
    } while(I2CA_Status);
    do {
        I2CA_Status = I2C_O2O_ReadBytes(TSreadBuff, 16);
    } while(I2CA_Status);
    touches = TSreadBuff[2];

    if ((touches > 2) || (touches == 0)) touches = 0;

      touchX[0] = TSreadBuff[0x04 ] ;
     // touchX[0] <<= 8;
      //touchX[0] |= TSreadBuff[0x04 ];
      touchY[0] = TSreadBuff[0x05 ] & 0x0F;
      touchY[0] <<= 8;
      touchY[0] |= TSreadBuff[0x06];
      touchID[0] = TSreadBuff[0x07] >> 4;


}
