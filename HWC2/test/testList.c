/*
 * Autore: Valter Crescenzi e gli studenti del 
 * Corso di Programmazione Concorrente
 *  
 * testList.c
 *
 * test sulle liste
*/
#include <CUnit/CUnit.h>
#include <stdlib.h>

#include "../list.h"
#include "testList.h"
#include "listDomainAssertion.h"

typedef struct {
	int garbage;
	int padding;
} payload_t;

// 4 elementi da infilare e sfilare dalle liste
payload_t *single, 
          *a, 
          *b, 
          *c; 

// allocati allo startup e dellocati al teardown

int listBefore(void) {
  single  = malloc(sizeof(payload_t));

  a = malloc(sizeof(payload_t));
  b = malloc(sizeof(payload_t));
  c = malloc(sizeof(payload_t));
  return 0;
}

int listAfter(void) {
  free(single);
  free(a);
  free(b);
  free(c);
  return 0;
}

void test_list_init(void){
  list_t* lista = list_init();
  CU_ASSERT_PTR_NOT_NULL(lista);
  CU_ASSERT_PTR_NULL(lista->head);
  CU_ASSERT_PTR_NULL(lista->tail);
}

void test_list_destroy_emptyList(void) {
  list_t* emptyList = list_init();
  list_destroy(emptyList);
  // basta che il test termini correttamente: no asserzioni!
}

void test_list_destroy_singletonList(void) {
  list_t* list = list_init();
  payload_t * elemento = malloc(sizeof(payload_t));
  addElement(list, (void*)elemento);
  list_destroy(list);
  free(elemento);
  // basta che il test termini correttamente: no asserzioni!
}

void test_list_size_emptyList() {
  list_t* emptyList = list_init();
  CU_ASSERT_EQUAL(size(emptyList),0);
  list_destroy(emptyList);
}

void test_list_size_nonEmptyList() {
  list_t* list = list_init();
  addElement(list,single);
  CU_ASSERT_EQUAL(size(list),1);
  list_destroy(list);
}

void test_list_isEmpty_emptyList() {
  list_t* emptyList = list_init();
  CU_ASSERT_TRUE(isEmpty(emptyList));
  list_destroy(emptyList);
}

void test_list_isEmpty_nonEmptyList() {
  list_t* list = list_init();
  addElement(list,single);
  CU_ASSERT_FALSE(isEmpty(list));
  list_destroy(list);
}

void test_addElement_emptyList(void) {
  list_t* listaDaRiempire = list_init();

  addElement(listaDaRiempire, (void*)single);

  CU_ASSERT_PTR_NOT_NULL(listaDaRiempire->head);
  CU_ASSERT_PTR_NOT_NULL(listaDaRiempire->tail);
  CU_ASSERT_PTR_EQUAL(listaDaRiempire->head, listaDaRiempire->tail);

  list_destroy(listaDaRiempire);
}

void test_addElement_2elements(void) {
  list_t* listaDaRiempire = list_init();

  addElement(listaDaRiempire, (void*)a);
  addElement(listaDaRiempire, (void*)b);

  CU_ASSERT_PTR_NOT_NULL(listaDaRiempire->head);
  CU_ASSERT_PTR_NOT_NULL(listaDaRiempire->tail);
  CU_ASSERT_PTR_NOT_EQUAL(listaDaRiempire->head, listaDaRiempire->tail);

  CU_ASSERT_PTR_EQUAL(listaDaRiempire->head->payload, a);
  CU_ASSERT_PTR_EQUAL(listaDaRiempire->tail->payload, b);
  CU_ASSERT_PTR_EQUAL(listaDaRiempire->head->next, listaDaRiempire->tail);
  CU_ASSERT_PTR_NULL(listaDaRiempire->tail->next);

  list_destroy(listaDaRiempire);
}

void test_addElement_3elements(void) {
  list_t* listaDaRiempire = list_init();

  addElement(listaDaRiempire, (void*)a);
  addElement(listaDaRiempire, (void*)b);
  addElement(listaDaRiempire, (void*)c);

  CU_ASSERT_PTR_NOT_NULL(listaDaRiempire->head);
  CU_ASSERT_PTR_NOT_NULL(listaDaRiempire->tail);
  CU_ASSERT_PTR_NOT_EQUAL(listaDaRiempire->head, listaDaRiempire->tail);

  CU_ASSERT_PTR_EQUAL(listaDaRiempire->head->payload, a);
  CU_ASSERT_PTR_EQUAL(listaDaRiempire->head->next->payload, b);
  CU_ASSERT_PTR_EQUAL(listaDaRiempire->tail->payload, c);
  CU_ASSERT_PTR_EQUAL(listaDaRiempire->head->next->next, listaDaRiempire->tail);
  CU_ASSERT_PTR_NULL(listaDaRiempire->tail->next);

  list_destroy(listaDaRiempire);
}

void test_addAndRemoveElement(void) {
  list_t* list = list_init();

  addElement(list, (void*)single);
  removeElement(list, (void*)single);

  CU_ASSERT_TRUE(isEmpty(list));

  list_destroy(list);
}

