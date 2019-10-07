/*
 * transferBuffer.h
 *
 *  Created on: Mar 13, 2019
 *      Author: Claudine
 */

#ifndef BUFFERS_TRANSFERBUFFER_H_
#define BUFFERS_TRANSFERBUFFER_H_
#include "buffer.h"
typedef struct transferBuffer_t{
    Uint16 sendFlag;
    Uint16 loadFlag;
    Uint16 size;
    buffer * write;
    buffer * send;
    buffer * read;
    buffer * load;
} transferBuffer;
transferBuffer * new_TransferBuffer(Uint16 size);
transferBuffer TransferBuffer(Uint16 size);
#endif /* BUFFERS_TRANSFERBUFFER_H_ */
