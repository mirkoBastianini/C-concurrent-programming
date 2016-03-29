#include "poison_pill.h"


const msg_t POISON_PILL_MSG={
    "POISON_PILL_MSG",
    msg_init_pill,
    msg_destroy_pill,
    msg_copy_pill
};

msg_t* msg_init_pill(void * v){
    return POISON_PILL;
}

msg_t* msg_copy_pill(msg_t * m){
    return POISON_PILL;
}

void msg_destroy_pill(msg_t * m){
    /**/
}