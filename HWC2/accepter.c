
#include "accepter.h"
#include "reader_list.h"
#include <stdlib.h>
#include <math.h>


void *submitRequest(){
    msg_t* request;
    if(reader_list==NULL)
            reader_list_initialize();
    do{
	request= accepter_buffer_delete(); //get _non_bloccante
        if(request!=NULL){
            if(strcmp(request->content,"POISON_PILL_MSG")!=0){
		int delay=rand()%20000;
		reader_t* reader=reader_initialize(request->content, delay, 10);
		reader_list_addReader((void*)reader);
                reader_active(reader);
                
            }else{
                pthread_exit(NULL);
            }
        }
    }while(1);
}

