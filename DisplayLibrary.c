/*
 * DisplayLibrary.c
 *
 *  Created on: Nov 13, 2019
 *      Author: Claudine
 */
#include "DisplayLibrary.h"

void drawChar( char letter, Uint16* color, FontInfo font, Uint32 x, Uint32 y){
    Uint16 letterIndex = (Uint16)letter - (Uint16)font.FirstChar;
    CharInfo character =font.Descrpitors[letterIndex];
    Uint16 size = character.width*character.hight;
    Uint16 dispChar[200]={0};
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

               switch(check&strip){
                   case 0:
                       dispChar[character.width*col+row]= color[1];
                       break;
                   default:
                       dispChar[character.width*col+row]= color[0];
               }
               check = check>>1;
        }
    }
    setWindow(x, y, x+character.width, y+character.hight);
    pushColors(dispChar, character.width*character.hight);
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

//void drawText(Text text);
//Text createText(char* string, Uint16* color, FontInfo font);
