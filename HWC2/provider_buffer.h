#ifndef PROVIDER_BUFFER_H
#define PROVIDER_BUFFER_H

#include "buffer.h"

buffer_t* provider_buffer;

void provider_buffer_init(int);
void provider_buffer_destroy();
void provider_buffer_insert(msg_t*);
msg_t* provider_buffer_read();

#endif 

