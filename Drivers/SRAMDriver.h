/*
 * SRAMDriver.h
 *
 *  Created on: Feb 23, 2019
 *      Author: Claudine
 */

#ifndef SRAMDRIVER_H_
#define SRAMDRIVER_H_
#define     HighByte    7
#define     LowByte     9
#define     SRAM_SizeW  0x1ffff
#define     inst_Read   3<<8
#define     inst_Write  2<<8
void Init_SRAM(void);
Uint16 * SRAM_Read(Uint32 Addr, Uint16 size);
void SRAM_Write(Uint32 Addr, Uint16 * Buffer,Uint16 size);
Uint16 * copyToBuffer( Uint16 value, Uint16 size);
Uint16 SRAM_Rd(Uint32 Addr);
void SRAM_Wr(Uint32 Addr, Uint16 data);
#endif /* SRAMDRIVER_H_ */
