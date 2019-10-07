/*
 * BufferLib.h
 *
 *  Created on: Mar 13, 2019
 *      Author: Claudine
 */

#ifndef BUFFERLIB_H_
#define BUFFERLIB_H_
#include"Buffers/circularBuffer.h"
#include"SRAMDriver.h"
#define bufferLimit 0x3ffff
transferBuffer * new_TransferBuffer(Uint16 size);
transferBuffer TransferBuffer(Uint16 size);
void bufferSwap( buffer * buff1, buffer * buff2);
void setSend( circularBuffer * circular);
void send( circularBuffer * circular, Uint16 amount);
void setLoad( circularBuffer * circular);
void load( circularBuffer * circular, Uint16 amount);
Uint32 circ_write( circularBuffer * memory, Uint16 data);
Uint32 write_at(circularBuffer * memory, Uint16 data,Uint32 i);
Uint16 circ_read( circularBuffer * memory);
Uint16 read_at(circularBuffer * memory, Uint16 data,Uint32 i);
Uint32 wr(circularBuffer * memory, Uint16 data);
Uint32 rd(circularBuffer * memory);
#endif /* BUFFERLIB_H_ */
