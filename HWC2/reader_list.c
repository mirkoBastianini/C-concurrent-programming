#include "reader_list.h"

void reader_list_initialize(){
	reader_list=list_init();
	pthread_mutex_init(&(reader_list_mutex),NULL);	
}

void reader_list_destroy(){
	pthread_mutex_destroy(&(reader_list_mutex));
	list_destroy(reader_list);
}

int reader_list_size (){
	pthread_mutex_lock(&(reader_list_mutex));
	int dim=size(reader_list);
	pthread_mutex_unlock(&(reader_list_mutex));
	return dim;
}

int reader_list_isEmpty(){
	pthread_mutex_lock(&(reader_list_mutex));
	int vuota=isEmpty(reader_list);
	pthread_mutex_unlock(&(reader_list_mutex));
	return vuota;
}

void reader_list_addReader(void* reader){
	pthread_mutex_lock(&(reader_list_mutex));
	addElement(reader_list, reader);
	pthread_mutex_unlock(&(reader_list_mutex));
}

int reader_list_removeReader(void* reader){
	int i=pthread_mutex_trylock(&(reader_list_mutex));
	int deleted= removeElement(reader_list, reader);
	if(i)
            pthread_mutex_unlock(&(reader_list_mutex));
	return deleted;
}

void reader_list_kill(){
	pthread_mutex_lock(&(reader_list_mutex));
	iterator=iterator_init(reader_list);
	while(hasNext(iterator)){
		reader_t* reader=(reader_t*)next(iterator);
		while(!reader_buffer_insert(reader->buffer,(msg_t*)&POISON_PILL_MSG));
	}
	iterator_destroy(iterator);
	pthread_mutex_unlock(&(reader_list_mutex));
}

void reader_list_broadcast(msg_t* message){
	pthread_mutex_lock(&(reader_list_mutex));
	iterator=iterator_init(reader_list);
	while(hasNext(iterator)){
		reader_t* reader=(reader_t*)next(iterator);
		if(!reader_buffer_insert(reader->buffer,message)){
                    printf("\n\nRIMOZIONE reader troppo lento READER =%s\n",reader->name);
                    reader_list_removeReader((void*)reader);
                    reader_destroy(reader);
                }
	}
	iterator_destroy(iterator);
	pthread_mutex_unlock(&(reader_list_mutex));
}
