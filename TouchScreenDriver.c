/*
 * TouchScreenDriver.c
 *
 *  Created on: Oct 7, 2019
 *      Author: Claudine
 */
#include "TouchScreenDriver.h"

Unit16 TS_init(Uint16 thresh){
    I2C_O2O_Master_Init(LCD_addr, 200.0,1.0);
}
Uint16 touched(){}
TouchPoint getTouchPoint(){}
Uint16 TS_readRegister8(Uint16 r){

}
void TS_writeRegister8(Uint16 r, Uint16 d){}
void readData(){}
