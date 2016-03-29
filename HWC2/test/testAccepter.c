#include "CUnit/CUnit.h"
#include <stdlib.h>

#include "../accepter.h"
#include "../accepter_buffer.h"
#include "../reader_list.h"
#include "testAccepter.h"
#include "bufferDomainAssertion.h"
#include <pthread.h>

msg_t* messages[8];
reader_t* reader1;
pthread_t requester[8];



void accepter_buffer_fixture_1(msg_t* messages[]){
	int i;
	for(i=0;i<7;i++){
		pthread_create(&requester[i],NULL,accepter_buffer_insert,(void*)messages[i]);
                pthread_join(requester[i],NULL);
	}

        pthread_create(&requester[7],NULL,accepter_buffer_insert,(void*)&POISON_PILL_MSG);
        pthread_join(requester[7],NULL);
}

void accepter_buffer_fixture_2(msg_t* poison_pill){
	pthread_create(&requester[0],NULL,accepter_buffer_insert,(void*)poison_pill);
	pthread_join(requester[0],NULL);

}

void reader_list_fixture(reader_t* reader){
	reader1= reader_initialize("Reader 1",1,10);
	reader_list_addReader((void*)reader1);
}

int accepter_buffer_before(){
	int i;
        char tmp[20];
            char cont[20];
            char *prefix = "Messaggio ";
            strcpy(tmp, prefix);
            strcat(tmp, "%d");
	for (i=0;i<8;i++){
            sprintf(cont, tmp, i);
            messages[i] = msg_init_string((void*)cont);
	}
	return 0;
}

int accepter_buffer_after(){

	return 0;
}

void test_accepter_buffer_insert_oneMessage(){
	accepter_buffer_initialize(8);
	pthread_create(&requester[0],NULL,accepter_buffer_insert,(void*)messages[0]);
	pthread_join(requester[0],NULL);
	CU_ASSERT_PTR_NULL(accepter_buffer->T);
	CU_ASSERT_PTR_EQUAL(accepter_buffer->k,1);
        CU_ASSERT_STRING_EQUAL(accepter_buffer->msg_list[0].content,messages[0]->content);
	accepter_buffer_destroy();
}

void test_accepter_buffer_insert_eightMessage(){
	accepter_buffer_initialize(8);
	int i;
	for(i=0;i<8;i++){
		pthread_create(&requester[i],NULL,accepter_buffer_insert,(void*)messages[i]);
                pthread_join(requester[i],NULL);
	}
	CU_ASSERT_EQUAL(accepter_buffer->k,8);
        for(i=0;i<8;i++){
            CU_ASSERT_STRING_EQUAL(accepter_buffer->msg_list[i].content,messages[i]->content);
	}

	accepter_buffer_destroy();
}

void test_accepter_buffer_delete(){
	accepter_buffer_initialize(8);
	accepter_buffer_fixture_1(messages);
	int i,j;
	msg_t* testing[7];
	for(i=0, j=1;i<7,j<8;i++,j++){
		testing[i]=messages[j];
	}
	msg_t* retrieve= accepter_buffer_delete();
	CU_ASSERT_PTR_NOT_NULL(retrieve->content);
	CU_ASSERT_EQUAL(accepter_buffer->k,7);
	accepter_buffer_destroy();
}

void test_accepter_buffer_deleteAll(){
	accepter_buffer_initialize(8);
	accepter_buffer_fixture_1(messages);
	CU_ASSERT_EQUAL(accepter_buffer->k,8);
	accepter_buffer_deleteAll();
	CU_ASSERT_EQUAL(accepter_buffer->k, 0);
	accepter_buffer_destroy();
}

void test_accepter_submitRequestNormal(){
	accepter_buffer_initialize(8);
	accepter_buffer_fixture_1(messages);
	reader_list_initialize();
        pthread_t acc;
	pthread_create(&acc,NULL,submitRequest,NULL);
	pthread_join(acc,NULL);
        iterator_t* iterator=iterator_init(reader_list);
	CU_ASSERT_TRUE(hasNext(iterator));
        int i;
        int s=size(reader_list);
        for(i=0;i<s;i++){
            reader_t* reader= (reader_t*)next(iterator);
            pthread_cancel(*reader->thread);
            pthread_join(*reader->thread,NULL);
            CU_ASSERT_STRING_EQUAL(reader->name,messages[i]->content);
        }
        
	reader_list_destroy();
	accepter_buffer_destroy();
}

void test_accepter_submitRequestPoisonPill(){
	accepter_buffer_initialize(8);
	accepter_buffer_fixture_2((msg_t*)&POISON_PILL_MSG);
	reader_list_initialize();
	reader_list_fixture(reader1);
	CU_ASSERT_STRING_EQUAL(accepter_buffer->msg_list[0].content,POISON_PILL_MSG.content);
        CU_ASSERT_EQUAL(accepter_buffer->k,1);
        CU_ASSERT_EQUAL(size(reader_list),1);
        pthread_t acc;
	pthread_create(&acc,NULL,submitRequest,NULL);
	pthread_join(acc,NULL);
        CU_ASSERT_EQUAL(accepter_buffer->k,0);
	CU_ASSERT_EQUAL(size(reader_list),1);
	reader_list_destroy();
	accepter_buffer_destroy();
}
