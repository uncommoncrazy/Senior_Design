/*
 * MapInfo.h
 *
 *  Created on: Nov 13, 2019
 *      Author: Claudine
 */

#ifndef TEXTMAPS_MAPINFO_H_
#define TEXTMAPS_MAPINFO_H_
#include<F28x_Project.h>
typedef struct CINFO_t{
    Uint16 width, hight, offset;
} CharInfo;
typedef struct fontinfo_t{
    Uint16  hight;
    char    FirstChar, LastChar;
    const CharInfo* Descrpitors;
    const Uint16*  Bitmaps;

} FontInfo;


#endif /* TEXTMAPS_MAPINFO_H_ */
