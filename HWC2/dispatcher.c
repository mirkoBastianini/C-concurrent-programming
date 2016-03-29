#include "dispatcher.h"

void *dispatch_message(){   
    int poison=1; //finchè non è arrivata la poison pill
	while(poison){
		msg_t* message=provider_buffer_read();
		if (message!=NULL){
                    if (strcmp(message->content,"POISON_PILL_MSG")==0){//è una poison pill
			reader_list_kill();//uccido i reader nella lista
			poison=0;
                    }else{
			reader_list_broadcast(message);
			sleep(1);
                    }
		}
	}
    printf("\ndispatcher terminato=> POISON PILL inoltrata");
	pthread_exit(NULL);
}
