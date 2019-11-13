/*
 * DisplayLibrary.h
 *
 *  Created on: Nov 13, 2019
 *      Author: Claudine
 */

#ifndef DISPLAYLIBRARY_H_
#define DISPLAYLIBRARY_H_
#include"Textmaps/capitalLetters10.h"
#include"TFTLCD_Driver.h"
#include"Textmaps/lowerLetters10.h"
#include"Textmaps/numbers10.h"
typedef struct font_t{
    FontInfo const lowerCase, upperCase, numerical;
}Font;
typedef struct text_t{
    char* string;
    Uint16 color;
    Uint32 x,y;
}Text;

//extern  Font ariel10;
Uint16 drawChar( char letter, Uint16 color, FontInfo font, Uint32 x, Uint32 y);
void drawText(Text text);
void test(Uint32 x, Uint32 y , Uint16 side);
//Text createText(char* string, Uint16* color, FontInfo font);
#endif /* DISPLAYLIBRARY_H_ */
