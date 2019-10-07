

/**
 * main.c
 */
/*Descriptions
 * This codes use given driver to write name and course code on the lcd
 */
#include<F28x_Project.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCDDriver.h"

//Uint16 LCDBuffer[4];
Uint16 * LCDBuffer(void)
{
    Uint16 * Buffer = malloc(4*sizeof(Uint16));
    return Buffer;
}
void LCD_String(char * str, Uint16 * const Buffer)
{
   Uint16 i = 0;
   while(str[i]){
       LCD_Data(str[i++], Buffer);
       DELAY_US(5000);
   }
}
void LCD_float(float decimal, Uint16 * const Buffer)
{
   Uint16 i = 0;
   Uint16 ones = (Uint16)(decimal);
   float shift = (decimal-(float)(ones))*10;
   Uint16 tenths = (Uint16)(shift);
   shift = (shift-(float)(tenths))*10;
   Uint16 hundreths = (Uint16)(shift);
   char floating[] = { (char)(ones+0x30),'.',(char)(tenths+0x30),(char)(hundreths+0x30),(char)(0)};
   while(floating[i]){
       LCD_Data(floating[i++], Buffer);
       DELAY_US(5000);
   }
}
void LCD_thousandsfloat(float decimal, Uint16 * const Buffer)
{
   Uint16 i = 0;
   Uint16 thousands = (Uint16)(decimal)/1000;
   Uint16 hundreds = ((Uint16)(decimal)-thousands*1000)/100;
   Uint16 tens = ((Uint16)(decimal)-(thousands*1000+hundreds*100))/10;
   Uint16 ones = ((Uint16)(decimal)-(thousands*1000+hundreds*100+tens*10));
   char floating[] = { (char)(thousands+0x30),(char)(hundreds+0x30),(char)(tens+0x30),(char)(ones+0x30),(char)(0)};
   while(floating[i]){
       LCD_Data(floating[i++], Buffer);
       DELAY_US(5000);
   }
}
void Init_LCD( Uint16 * const Buffer)
{
    I2C_O2O_Master_Init(LCD_addr, 200.0,1.0);

    LCD_Command(LCD_init1, Buffer);
    LCD_Command(LCD_init2, Buffer);
    LCD_Command(Mode, Buffer);
    LCD_Command(DCP, Buffer);
    LCD_Command(ClearDisp, Buffer);

}

void LCD_Data(char data, Uint16 * const Buffer)
{
    Buffer[0]=  (Uint16)((data&0xf0)|RSEnableHi);
    Buffer[1]=  (Uint16)((data&0xf0)|RSEnableLo);
    Buffer[2]=  (Uint16)(((data<<4)&0xf0)|RSEnableHi);
    Buffer[3]=  (Uint16)(((data<<4)&0xf0)|RSEnableLo);
    I2C_O2O_SendBytes(Buffer, 4);

}

void LCD_Command(Uint16 Command, Uint16 * const Buffer)
{
    Buffer[0]= (Command&0xf0)|EnableHi;
    Buffer[1]= (Command&0xf0)|EnableLo;
    Buffer[2]= ((Command<<4)&0xf0)|EnableHi;
    Buffer[3]= ((Command<<4)&0xf0)|EnableLo;
    I2C_O2O_SendBytes(Buffer, 4);
}