void test_removeElement_2elements_lastRemain(void) {
  list_t* list = list_init();

  addElement(list, (void*)a);
  addElement(list, (void*)b);
  removeElement(list, (void*)a);

  ASSERT_SINGLETONLIST(list, b);

  list_destroy(list);
}

void test_removeElement_2elements_firstRemain(void) {
  list_t* list = list_init();

  addElement(list, (void*)a);
  addElement(list, (void*)b);
  removeElement(list, (void*)b);

  ASSERT_SINGLETONLIST(list, a);
  list_destroy(list);
}

void test_removeElement_2elements_empties(void) {
  list_t* list = list_init();

  addElement(list, (void*)a);
  addElement(list, (void*)b);
  removeElement(list, (void*)b);
  removeElement(list, (void*)a);

  CU_ASSERT_TRUE(isEmpty(list));

  list_destroy(list);
}

void test_removeElement_3elements_first(void) {
  list_t* list = list_init();

  addElement(list, (void*)a);
  addElement(list, (void*)b);
  addElement(list, (void*)c);
  removeElement(list, (void*)a);

  ASSERT_LIST_EQUAL2(list, b, c);

  list_destroy(list);
}

void test_removeElement_3elements_middle(void) {
  list_t* list = list_init();

  addElement(list, (void*)a);
  addElement(list, (void*)b);
  addElement(list, (void*)c);
  removeElement(list, (void*)b);

  ASSERT_LIST_EQUAL2(list, a, c);

  list_destroy(list);
}

void test_removeElement_3elements_last(void) {
  list_t* list = list_init();

  addElement(list, (void*)a);
  addElement(list, (void*)b);
  addElement(list, (void*)c);
  removeElement(list, (void*)c);

  ASSERT_LIST_EQUAL2(list, a, b);

  list_destroy(list);
}

void test_removeElement_all(void) {
  list_t* list = list_init();

  addElement(list, (void*)a);
  addElement(list, (void*)b);
  addElement(list, (void*)c);
  removeElement(list, (void*)c);
  removeElement(list, (void*)a);
  removeElement(list, (void*)b);

  CU_ASSERT_TRUE(isEmpty(list));

  list_destroy(list);
}

void test_iterator_init_destroy(void) {
  list_t* list = list_init();

  iterator_t* i=iterator_init(list);

  CU_ASSERT_PTR_NOT_NULL(i);
  CU_ASSERT_PTR_EQUAL(i->list, list);
  CU_ASSERT_PTR_EQUAL(i->currentNode, list->head);

  iterator_destroy(i);
  list_destroy(list);
}

void test_hasNext_emptyList(void) {
  list_t* lista = list_init();

  iterator_t* i=iterator_init(lista);

  CU_ASSERT_EQUAL(hasNext(i), 0);

  iterator_destroy(i);
  list_destroy(lista);
}

void test_hasNext_nonEmptyList(void) {
  list_t* lista = list_init();

  addElement(lista, (void*)single);
  iterator_t* i=iterator_init(lista);

  CU_ASSERT_NOT_EQUAL(hasNext(i), 0);
  CU_ASSERT_PTR_EQUAL(i->currentNode, lista->head);

  iterator_destroy(i);
  list_destroy(lista);
}

void test_next_emptyList(void) {
  list_t* lista = list_init();
  iterator_t* it=iterator_init(lista);

  CU_ASSERT_PTR_NULL(next(it));
  CU_ASSERT_PTR_NULL(it->currentNode);

  iterator_destroy(it);
  list_destroy(lista);
}

void test_next_nonEmptyList(void) {
  list_t* lista = list_init();

  addElement(lista, (void*)a);
  addElement(lista, (void*)b);
  addElement(lista, (void*)c);
  iterator_t* i=iterator_init(lista);

  payload_t * corrente = next(i);
  CU_ASSERT_PTR_NOT_NULL(corrente);
  CU_ASSERT_PTR_EQUAL(corrente, lista->head->payload);

  corrente = next(i);
  CU_ASSERT_PTR_NOT_NULL(corrente);
  CU_ASSERT_PTR_EQUAL(corrente, lista->head->next->payload);

  corrente = next(i);
  CU_ASSERT_PTR_NOT_NULL(corrente);
  CU_ASSERT_PTR_EQUAL(corrente, lista->tail->payload);

  corrente = next(i);
  CU_ASSERT_PTR_NULL(corrente);

  iterator_destroy(i);
  list_destroy(lista);
}

void ASSERT_LIST_EQUAL2(list_t *list, void *exp1, void *exp2) {
  CU_ASSERT_PTR_NOT_NULL(list->tail);
  CU_ASSERT_PTR_NOT_NULL(list->head);
  CU_ASSERT_PTR_EQUAL(list->head->payload,exp1);
  CU_ASSERT_PTR_EQUAL(list->tail->payload,exp2);
  CU_ASSERT_PTR_EQUAL(list->tail, list->head->next);
  CU_ASSERT_PTR_NULL(list->tail->next);
}

void ASSERT_SINGLETONLIST(list_t *list, void* expected) {
  CU_ASSERT_PTR_EQUAL(list->tail, list->head);
  CU_ASSERT_PTR_EQUAL(list->tail->payload, expected);
  CU_ASSERT_PTR_NULL(list->tail->next);
}
