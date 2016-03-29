#include "provider.h"
#include <unistd.h>
#define Sleep(ms) sleep((ms)/1000)

void* create_messages_streaming(void* content){
    msgs* msg = (msgs*) content;
    int n = msg->size;
    int i;
    for(i=0 ; i<n ; i++){
        
        provider_buffer_insert(msg->messages[i]);
        Sleep(1);
    }
    provider_buffer_insert((msg_t*)&POISON_PILL_MSG);
    pthread_exit(NULL);
}