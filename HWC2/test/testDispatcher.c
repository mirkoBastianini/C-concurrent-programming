#include "testDispatcher.h"
#include "bufferDomainAssertion.h"
#include "../dispatcher.h"
#include "../provider.h"
#include "CUnit/CUnit.h"
#include <stdlib.h>

reader_t* readers[3];
pthread_t dispatcher;
pthread_t provider;
pthread_t readers_thread[3];
int dim=10;
msgs* sequence;



void create_readers(){
	readers[0]=reader_initialize("Reader 1",1,dim);
	readers[1]=reader_initialize("Reader 2",4,dim);
	readers[2]=reader_initialize("Reader 3",15,dim);
}
void create_readersSlow(){
	readers[0]=reader_initialize("Reader 1",1,dim/3);
	readers[1]=reader_initialize("Reader 2",4,dim/3);
	readers[2]=reader_initialize("Reader 3",20000,dim/3); //reader lento 20 secondi
        reader_active(readers[0]);
        reader_active(readers[1]);
        reader_active(readers[2]);
}

void add_readers(){
	int i;
	for(i=0;i<3;i++){
		reader_list_addReader((void*)readers[i]);
	}
}

int dispatcher_before(){
	
	provider_buffer_init(dim);
        sequence = (msgs*)calloc((dim-1),sizeof(msgs));
        int i;
        char tmp[20];
        char cont[20];
        char *prefix = "Messaggio ";
        strcpy(tmp, prefix);
        strcat(tmp, "%d");      
        sequence->size=dim-1;
	for (i=0;i<dim-1;i++){
                sprintf(cont, tmp, i);
                sequence->messages[i] = msg_init_string((void*)cont);
	}   
	return 0;
}

int dispatcher_after(){
	return 0;
}

void test_dispatch_message(){
        reader_list_initialize();
	create_readers();
	add_readers();
	pthread_create(&provider,NULL,create_messages_streaming,(void*)sequence);
	sleep(2);
	pthread_create(&dispatcher,NULL,dispatch_message,NULL);
	sleep(2);
	pthread_join(dispatcher,NULL);
	ASSERT_ALL_BUFFER_EQUAL(reader_list);
	pthread_join(provider,NULL);
        reader_list_destroy();
	
}

void test_dispatchAndDelete_slow_reader(){
        printf("Reader 3 lento");
        reader_list_initialize();
	create_readersSlow();
	add_readers();
	pthread_create(&provider,NULL,create_messages_streaming,(void*)sequence);
	sleep(2);
	pthread_create(&dispatcher,NULL,dispatch_message,NULL);
	sleep(2);
	pthread_join(dispatcher,NULL);
        sleep(2);
	pthread_join(provider,NULL);
        reader_list_destroy();
}