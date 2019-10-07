/*
 * buffer.h
 *
 *  Created on: Mar 13, 2019
 *      Author: Claudine
 */

#ifndef BUFFERS_BUFFER_H_
#define BUFFERS_BUFFER_H_
#include<F28x_Project.h>
#include <stdlib.h>
typedef struct buffer_t{
    Uint16 * address;
    Uint16 index,size;
} buffer;
buffer * new_buffer(Uint16 Size);
#endif /* BUFFERS_BUFFER_H_ */
