
#include "CUnit/CUnit.h"
#include <stdlib.h>

#include "../reader.h"
#include "../reader_buffer.h"
#include "../reader_list.h"
#include "testReaderList.h"
#include "bufferDomainAssertion.h"
#include <pthread.h>

msg_t* messaggio;
pthread_t readers[2];

void add_twoReaders(reader_t* reader1,reader_t* reader2){
	reader_list_addReader((void*)reader1);
	reader_list_addReader((void*)reader2);
}

void remove_twoReaders(reader_t* reader1,reader_t* reader2){
        reader_destroy(reader1);
	reader_destroy(reader2);
	pthread_join(readers[0],NULL);
	pthread_join(readers[1],NULL);
}

void clean_buffer_reader(reader_t* reader1, reader_t* reader2){
	pthread_create(&readers[0],NULL,reader_consume,(void*)reader1);
        pthread_join(readers[0],NULL);
	pthread_create(&readers[1],NULL,reader_consume,(void*)reader2);
        pthread_join(readers[1],NULL);
}

int reader_list_before(){
	messaggio=msg_init_string((void*)"Messaggio 0");
	return 0;
}
int reader_list_after(){
	free(messaggio);
	return 0;
}

void test_reader_list_isEmpty(){
	reader_list_initialize();
	CU_ASSERT_TRUE(reader_list_isEmpty());
	reader_list_destroy();
}

void test_reader_list_addOneReader(){
	reader_list_initialize();
	reader_t* reader_ritardoUnitario_bufferGenerico=reader_initialize("Reader 1",1,10);
	CU_ASSERT_EQUAL(reader_list_size(),0);
	reader_list_addReader((void*)reader_ritardoUnitario_bufferGenerico);
	CU_ASSERT_PTR_NOT_NULL(reader_list->head);
	CU_ASSERT_PTR_NOT_NULL(reader_list->tail);
	CU_ASSERT_PTR_EQUAL(reader_list->head, reader_list->tail);
	CU_ASSERT_EQUAL(reader_list_size(),1);

        
	reader_list_destroy();
}

void test_reader_list_isNotEmpty(){
	reader_list_initialize();
	reader_t* reader_ritardo5_bufferUnitario=reader_initialize("Reader 2",5,1);
	reader_list_addReader((void*)reader_ritardo5_bufferUnitario);
	CU_ASSERT_FALSE(reader_list_isEmpty());
	reader_list_destroy();
}

void test_reader_list_addTwoReaders(){
	reader_list_initialize();
	CU_ASSERT_EQUAL(reader_list_size(),0);
	reader_t* reader_ritardoUnitario_bufferGenerico=reader_initialize("Reader 1",1,10);
	reader_t* reader_ritardo5_bufferUnitario=reader_initialize("Reader 2",5,1);
	add_twoReaders(reader_ritardoUnitario_bufferGenerico,reader_ritardo5_bufferUnitario);
	CU_ASSERT_PTR_NOT_NULL(reader_list->head);
	CU_ASSERT_PTR_NOT_NULL(reader_list->tail);
	CU_ASSERT_PTR_NOT_EQUAL(reader_list->head, reader_list->tail);
	CU_ASSERT_PTR_EQUAL(reader_list->head->next, reader_list->tail);
	CU_ASSERT_EQUAL(reader_list_size(),2);
	reader_list_destroy();
}

void test_reader_list_kill(){
	reader_list_initialize();
	reader_t* reader_ritardoUnitario_bufferGenerico=reader_initialize("Reader 1",1,10);
	reader_t* reader_ritardo5_bufferUnitario=reader_initialize("Reader 2",5,1);
	add_twoReaders(reader_ritardoUnitario_bufferGenerico,reader_ritardo5_bufferUnitario);
	reader_list_kill();
	ASSERT_BUFFER_CONTAINS_ONLY(reader_ritardoUnitario_bufferGenerico->buffer->read_buffer, (msg_t*)&POISON_PILL_MSG);
	ASSERT_BUFFER_CONTAINS_ONLY(reader_ritardo5_bufferUnitario->buffer->read_buffer, (msg_t*)&POISON_PILL_MSG);
	clean_buffer_reader(reader_ritardoUnitario_bufferGenerico, reader_ritardo5_bufferUnitario);
	reader_list_destroy();	
}

void test_reader_list_broadcast(){
	reader_list_initialize();
	reader_t* reader_ritardoUnitario_bufferGenerico=reader_initialize("Reader 1",1,10);
	reader_t* reader_ritardo5_bufferGenerico=reader_initialize("Reader 2", 5,2);
	add_twoReaders(reader_ritardoUnitario_bufferGenerico,reader_ritardo5_bufferGenerico);
	reader_list_broadcast(messaggio);
        reader_list_broadcast((msg_t*)&POISON_PILL_MSG);
	ASSERT_BUFFER_CONTAINS_ONLY(reader_ritardoUnitario_bufferGenerico->buffer->read_buffer, messaggio);
	ASSERT_BUFFER_CONTAINS_ONLY(reader_ritardo5_bufferGenerico->buffer->read_buffer, messaggio);
	clean_buffer_reader(reader_ritardoUnitario_bufferGenerico, reader_ritardo5_bufferGenerico);
        sleep(1);
	reader_list_destroy();	
}