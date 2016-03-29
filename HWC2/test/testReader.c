#include "CUnit/CUnit.h"
#include <stdlib.h>

#include "../reader.h"
#include "../reader_buffer.h"
#include "../reader_list.h"
#include "testReader.h"
#include "bufferDomainAssertion.h"
#include <pthread.h>
#include <stdint.h>

reader_t* reader_ritardo5_bufferUnitario;
reader_t* reader_ritardoUnitario_bufferGenerico;
msg_t* messages[8];
pthread_t reader_thread;

void reader_fixture(reader_t* reader, msg_t* messages[]){
	int i;
	for(i=0;i<8;i++){
		reader_insert(reader_ritardoUnitario_bufferGenerico,messages[i]);
	}
}

int reader_before1(){
	reader_list_initialize();
	reader_ritardo5_bufferUnitario=reader_initialize("Reader 1",5,1);
	reader_ritardoUnitario_bufferGenerico=reader_initialize("Reader 2",1,8);
        int i;
        char tmp[20];
        char cont[20];
        char *prefix = "Messaggio ";
        strcpy(tmp, prefix);
        strcat(tmp, "%d");
        for (i=0; i<7; i++) {
            sprintf(cont, tmp, i);
            messages[i] = msg_init_string((void*)cont);
        }
        messages[i]=msg_init_string((void*)POISON_PILL_MSG.content);
	return 0;
}
int reader_after1(){
	
	reader_list_destroy();
	return 0;
}

int reader_before2(){
	reader_list_initialize();
	reader_ritardoUnitario_bufferGenerico=reader_initialize("Reader 2",1,8);
	int i;
        char tmp[20];
        char cont[20];
        char *prefix = "Messaggio ";
        strcpy(tmp, prefix);
        strcat(tmp, "%d");
        for (i=0; i<7; i++) {
            sprintf(cont, tmp, i);
            messages[i] = msg_init_string((void*)cont);
        }
        messages[i]=msg_init_string((void*)POISON_PILL_MSG.content);
	reader_list_addReader((void*)reader_ritardoUnitario_bufferGenerico);
	return 0;
}
int reader_after2(){
	
	reader_list_destroy();
	return 0;
}

void test_reader_insert_oneMessage(){
	reader_insert(reader_ritardo5_bufferUnitario,messages[0]);
	CU_ASSERT_PTR_NULL(reader_ritardo5_bufferUnitario->buffer->read_buffer->T);
	CU_ASSERT_STRING_EQUAL(reader_ritardo5_bufferUnitario->buffer->read_buffer->msg_list[reader_ritardo5_bufferUnitario->buffer->read_buffer->T].content,messages[0]->content);
}

void test_reader_insert_eightMessage(){
	int i;
	for(i=0;i<8;i++){
		reader_insert(reader_ritardoUnitario_bufferGenerico,messages[i]);
	}
	ASSERT_BUFFER_EQUALS(reader_ritardoUnitario_bufferGenerico->buffer->read_buffer,messages);
}

void test_reader_consume(){
	reader_fixture(reader_ritardoUnitario_bufferGenerico,messages);
	ASSERT_BUFFER_EQUALS(reader_ritardoUnitario_bufferGenerico->buffer->read_buffer,messages);
	pthread_create(&reader_thread,NULL,reader_consume,(void*)reader_ritardoUnitario_bufferGenerico);
	pthread_join(reader_thread,NULL);
        sleep(1);
	ASSERT_BUFFER_EMPTY(reader_ritardoUnitario_bufferGenerico->buffer->read_buffer);
        reader_destroy(reader_ritardoUnitario_bufferGenerico);
        sleep(2);
	
}