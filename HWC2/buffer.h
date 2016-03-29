#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_ERROR (msg_t*) NULL
#include "message.h"
#include <pthread.h>



typedef struct {
    msg_t * msg_list;
    int T;                      //intero che indica il primo spazio occupato
    int D;                      //intero che indica il primo spazio libero
    int size;                   //dimensione del buffer
    int k;                      //numero messaggi inseriti
    pthread_cond_t non_vuoto;   //variabile condizione per indicare il buffer non vuoto
    pthread_cond_t non_pieno;   //variabile condizione per indicare il buffer non pieno
    pthread_mutex_t buffer_mutex; //mutex per l'accesso al buffer
    pthread_mutex_t uso_t;        //mutex per l'accesso all'indice delle estrazioni
    pthread_mutex_t uso_d;        //mutex per l'accesso all'indice degli inserimenti
} buffer_t;

buffer_t* buffer_init(unsigned int maxsize);
void buffer_destroy(buffer_t* buffer);
msg_t* put_bloccante(buffer_t* buffer, msg_t* msg);
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg);
msg_t* get_bloccante(buffer_t* buffer);
msg_t* get_non_bloccante(buffer_t* buffer);

#endif 

