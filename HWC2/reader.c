#include "reader.h"
#include "reader_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#define Sleep(ms) sleep((ms)/1000)

reader_t* reader_initialize(char name[],int delay, int maxsize){
		reader_t* new_reader=(reader_t*)malloc(sizeof(reader_t));
		new_reader->name=name;
		new_reader->processing_time=delay;
		new_reader->buffer=reader_buffer_initialize(maxsize);
                new_reader->thread =(pthread_t*)malloc(sizeof(pthread_t));
		return new_reader;
}
//un thread killer, per distruggere il reader
void reader_destroy(reader_t* reader){
    pthread_t killer;
    pthread_create(&killer,NULL,killerMetod,(void*)reader);
}
void *killerMetod(void* readerK){
    reader_t* r=(reader_t*)readerK;
    while(!reader_insert(r,(msg_t*)&POISON_PILL_MSG));
    pthread_exit(NULL);
}

void *reader_consume(void* reader){
	while(1){
		msg_t* message= reader_buffer_consume(((reader_t*)reader)->buffer);
                //se il messaggio non è vuoto e non è una poision pill
		if(message!=NULL && (strcmp(message->content,"POISON_PILL_MSG")!=0) && message->content!=NULL){
			printf("\n%s ha letto %s", (char*)((reader_t*)reader)->name,(char*) message->content);
			Sleep(((reader_t*)reader)->processing_time);
		}
                //se il messaggio non è vuoto e hai ricevuto una Poison Pill,esci
		else if(message!=NULL && (strcmp(message->content,"POISON_PILL_MSG")==0) && message->content!=NULL){
			printf("\n%s ha ricevuto %s", (char*)((reader_t*)reader)->name, (char*)message->content);
			break;
                }
                Sleep(2);
		
	}
        reader_list_removeReader((void*)reader);
        pthread_cancel(*((reader_t*)reader)->thread);
        pthread_join(*((reader_t*)reader)->thread,NULL);
        free(reader);	
	pthread_exit(NULL);
}

int reader_insert(reader_t* reader, msg_t* message){
	int result=reader_buffer_insert(reader->buffer, message);
	return result;
}

void reader_active(reader_t* reader){
    pthread_create(reader->thread,NULL,reader_consume,reader);
}
