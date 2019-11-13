/*
 * DisplayLibrary.c
 *
 *  Created on: Nov 13, 2019
 *      Author: Claudine
 */
#include "DisplayLibrary.h"

Uint16 drawChar( char letter, Uint16 color, FontInfo font, Uint32 x, Uint32 y){
    Uint16 letterIndex = (Uint16)letter - (Uint16)font.FirstChar;
    CharInfo character =font.Descrpitors[letterIndex];
   // Uint16 dispChar[200]={0};
    Uint16 strip;
    Uint16 check;
    Uint16 checkInit;
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

void drawText(Text text){
    char * print = text.string;
    Uint16 x = text.x, y=text.y;
    Uint16 index=0;
    while( print[index]){
        y += drawChar(print[index++],text.color,text.font,x,y)+3;
    }
}
//Text createText(char* string, Uint16* color, FontInfo font);
