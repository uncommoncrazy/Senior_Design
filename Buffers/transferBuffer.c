#include "transferBuffer.h"
transferBuffer * new_TransferBuffer(Uint16 size){
    transferBuffer * newBuff = (transferBuffer*)far_malloc(sizeof(transferBuffer));
    newBuff->write= new_buffer(size);
    newBuff->send = new_buffer(size);
    newBuff->read = new_buffer(size);
    newBuff->load = new_buffer(size);
    far_free(newBuff->load->address);
    newBuff->size = size;
    newBuff->loadFlag=0;
    newBuff->sendFlag=0;
    return newBuff;
 }
transferBuffer TransferBuffer(Uint16 size){
    transferBuffer newBuff= {0};
    newBuff.write->index=0;
    newBuff.write->size=size;
    newBuff.write->address=  far_malloc(sizeof(Uint16)*size);
    newBuff.send->index=0;
    newBuff.send->size=size;
    newBuff.send->address=  far_malloc(sizeof(Uint16)*size);
    newBuff.read->index=0;
    newBuff.read->size=size;
    newBuff.read->address=  far_malloc(sizeof(Uint16)*size);
    newBuff.load->index=0;
    newBuff.load->size=size;
    newBuff.size = size;
    return newBuff;
 }
