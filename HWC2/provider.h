#ifndef PROVIDER_H_
#define PROVIDER_H_

#include "poison_pill.h"
#include "provider_buffer.h"
#include <pthread.h>

typedef struct{
    int size;
    msg_t* messages[];
}msgs;


void* create_messages_streaming(void*);

#endif 
