/*
 * numbers10.c
 *
 *  Created on: Nov 13, 2019
 *      Author: Claudine
 */
#include "numbers10.h"
/*
**  Font data for Arial Narrow 10pt
*/

/* Character bitmaps for Arial Narrow 10pt */
Uint16 numbers10Bitmaps[] =
{
    /* @0 '.' (1 pixels wide) */
    0x00, //
    0x00, //
    0x00, //
    0x00, //
    0x00, //
    0x00, //
    0x00, //
    0x00, //
    0x80, // #

    /* @9 '/' (3 pixels wide) */
    0x20, //   #
    0x20, //   #
    0x20, //   #
    0x40, //  #
    0x40, //  #
    0x40, //  #
    0x40, //  #
    0x80, // #
    0x80, // #

    /* @18 '0' (5 pixels wide) */
    0x20, //   #
    0x50, //  # #
    0x88, // #   #
    0x88, // #   #
    0x88, // #   #
    0x88, // #   #
    0x88, // #   #
    0x50, //  # #
    0x20, //   #

    /* @27 '1' (3 pixels wide) */
    0x20, //   #
    0x60, //  ##
    0xA0, // # #
    0x20, //   #
    0x20, //   #
    0x20, //   #
    0x20, //   #
    0x20, //   #
    0x20, //   #

    /* @36 '2' (5 pixels wide) */
    0x60, //  ##
    0x90, // #  #
    0x88, // #   #
    0x08, //     #
    0x10, //    #
    0x20, //   #
    0x40, //  #
    0x80, // #
    0xF8, // #####

    /* @45 '3' (5 pixels wide) */
    0x60, //  ##
    0x90, // #  #
    0x08, //     #
    0x10, //    #
    0x20, //   #
    0x10, //    #
    0x88, // #   #
    0x90, // #  #
    0x60, //  ##

    /* @54 '4' (5 pixels wide) */
    0x10, //    #
    0x30, //   ##
    0x30, //   ##
    0x50, //  # #
    0x50, //  # #
    0x90, // #  #
    0xF8, // #####
    0x10, //    #
    0x10, //    #

    /* @63 '5' (5 pixels wide) */
    0x78, //  ####
    0x40, //  #
    0x80, // #
    0xE0, // ###
    0x90, // #  #
    0x08, //     #
    0x88, // #   #
    0x90, // #  #
    0x60, //  ##

    /* @72 '6' (5 pixels wide) */
    0x30, //   ##
    0x48, //  #  #
    0x80, // #
    0xA0, // # #
    0xD0, // ## #
    0x88, // #   #
    0x88, // #   #
    0x50, //  # #
    0x20, //   #

    /* @81 '7' (5 pixels wide) */
    0xF8, // #####
    0x08, //     #
    0x10, //    #
    0x20, //   #
    0x20, //   #
    0x20, //   #
    0x40, //  #
    0x40, //  #
    0x40, //  #

    /* @90 '8' (5 pixels wide) */
    0x20, //   #
    0x50, //  # #
    0x88, // #   #
    0x50, //  # #
    0x20, //   #
    0x50, //  # #
    0x88, // #   #
    0x50, //  # #
    0x20, //   #

    /* @99 '9' (5 pixels wide) */
    0x20, //   #
    0x50, //  # #
    0x88, // #   #
    0x88, // #   #
    0x58, //  # ##
    0x28, //   # #
    0x08, //     #
    0x90, // #  #
    0x60, //  ##

    /* @108 ':' (1 pixels wide) */
    0x00, //
    0x00, //
    0x80, // #
    0x00, //
    0x00, //
    0x00, //
    0x00, //
    0x00, //
    0x80, // #
};
/* Character descriptors for Arial Narrow 10pt */
/* { [Char width in bits], [Char height in bits], [Offset into arialNarrow_10ptCharBitmaps in bytes] } */
CharInfo numbers10Descriptors[] =
{
    {1, 9, 0},      /* . */
    {3, 9, 9},      /* / */
    {5, 9, 18},         /* 0 */
    {3, 9, 27},         /* 1 */
    {5, 9, 36},         /* 2 */
    {5, 9, 45},         /* 3 */
    {5, 9, 54},         /* 4 */
    {5, 9, 63},         /* 5 */
    {5, 9, 72},         /* 6 */
    {5, 9, 81},         /* 7 */
    {5, 9, 90},         /* 8 */
    {5, 9, 99},         /* 9 */
    {1, 9, 108},        /* : */
};


/* Font information for Arial Narrow 10pt */
const FontInfo numbers10FontInfo =
{
    9, /*  Character height */
    '.', /*  Start character */
    ':', /*  End character */
    numbers10Descriptors, /*  Character descriptor array */
    numbers10Bitmaps, /*  Character bitmap array */
};



