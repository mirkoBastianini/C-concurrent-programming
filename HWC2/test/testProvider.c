#include "CUnit/CUnit.h"
#include <stdlib.h>

#include "../provider.h"
#include "../provider_buffer.h"
#include "testProvider.h"
#include "bufferDomainAssertion.h"
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define Sleep(ms) sleep((ms)/1000)

msg_t* testing[8];
msgs* content;
pthread_t provider;



int provider_buffer_before(){
	int i;
        char cont[20];
        content = (msgs*)calloc(7,sizeof(msgs));
	for (i=0;i<7;i++){
                sprintf(cont, "Messaggio %d", i);
		testing[i]=msg_init_string((void*)cont);
                content->messages[i] = msg_init_string(cont);
	}
	testing[7]=(msg_t*)&POISON_PILL_MSG;
        content->size=7;
	return 0;
}

int provider_buffer_after(){
	return 0;
}



void test_provider_buffer_insert_oneMessage(){
	provider_buffer_init(8);
	provider_buffer_insert(testing[0]);
	CU_ASSERT_PTR_NULL(provider_buffer->T);
	CU_ASSERT_STRING_EQUAL(provider_buffer->msg_list[provider_buffer->T].content,testing[0]->content);
	provider_buffer_destroy();
}

void test_provider_buffer_insert_eightMessage(){
	provider_buffer_init(8);
	int i;
	for(i=0;i<8;i++){
		provider_buffer_insert(testing[i]);
	}
	ASSERT_BUFFER_EQUALS(provider_buffer,testing);
	provider_buffer_destroy();
}

void test_createMessageSequence(){
	provider_buffer_init(8);
	pthread_create(&provider,NULL,create_messages_streaming,(void*)content);
        pthread_join(provider,NULL);
        sleep(1);
	ASSERT_BUFFER_EQUALS(provider_buffer,testing);
	provider_buffer_destroy();
	
}

void test_provider_buffer_insertAndRead(){
	provider_buffer_init(8);
	provider_buffer_insert(testing[0]);
	msg_t* retrived=provider_buffer_read();
	CU_ASSERT_PTR_NOT_NULL(provider_buffer->T);
	CU_ASSERT_STRING_EQUAL(retrived->content,testing[0]->content);
	provider_buffer_destroy();
}
 