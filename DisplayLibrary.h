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
extern Uint16  textColor, textFill;
Uint32 drawFloat(float decimal,Uint32 x,Uint32 y,Uint16 color);
Uint32 drawThousands(Uint16 decimal,Uint32 x,Uint32 y,Uint16 color);
//extern  Font ariel10;
Uint32 drawChar( char letter, Uint16 color, Uint32 x, Uint32 y);
Uint32 drawText(Text text);
void test(Uint32 x, Uint32 y , Uint16 side);
Uint32 drawCharQ( char letter, Uint32 x, Uint32 y);

//Text createText(char* string, Uint16* color, FontInfo font);
#endif /* DISPLAYLIBRARY_H_ */
