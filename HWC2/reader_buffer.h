#ifndef READER_BUFFER_H
#define READER_BUFFER_H


#include "buffer.h"

typedef struct reader_buffer {
	buffer_t* read_buffer;
}reader_buffer_t;

reader_buffer_t* reader_buffer_initialize(int maxsize);
void reader_buffer_destroy(reader_buffer_t* r_buffer);
int reader_buffer_insert(reader_buffer_t* r_buffer, msg_t* message);
msg_t* reader_buffer_consume(reader_buffer_t* r_buffer);
int reader_buffer_numberOfMessage(reader_buffer_t* r_buffer);
int reader_buffer_size(reader_buffer_t* r_buffer);

#endif

