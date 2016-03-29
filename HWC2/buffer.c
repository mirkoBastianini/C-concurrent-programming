#include <stdlib.h>
#include <stdio.h>
#include "buffer.h"

buffer_t* buffer_init(unsigned int maxsize) {
   
    buffer_t* buffer = (buffer_t*) malloc(sizeof(buffer_t));
    buffer->D = 0;
    buffer->T = 0;
    buffer->k = 0;
    buffer->size = maxsize;
    pthread_mutex_init(&(buffer->buffer_mutex), NULL);
    pthread_mutex_init(&(buffer->uso_d), NULL);
    pthread_mutex_init(&(buffer->uso_t), NULL);
    pthread_cond_init(&(buffer->non_pieno), NULL);
    pthread_cond_init(&(buffer->non_vuoto), NULL);
    buffer->msg_list = (msg_t*) calloc(maxsize , sizeof (msg_t));
    return buffer;
}

void buffer_destroy(buffer_t* buffer) {
    pthread_cond_destroy(&(buffer->non_pieno));
    pthread_cond_destroy(&(buffer->non_vuoto));
    pthread_mutex_destroy(&(buffer->buffer_mutex));
    pthread_mutex_destroy(&(buffer->uso_t));
    pthread_mutex_destroy(&(buffer->uso_d));
    free(buffer->msg_list);
    free(buffer);

}

msg_t* put_bloccante(buffer_t* buffer, msg_t* msg) {
    msg_t* temp = msg_copy_string(msg);
    pthread_mutex_lock(&(buffer->buffer_mutex));
    while(buffer->size == buffer-> k)
        pthread_cond_wait(&(buffer->non_pieno),&(buffer->buffer_mutex));
    pthread_mutex_lock(&(buffer->uso_d));
    buffer->msg_list[buffer->D] = *temp;
    buffer->D = (buffer->D+1) % (buffer->size);
    buffer->k++;
    pthread_mutex_unlock(&(buffer->uso_d));
    pthread_cond_signal(&(buffer->non_vuoto));
    pthread_mutex_unlock(&(buffer->buffer_mutex));
    return temp;
}

msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg) {
    msg_t* temp = msg_copy_string(msg);
    pthread_mutex_lock(&(buffer->buffer_mutex));
    if(buffer->size == buffer-> k){
        pthread_mutex_unlock(&(buffer->buffer_mutex));
        return BUFFER_ERROR;
    }
    pthread_mutex_lock(&(buffer->uso_d));
    buffer->msg_list[buffer->D] = *temp;
    buffer->D = (buffer->D+1) % (buffer->size);
    buffer->k++;
    pthread_mutex_unlock(&(buffer->uso_d));
    pthread_cond_signal(&(buffer->non_vuoto));
    pthread_mutex_unlock(&(buffer->buffer_mutex));
    return temp;
}

msg_t* get_bloccante(buffer_t* buffer) {
    pthread_mutex_lock(&(buffer->buffer_mutex));
    while(buffer->k==0)
        pthread_cond_wait(&(buffer->non_vuoto),&(buffer->buffer_mutex));
    pthread_mutex_lock(&(buffer->uso_t));
    msg_t* temp = msg_copy_string(&(buffer->msg_list[buffer->T]));
    buffer->msg_list[buffer->T].content=NULL;	
    buffer->T=(buffer->T+1)%(buffer->size);
    buffer->k--;
    pthread_mutex_unlock(&(buffer->uso_t));
    pthread_cond_signal(&(buffer->non_pieno));
    pthread_mutex_unlock(&(buffer->buffer_mutex));
    return temp;
}

msg_t* get_non_bloccante(buffer_t* buffer) {
    pthread_mutex_lock(&(buffer->buffer_mutex));
   if(buffer->k==0){
        pthread_mutex_unlock(&(buffer->buffer_mutex));
        return BUFFER_ERROR;
   }
    pthread_mutex_lock(&(buffer->uso_t));
    msg_t* temp = msg_copy_string(&(buffer->msg_list[buffer->T]));
    buffer->msg_list[buffer->T].content=NULL;	
    buffer->T=(buffer->T+1)%(buffer->size);
    buffer->k--;
    pthread_mutex_unlock(&(buffer->uso_t));
    pthread_cond_signal(&(buffer->non_pieno));
    pthread_mutex_unlock(&(buffer->buffer_mutex));
    return temp;
}