#include <CUnit/CUnit.h>
#include "bufferDomainAssertion.h"
#include <string.h>


void ASSERT_BUFFER_EQUALS(buffer_t* buffer, msg_t* messages[]){
	int i;
	for(i=0;i<buffer->size;i++){
		CU_ASSERT_STRING_EQUAL(buffer->msg_list[buffer->T].content,messages[i]->content);
		buffer->T=(buffer->T+1)%(buffer->size);
	}
}

void ASSERT_BUFFER_NOTEQUALS(buffer_t* buffer, msg_t* messages[]){
	int i;
	for(i=0;i<buffer->size;i++){
		CU_ASSERT_STRING_NOT_EQUAL(buffer->msg_list[buffer->T].content,messages[i]->content);
		buffer->T=(buffer->T+1)%(buffer->size);
	}
}

void ASSERT_TWO_BUFFER_EQUAL(buffer_t* buffer1, buffer_t* buffer2){
	int i;
	for(i=0;i<buffer1->size;i++){
		CU_ASSERT_STRING_EQUAL(buffer1->msg_list[buffer1->T].content,buffer2->msg_list[buffer2->T].content);
		buffer1->T=(buffer1->T+1)%(buffer1->size);
		buffer2->T=(buffer2->T+1)%(buffer2->size);
	}
}


void ASSERT_BUFFER_EQUALS2 (buffer_t* buffer, msg_t* messages[]){
	int i;
	for(i=0;i<buffer->k;i++){
		CU_ASSERT_STRING_EQUAL(buffer->msg_list[buffer->T].content,messages[i]->content);
		buffer->T=(buffer->T+1)%(buffer->size);
	}
}
	
void ASSERT_BUFFER_CONTAINS_ONLY(buffer_t* buffer, msg_t* message){
	CU_ASSERT_STRING_EQUAL(buffer->msg_list[buffer->T].content,message->content);
	CU_ASSERT_PTR_NOT_EQUAL(&(buffer->msg_list[buffer->T+1]),message);
}


void ASSERT_ALL_BUFFER_EQUAL(list_t* list){
	iterator_t* iterator= iterator_init(list);
	int dim=size(list);
	int i=0;
	reader_t* readers[dim];
	while(hasNext(iterator)&&i<dim){
		readers[i]=(reader_t*)next(iterator);
		i++;
	}
	for (i=0; i<(dim-1); i++){
		ASSERT_TWO_BUFFER_EQUAL(readers[i]->buffer->read_buffer,readers[i+1]->buffer->read_buffer);
	}
}


void ASSERT_BUFFER_EMPTY(buffer_t* buffer){
	int i;
	for(i=0;i<buffer->size;i++){
		CU_ASSERT_PTR_NULL(buffer->msg_list[i].content);
	}
}