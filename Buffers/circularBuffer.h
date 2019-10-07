/*
 * circularBuff.h
 *
 *  Created on: Mar 13, 2019
 *      Author: Claudine
 */

#ifndef BUFFERS_CIRCULARBUFFER_H_
#define BUFFERS_CIRCULARBUFFER_H_
#include "transferBuffer.h"
typedef struct circularBuffer_t{
    Uint32 SRAMaddress,size,wIndex,rIndex;
    transferBuffer * buffer;
} circularBuffer;
#endif /* BUFFERS_CIRCULARBUFFER_H_ */
