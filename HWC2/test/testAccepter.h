#ifndef TESTACCEPTER_H_
#define TESTACCEPTER_H_

int accepter_buffer_before();
int accepter_buffer_after();

void test_accepter_buffer_insert_oneMessage();
void test_accepter_buffer_insert_eightMessage();
void test_accepter_buffer_delete();
void test_accepter_buffer_deleteAll();
void test_accepter_submitRequestNormal();
void test_accepter_submitRequestPoisonPill();
#endif

