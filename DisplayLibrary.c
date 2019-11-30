/*
 * DisplayLibrary.c
 *
 *  Created on: Nov 13, 2019
 *      Author: Claudine
 */
#include "DisplayLibrary.h"
//Font ariel10 = {lowercase10,capitalLetter10, numbers10};
Uint32 drawChar( char letter, Uint16 color, Uint32 x, Uint32 y){
   // Uint16 dispChar[200]={0};
    Uint16 strip;
    Uint16 check;
    Uint16 checkInit;
    FontInfo font;
    if((Uint16)letter>=(Uint16)lowercase10.FirstChar){
        font=lowercase10;
    }else if((Uint16)letter<(Uint16)capitalLetter10.FirstChar){
        font=numbers10;
    }else{
        font=capitalLetter10;
    }
    Uint16 letterIndex = (Uint16)letter - (Uint16)font.FirstChar;
    CharInfo character =font.Descrpitors[letterIndex];
    if(character.width<=8){
        checkInit = 0x80;
    }else{
        checkInit = 0x8000;
    }
    // draw char into array
    for(Uint16 row =0; row<character.hight; row++){
        check = checkInit;
        strip =font.Bitmaps[character.offset+row];
        for(Uint16 col =0; col<character.width; col++){
               if(check&strip) drawPixel(x-row, y+col,  color);
               check = check>>1;
        }
    }
    return character.width;
}
void test(Uint32 x, Uint32 y , Uint16 side){
     Uint16 Disp[2500];
     Uint16 length = side*side;
     Uint16 color;
     for(Uint16 count = 0; count<10000; count++){
         color = count&0xff;
         Disp[count]=genColor(color, color^0xff, 0);
     }
     setWindow(x, y, x+side, y+side);
     pushColors(Disp, length);
 }
Uint32 drawFloat(float decimal,Uint32 x,Uint32 y,Uint16 color)
{
    Uint16 ones = (Uint16)(decimal);
    float shift = (decimal-(float)(ones))*10;
    Uint16 tenths = (Uint16)(shift);
    shift = (shift-(float)(tenths))*10;
    Uint16 hundreths = (Uint16)(shift);
    char floating[] = { (char)(ones+0x30),'.',(char)(tenths+0x30),(char)(hundreths+0x30),(char)(0)};
    Text number = {.string = floating, .x = x,.y = y, .color=color};
    return drawText(number);
}
Uint32 drawThousands(Uint16 decimal,Uint32 x,Uint32 y,Uint16 color)
{
   Uint16 thousands = (Uint16)(decimal)/1000;
   Uint16 hundreds = ((Uint16)(decimal)-thousands*1000)/100;
   Uint16 tens = ((Uint16)(decimal)-(thousands*1000+hundreds*100))/10;
   Uint16 ones = ((Uint16)(decimal)-(thousands*1000+hundreds*100+tens*10));
   char floating[] = { (char)(thousands+0x30),(char)(hundreds+0x30),(char)(tens+0x30),(char)(ones+0x30),(char)(0)};
   Text number = {.string = floating, .x = x,.y = y, .color=color};
   return drawText(number);
}
Uint32 drawText(Text text){
    char * print = text.string;
    Uint16 x = text.x, y=text.y;
    Uint16 index=0;
    while(print[index]){
        y += drawChar(print[index++],text.color,x,y)+3;
    }
    return y;
}
//Text createText(char* string, Uint16* color, FontInfo font);
