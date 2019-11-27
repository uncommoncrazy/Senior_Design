/*
 * TFTLCD_Driver.c
 *
 *  Created on: Oct 5, 2019
 *      Author: Claudine
 */
#include <F28x_Project.h>
#include "TFTLCD_Driver.h"
#include "TFTRegisters.h"
#define TFTWIDTH        240
#define TFTHEIGHT       320
#define TFTLCD_DELAY    0xff
void LCD_init(){
 Init_LCDPins();
 LCD_CS_Off;
 LCD_Write_Off;
 LCD_Read_Off;
 LCD_CD_Data;
}

void startLCD(){
    resetLCD();
    DELAY_US(200000);
    LCD_CS_On;
    LCD_WriteReg8(ILI9341_SOFTRESET, 0);
    DELAY_US(50000);
    LCD_WriteReg8(ILI9341_DISPLAYOFF, 0);

    LCD_WriteReg8(ILI9341_POWERCONTROL1, 0x23);
    LCD_WriteReg8(ILI9341_POWERCONTROL2, 0x10);
    LCD_WriteReg16(ILI9341_VCOMCONTROL1, 0x2B2B);
    LCD_WriteReg8(ILI9341_VCOMCONTROL2, 0xC0);
    LCD_WriteReg8(ILI9341_MEMCONTROL, ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR);
    LCD_WriteReg8(ILI9341_PIXELFORMAT, 0x55);
    LCD_WriteReg16(ILI9341_FRAMECONTROL, 0x001B);

    LCD_WriteReg8(ILI9341_ENTRYMODE, 0x07);
    /* writeRegister32(ILI9341_DISPLAYFUNC, 0x0A822700);*/

    LCD_WriteReg8(ILI9341_SLEEPOUT, 0);
    DELAY_US(150000);
    LCD_WriteReg8(ILI9341_DISPLAYON, 0);
    DELAY_US(500000);
    setWindow(0, 0, TFTWIDTH-1, TFTHEIGHT-1);
}
void resetLCD(){
    LCD_CS_Off;
    LCD_Write_Off;
    LCD_Read_Off;

    LCD_CS_On;
    LCD_CD_Command;
    LCD_Write8(0);
    for (Uint16 i = 0; i <3 ; i++) LCD_WR_Strobe;
    LCD_CS_Off;
}
void setWindow(int32 x1, int32 y1, int32 x2, int32 y2){
    LCD_CS_On;
    int32 temp = (x2);
    temp |= x1 << 16;

    writeRegister32(ILI9341_COLADDRSET, temp);
    temp = (y2);
    temp |= y1 << 16;

    writeRegister32(ILI9341_PAGEADDRSET, temp);
    LCD_CS_Off;
}
void pushColors(Uint16 *data, Uint16 len){
    uint16_t color, hi, lo;
//    CS_ACTIVE;
    LCD_CS_On;
    LCD_CD_Command;
    LCD_Write8(0x2C);
    //LCD_CS_Off;
   // LCD_CS_On;
    LCD_CD_Data;
    while(len--) {
      color = *data++;
      hi    = color >> 8; // Don't simplify or merge these
      lo    = color&0xff;      // lines, there's macro shenanigans
      LCD_Write8(hi);         // going on.
      LCD_Write8(lo);
    }
    LCD_CS_Off;
}
void flood(Uint16 color, Uint32 length){
    LCD_CS_On;
    Uint16 blocks, hi ,lo , i;
    hi = (color>>8)&0xff;
    lo = color&0xff;
    LCD_CS_On;
    LCD_CD_Command;
    LCD_Write8(0x2C);
    LCD_Delay;
    LCD_CD_Data;
    LCD_Delay;
    LCD_Write8(hi);
    LCD_Write8(lo);
    length--;
    blocks = (Uint16)(length/64);
    if (hi == lo){
        while(blocks--){
            i = 16;
            do{
                LCD_WR_Strobe;
                 LCD_WR_Strobe;
                LCD_WR_Strobe;
                LCD_WR_Strobe;
                LCD_WR_Strobe;
                LCD_WR_Strobe;
                LCD_WR_Strobe;
                LCD_WR_Strobe;
            }while(--i);
        }
        for(i = (Uint16)(length)&63;i--;){
            LCD_WR_Strobe;
            LCD_WR_Strobe;
        }
    }else{
        while(blocks--){
             i = 16;
             do{
                 LCD_Write8(hi);
                 LCD_Write8(lo);
                 LCD_Write8(hi);
                 LCD_Write8(lo);
                 LCD_Write8(hi);
                 LCD_Write8(lo);
                 LCD_Write8(hi);
                 LCD_Write8(lo);
             }while(--i);
         }
         for(i = (Uint16)(length)&63;i--;){
             LCD_Write8(hi);
             LCD_Write8(lo);
         }
    }
    LCD_CS_Off;
}
void drawPixel(int16 x, int16 y, Uint16 color){
    if(( x<0 )||(y<0)||(y>=TFTHEIGHT)||(x>=TFTWIDTH)) return;
    LCD_CS_On;
    setWindow(x,y,TFTWIDTH-1,TFTHEIGHT-1);
    LCD_CS_On;
    LCD_CS_On;
    LCD_CD_Command;
    LCD_Write8(0x2C);
    LCD_CD_Data;
    LCD_Write8(color>>8);
    LCD_Write8(color);
    LCD_CS_Off;
}
void drawHorzLine(int16 x, int16 y, int16 length,Uint16 color){
    int16 x2;
    if((length <=0 )||(y<0)||(y>=TFTHEIGHT)||(x>=TFTWIDTH)||((x2=(x+length-1))<0)) return;
    if(x < 0){
        length +=x;
        x=0;
    }
    if(x2>=TFTWIDTH){
        x2 = TFTWIDTH-1;
        length = x2 - x + 1;
    }
    setWindow(x,y,x2,y);
    flood(color, length);
}
void drawVertLine(int16 x, int16 y, int16 length,Uint16 color){
    int16 y2;
    if((length <=0 )||(y<0)||(y>=TFTHEIGHT)||(x>=TFTWIDTH)||((y2=(y+length-1))<0)) return;
    if(y < 0){
        length +=y;
        y=0;
    }
    if(y2>=TFTHEIGHT){
        y2 = TFTHEIGHT-1;
        length = y2 - y + 1;
    }
    setWindow(x,y,x,y2);
    flood(color, length);
}
void fillRect(int16 x, int16 y, int16 w, int16 h, Uint16 color){
    int16 x2,y2;
    if((w <= 0)||(h<=0)||(y>=TFTHEIGHT)||(x>=TFTWIDTH)||((x2=x+w-1)<0)||((y2=y+h-1)<0)) return;
    if(x<0){
        w += x;
        x=0;
    }
    if(y<0){
        h += y;
        y = 0;
    }
    if(x2>=TFTWIDTH){
        x2 = TFTWIDTH-1;
        w = x2-x+1;
    }
    if(y2>=TFTHEIGHT){
        y2 = TFTHEIGHT -1;
        h= y2-y+1;
    }
    setWindow(x,y,x2,y2);
    flood(color, ((Uint32)w)*((Uint32)h));
}
void fillScreen(Uint16 color){
    setWindow(0, 0, TFTWIDTH-1, TFTHEIGHT-1);
    flood(color, (Uint32)TFTWIDTH*(Uint32)TFTHEIGHT);
}
void setRegister8(Uint16 *ptr, Uint16 value);
void setRegister16(Uint16 *ptr, Uint16 value);
void setRotation(Uint16 rotation){
    uint16_t t;

     switch (rotation) {
     case 2:
       t = ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR;
       break;
     case 3:
       t = ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
       break;
    case 0:
      t = ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR;
      break;
     case 1:
       t = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR;
       break;
    }
     LCD_WriteReg8(ILI9341_MADCTL, t ); // MADCTL
     // For 9341, init default full-screen address window:
     setWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1); // CS_IDLE happens
}

