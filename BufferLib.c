/*
 * BufferLib.c orginal
 *
 *  Created on: Mar 13, 2019
 *      Author: Claudine
 */
#include "BufferLib.h"
#define maxBufferSize 0x2ffff

void bufferSwap( buffer * buff1, buffer * buff2){
       Uint16* temp = buff1->address;
       buff1->address = buff2->address;
       buff2->address =  temp;
}
void setSend( circularBuffer * circular){
    //xor swap the buffer pointers
    bufferSwap(circular->buffer->write,circular->buffer->send);
    circular->buffer->sendFlag=  1;
    circular->buffer->write->index = 0;
}
void send( circularBuffer * circular, Uint16 amount){
    SRAM_Write(circular->SRAMaddress+circular->wIndex, circular->buffer->send->address,amount);
    circular->buffer->send->address = far_malloc(sizeof(Uint16)*circular->buffer->size);
    circular->wIndex+=amount;
    circular->buffer->sendFlag=  0;
}
void setLoad( circularBuffer * circular){
    //xor swap the buffer pointers
    bufferSwap(circular->buffer->read,circular->buffer->load);
    far_free(circular->buffer->load->address);
    circular->buffer->loadFlag=  1;
    circular->buffer->read->index = 0;
}
void load( circularBuffer * circular, Uint16 amount){
    circular->buffer->load->address= SRAM_Read(circular->SRAMaddress+circular->rIndex, amount);
    circular->rIndex+=amount;
    circular->buffer->loadFlag=  0;
}
// record data and send to sram
Uint32 circ_write( circularBuffer * memory, Uint16 data){
    Uint16 bsize = memory->buffer->size;
    Uint32 writesLeft = memory->size - memory->wIndex;
    // get minimum
    Uint16  checkSize = (writesLeft > bsize)? bsize:(Uint16)writesLeft;
    if(memory->size > memory->wIndex+memory->buffer->write->index)
          memory->buffer->write->address[memory->buffer->write->index++]=data;
    if(memory->buffer->write->index >= checkSize )
        setSend(memory);
    //store input

    //check if send buffer is full
    if(memory->buffer->sendFlag)
        send(memory, checkSize);

    return memory->wIndex + memory->buffer->write->index;
}
Uint32 wr(circularBuffer * memory, Uint16 data){
    SRAM_Wr(memory->SRAMaddress+memory->wIndex++,data);
    return memory->wIndex;
}
Uint32 write_at(circularBuffer * memory, Uint16 data,Uint32 i){
    Uint32 ammnt =memory->buffer->write->index;
    setSend(memory);
    send(memory, ammnt);
    memory->wIndex = i;
    return circ_write(memory,  data);
}
Uint16 circ_read( circularBuffer * memory){
    Uint16 out;
    Uint16 bsize = memory->buffer->size;
    Uint32 readsLeft =memory->size - memory->rIndex;
    //check if any reads left to buffer
    Uint16  checkSize = (readsLeft > bsize)? bsize:(Uint16)readsLeft;
    //send write buffer if full
    if(memory->buffer->read->index >= checkSize)
        setLoad(memory);

        //store input audio
    //check if send buffer is full
    if(memory->buffer->loadFlag || memory->rIndex==0 )
        load(memory, checkSize);
    //circle around
    if( !(memory->size - memory->rIndex))
        memory->rIndex=0;
    out =  memory->buffer->read->address[memory->buffer->read->index++];
    return out;
}
Uint32 rd(circularBuffer * memory){
    return SRAM_Rd(memory->SRAMaddress+memory->rIndex++);
}
Uint16 read_at(circularBuffer * memory, Uint16 data,Uint32 i){
    memory->rIndex = i;
    setLoad(memory);
    return circ_read(memory);
}
