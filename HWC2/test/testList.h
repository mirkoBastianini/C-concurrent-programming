/*
 * Autore: Valter Crescenzi e gli studenti del 
 * Corso di Programmazione Concorrente
 *  
 * testList.h
 *
 * test sulle liste
*/

#ifndef TESTLIST_H_
#define TESTLIST_H_

int listBefore(void);
int listAfter(void);

void test_list_init(void);
void test_list_destroy_emptyList(void);
void test_list_destroy_singletonList(void);

void test_list_size_emptyList(void);
void test_list_size_nonEmptyList(void);

void test_list_isEmpty_emptyList(void);
void test_list_isEmpty_nonEmptyList(void);

void test_addElement_emptyList(void);
void test_addElement_2elements(void);
void test_addElement_3elements(void);

void test_addAndRemoveElement(void);
void test_removeElement_2elements_lastRemain(void);
void test_removeElement_2elements_firstRemain(void);
void test_removeElement_2elements_empties(void);

void test_removeElement_3elements_first(void);
void test_removeElement_3elements_middle(void);
void test_removeElement_3elements_last(void);
void test_removeElement_all(void);

void test_iterator_init_destroy(void);
void test_hasNext_emptyList(void);
void test_hasNext_nonEmptyList(void);
void test_next_emptyList(void);
void test_next_nonEmptyList(void);

#endif /* TESTLIST_H_ */
