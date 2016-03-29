#ifndef BUFFERDOMAINASSERTION_H
#define BUFFERDOMAINASSERTION_H

#include "../buffer.h"
#include "../list.h"
#include "../reader.h"

void ASSERT_BUFFER_EQUALS(buffer_t* buffer, msg_t* messages[]);
void ASSERT_BUFFER_NOTEQUAL(buffer_t* buffer, msg_t* messages[]);
void ASSERT_BUFFER_EQUALS2(buffer_t* buffer, msg_t* messages[]);
void ASSERT_BUFFER_CONTAINS_ONLY(buffer_t* buffer, msg_t* message);
void ASSERT_ALL_BUFFER_EQUAL(list_t* list);
void ASSERT_BUFFER_EMPTY(buffer_t* buffer);

#endif 

