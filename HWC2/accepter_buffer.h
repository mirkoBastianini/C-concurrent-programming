#ifndef ACCEPTER_BUFFER_H
#define ACCEPTER_BUFFER_H


#include "buffer.h"

buffer_t* accepter_buffer;

void accepter_buffer_initialize(int size);
void accepter_buffer_destroy();

void *accepter_buffer_insert(void* messaggio);
msg_t* accepter_buffer_delete();
void accepter_buffer_deleteAll();



#endif 

