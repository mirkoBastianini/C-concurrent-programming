#ifndef POISON_PILL_H_
#define POISON_PILL_H_
#include "message.h"
#include <stdio.h>

#define POISON_PILL ( (msg_t*)&POISON_PILL_MSG )

msg_t* msg_init_pill(void *);
msg_t* msg_copy_pill(msg_t *);
void msg_destroy_pill(msg_t *);

extern const msg_t POISON_PILL_MSG;

#endif 

