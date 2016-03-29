#include "provider_buffer.h"


/*int buffer di dimensione dim*/
void provider_buffer_init(int dim){
    provider_buffer = buffer_init(dim);
}

void provider_buffer_destroy(){
    buffer_destroy(provider_buffer);
}

void provider_buffer_insert(msg_t* message){
    put_bloccante(provider_buffer,message);
}

msg_t* provider_buffer_read(){
    return get_non_bloccante(provider_buffer);    //get non bloccante 
}