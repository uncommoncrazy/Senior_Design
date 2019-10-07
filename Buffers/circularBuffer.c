#include "circularBuffer.h"
struct circularBuffer_t{
    Uint32 SRAMaddress,size,wIndex,rIndex;
    transferBuffer * buffer;

};