Uint16 genColor(Uint16 r, Uint16 g, Uint16 b){
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
void writeCommand(Uint16 d){
LCD_CS_On;
LCD_CD_Command;
LCD_Write8(d);
LCD_CS_Off;
}
void readStrobe(){
  LCD_CS_On;
  LCD_CD_Data;

}
Uint16 getID(){
    writeCommand(0xD3);
    LCD_DIR_RD;
    LCD_CS_On;
    LCD_CD_Data;
    LCD_Read8();
    LCD_Delay;
    LCD_Delay;
    LCD_Read8();
    LCD_Delay;
    LCD_Delay;
    Uint16 Id= LCD_Read8()<<8;
    LCD_Delay;
    LCD_Delay;
    Id |= LCD_Read8();
    LCD_CS_Off;
    LCD_DIR_Write;
    return Id;
}
void writeRegister24(Uint16 r, Uint32 d){
     LCD_CS_On;
     LCD_CD_Command;
     LCD_Write8(r);
     LCD_CD_Data;
     DELAY_US(10);
     LCD_Write8(d >> 16);
     DELAY_US(10);
     LCD_Write8(d >> 8);
     DELAY_US(10);
     LCD_Write8(d);
     LCD_CS_Off;
}
void writeRegister32(Uint16 r, Uint32 d){
    LCD_CS_On;
    LCD_CD_Command;
    LCD_Write8(r);
    LCD_CD_Data;
    DELAY_US(10);
    LCD_Write8(d >> 24);
    DELAY_US(10);
    LCD_Write8(d >> 16);
    DELAY_US(10);
    LCD_Write8(d >> 8);
    DELAY_US(10);
    LCD_Write8(d);
    LCD_CS_Off;
}
Uint16 readReg(Uint16 r){
    Uint32 id;
    Uint16 x;

    // try reading register #4
    LCD_CS_On;
    LCD_CD_Command;
    LCD_Write8(r);
    LCD_DIR_RD;
    LCD_CD_Data;
    DELAY_US(50);
    x=LCD_Read8();
    id = x;          // Do not merge or otherwise simplify
    id <<= 8;              // these lines.  It's an unfortunate
    x=LCD_Read8();
    id  |= x;        // shenanigans that are going on.
    id <<= 8;              // these lines.  It's an unfortunate
    x=LCD_Read8();
    id  |= x;        // shenanigans that are going on.
    id <<= 8;              // these lines.  It's an unfortunate
    x=LCD_Read8();
    id  |= x;        // shenanigans that are going on.
    LCD_CS_Off;
    LCD_DIR_Write;  // Restore LCD data port(s) to WRITE configuration

    return id;
}
