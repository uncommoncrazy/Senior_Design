#include"buffer.h"
buffer * new_buffer(Uint16 Size){
    buffer * newBuff = (buffer *) far_malloc(sizeof(buffer));
    newBuff->size=Size;
    newBuff->index=0;
    newBuff->address = (Uint16 *)far_malloc(sizeof(Uint16)*Size);
    return newBuff;
}
