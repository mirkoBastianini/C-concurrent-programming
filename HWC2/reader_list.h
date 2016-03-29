#ifndef READER_LIST_H
#define READER_LIST_H


#include "list.h"
#include <pthread.h>
#include "poison_pill.h"
#include "reader.h"

list_t* reader_list;
iterator_t* iterator;
pthread_mutex_t reader_list_mutex;

void reader_list_initialize();
void reader_list_destroy();
int reader_list_size();
int reader_list_isEmpty();

void reader_list_addReader(void* reader);
int reader_list_removeReader(void* reader);
void reader_list_kill();
void reader_list_broadcast(msg_t* message);



#endif 

