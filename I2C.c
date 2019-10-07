

/**
 * main.c
 */
#include<F28x_Project.h>
#include "OneToOneI2CDriver.h"
#define     LEDPins     0x00ff
#define     nLEDPins    0xffffff00
#define     PBPins      0x0001C000
#define     SwitchPins  0x00000f00
#define     Switch      8
#define     PB          9
#define     PU          (PBPins|SwitchPins)
#define     LCD_init1   0x33
#define     LCD_init2   0x32
#define     Mode        0x28
#define     DCP         0x0f
#define     ClearDisp   0x01
#define     Line2       0xC0
#define     LCD_addr    0x27
int main(void)
{
    Uint16 Data[] = {0x3C,0x38,0x3C,0x38,0x3C,0x38,0x2C,0x28,0x2C,0x28,0x8C,0x88,0x0C,0x08,0xFC,0xF8,0x0C,0x08,0x1C,0x18};
    Uint16 * const I2C_Data = Data;
    InitSysCtrl();
    I2C_O2O_Master_Init(LCD_addr, 200.0,1.0);
    DELAY_US(5000);
    I2C_O2O_SendBytes(I2C_Data, 20);
    while(1){
        EALLOW;
        DELAY_US(5000);

    }
}
