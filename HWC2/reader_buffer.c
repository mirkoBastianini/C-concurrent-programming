#include <stdio.h>
#include <stdlib.h>
#include "reader_buffer.h"

reader_buffer_t* reader_buffer_initialize(int maxsize){
		reader_buffer_t* new_r_buffer=malloc(sizeof(reader_buffer_t));
		new_r_buffer->read_buffer=buffer_init(maxsize);
		return new_r_buffer;
}

void reader_buffer_destroy(reader_buffer_t* r_buffer){
	free(r_buffer);
}

int reader_buffer_insert(reader_buffer_t* r_buffer, msg_t* message){//inserisce il messaggio dentro il buffer del reader
	msg_t* inserted=put_non_bloccante(r_buffer->read_buffer,message);
	if (inserted!=NULL){
		return 1;
	}
	else return 0;
}

msg_t* reader_buffer_consume (reader_buffer_t* r_buffer){
	return get_non_bloccante(r_buffer->read_buffer);
}

int reader_buffer_numberOfMessage(reader_buffer_t* r_buffer){
	pthread_mutex_lock(&(r_buffer->read_buffer->uso_d));
	int messaggi=r_buffer->read_buffer->k;
	pthread_mutex_unlock(&(r_buffer->read_buffer->uso_d));
	return messaggi;
}

int reader_buffer_size(reader_buffer_t* r_buffer){
	pthread_mutex_lock(&(r_buffer->read_buffer->uso_t));
	int dim=r_buffer->read_buffer->size;
	pthread_mutex_unlock(&(r_buffer->read_buffer->uso_t));
	return dim;
}